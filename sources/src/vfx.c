#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <vfx.h>
#include <sprite.h>
#include <window.h>
#include <constant.h>
#include <player.h>

#define NB_FRAME_EXPLOSION 4
#define NB_FRAME_SMOKE 4
#define NB_FRAME_KEY 4
#define NB_FRAME_BITE 9
#define NB_FRAME_BLAST 10

struct Vfx
{
	uint8_t type:3;
    uint8_t displayType:1; //0 for onpos 1 for on player
	uint16_t timeLeft; 
	uint16_t duration;
	void* data;
};
//struct to contain data
    struct Pos{
        uint8_t level:3; 
        uint8_t x,y;
    };
    static struct Pos* make_pos(uint8_t level,uint8_t x ,uint8_t y){
        struct Pos* pos = (struct Pos*) malloc(sizeof(struct Pos));
        pos->level=level;pos->x=x;pos->y=y; return(pos); }
struct Foreground
{
	uint8_t type; 
	uint8_t x, y;
};

uint8_t vfx_get_displayType(struct Vfx* vfx){
    return(vfx->displayType);
}

uint8_t vfx_get_type(struct Vfx* vfx){
    return(vfx->type);
}

void vfx_free(void* vfx_void){
    struct Vfx* vfx =(struct Vfx*) vfx_void;
    switch (vfx->displayType)
    {
        case 0:
            free(vfx->data);
            free(vfx);
            break;
        case 1:
            free(vfx);
        default:
            break;
    }

}

struct Vfx* vfx_pos_init(uint8_t level, int x, int y,enum vfx_type type,int duration){
	struct Vfx *vfx=(struct Vfx*) malloc(sizeof(struct Vfx));
	vfx->duration = duration ;
	vfx->timeLeft =  duration;
	vfx->displayType=0;
    vfx->type= type;
	vfx->data=make_pos(level,x,y);
	return vfx;
}
struct Vfx* vfx_onplayer_init(struct player* player,enum vfx_type type,int duration){
	struct Vfx *vfx=(struct Vfx*) malloc(sizeof(struct Vfx));
	vfx->duration = duration ;
	vfx->timeLeft =  duration;
    vfx->displayType=1;
    vfx->type= type;
	vfx->data=player;
	return vfx;
}
struct Foreground* vfx_foreground_init(uint8_t x, uint8_t y,uint8_t type){
	struct Foreground *fg=(struct Foreground*) malloc(sizeof(struct Foreground));
	fg->type=type;
	fg->x=x;
	fg->y=y;
    
	return fg;
}

void vfx_foreground_display(struct Foreground* fg){
    window_display_image(sprite_get_wall_top(fg->type),fg->x*SIZE_BLOC, fg->y*SIZE_BLOC);
}

uint8_t vfx_update(struct Vfx* vfx,uint8_t level,uint16_t elapsed_time){

    if( elapsed_time < vfx->timeLeft){
        vfx->timeLeft-= elapsed_time;
        switch (vfx->displayType)
        {
            case 0:
                ;//not very clean but I don't understand why declaration are not permited just after a statement
                struct Pos *pos = (struct Pos*) (vfx->data) ;
                if (level==pos->level){
                switch (vfx_get_type(vfx))
                    {   
                        case EFFECT_EXPLOSION:
                            window_display_image(sprite_get_explosion((NB_FRAME_EXPLOSION*(vfx->duration- vfx->timeLeft))/vfx->duration), pos->x*SIZE_BLOC, pos->y*SIZE_BLOC);
                            break;
                        case EFFECT_SMOKE:
                            window_display_image(sprite_get_smokes((NB_FRAME_SMOKE*(vfx->duration- vfx->timeLeft))/vfx->duration), pos->x*SIZE_BLOC, pos->y*SIZE_BLOC);
                            break;
                        default:
                            break;
                    }
                }
                break;
            case 1:
                ;//not very clean but I don't understand why declaration are not permited just after a statement
                struct player *player = (struct player*) (vfx->data);
                uint16_t xv,yv;
                player_get_visual_pos(player,&xv,&yv);
                switch (vfx_get_type(vfx))
                {   
                    case EFFECT_PLAYER_SHIELD:
                        window_display_image(sprite_get_shield(), xv, yv);
                        break;
                    case EFFECT_PLAYER_BITE:
                        window_display_image(sprite_get_bites((NB_FRAME_BITE*(vfx->duration- vfx->timeLeft))/vfx->duration), xv-12,yv-12);
                        break;
                    case EFFECT_PLAYER_BLAST:
                        window_display_image(sprite_get_blasts((NB_FRAME_BLAST*(vfx->duration- vfx->timeLeft))/vfx->duration), xv-12,yv-12);
                        break;
                    case EFFECT_PLAYER_KEY:
                        window_display_image(sprite_get_keys((NB_FRAME_KEY*(vfx->duration- vfx->timeLeft))/vfx->duration), xv-12,yv-12);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        return(0);
    }
    else{
        //end of the visual effect -> delete it from vfx set
        return(1);
    } 
}



