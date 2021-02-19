#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <constant.h>
#include <map.h>
#include <game.h>
#include <bomb.h>
#include <bombs_set.h>
#include <SDL/SDL.h>
#include <vfx_set.h>
#include <audio.h>
#include <mob_set.h>


struct Bomb
{
	uint8_t levelAndRange; // the three first bites are used for world the others for the bonuses
	uint16_t timer;
	uint16_t initialTimer;
	uint8_t x, y;
};

uint8_t bomb_get_level(struct Bomb* bomb){
    return(bomb->levelAndRange>>5);
}
uint8_t bomb_get_range(struct Bomb* bomb){
    return((bomb->levelAndRange & 0x1F) +1);
}
int bomb_get_x(struct Bomb* bomb){
    assert(bomb);
    return(bomb->x);
}

int bomb_get_y(struct Bomb* bomb){
    assert(bomb);
    return(bomb->y);
}

uint8_t bomb_get_levelAndRange(struct Bomb* bomb){
	assert(bomb);
	return(bomb->levelAndRange);
}

uint16_t bomb_get_timer(struct Bomb* bomb){
	assert(bomb);
	return(bomb->timer);
}

uint8_t bomb_update(struct Bomb* bomb,struct map* map,uint16_t elapsed_time,uint8_t is_current_level){
    uint8_t tilt;
	if (elapsed_time>= (bomb->timer)){
		return(0);
	}
	else{
        bomb->timer=bomb->timer-elapsed_time;
        tilt=(NB_FRAME_BOMB*(bomb->initialTimer-bomb->timer))/bomb->initialTimer;
        if (is_current_level && (map_get_cell_subtype(map,bomb->x,bomb->y)!=tilt)){
            audio_add_sound(AUDIO_FUSE);
            map_place_bomb(map,bomb->x,bomb->y,tilt);
            if (tilt==NB_FRAME_BOMB-1){
                return(2);
            }
        }

		return(1);
	}

}
void bomb_explode_direction(int x, int y ,char mv_x, char mv_y,unsigned char range,uint8_t level ,struct map* map,struct player* player,uint8_t is_current_level){
    x+=mv_x;
    y+=mv_y;
    if (range>0 && map_is_inside(map,x,y)){
        int cell=map_get_cell_type(map,x,y);
        switch (cell)
        {
            case CELL_BOX:
                if (is_current_level){
                    audio_add_sound(AUDIO_BOX_BREAK);
                }
                map_break_box(map,x,y,level);
                vfx_set_add_pos_vfx(level,x,y,EFFECT_EXPLOSION,200+100*(range-1));
                break;
            case CELL_BONUS:
                map_set_cell_type(map,x,y,CELL_EMPTY);
                vfx_set_add_pos_vfx(level,x,y,EFFECT_EXPLOSION,200+100*(range-1));
                bomb_explode_direction(x,y,mv_x,mv_y, range-1,level, map, player,is_current_level);
            case CELL_KEY:
                vfx_set_add_pos_vfx(level,x,y,EFFECT_EXPLOSION,200+100*(range-1));
                bomb_explode_direction(x,y,mv_x,mv_y, range-1,level, map, player,is_current_level);
                break;
            case CELL_EMPTY:
                if (is_current_level && player_get_x(player)==x && player_get_y(player)==y){
                    player_damage(player);
                }
                bomb_explode_direction(x,y,mv_x,mv_y, range-1,level, map, player,is_current_level);
                vfx_set_add_pos_vfx(level,x,y,EFFECT_EXPLOSION,200+100*(range-1));
                break;
            case CELL_BOMB: // the player can be on top of a bomb just after placing it
                bombs_set_at_pos_explode(x, y, level, player, map, is_current_level);
                break;
            case CELL_MONSTER:
                mob_set_damage(map,level,map_get_cell_subtype(map,x,y));
                bomb_explode_direction(x,y,mv_x,mv_y, range-1,level, map, player,is_current_level);
                vfx_set_add_pos_vfx(level,x,y,EFFECT_EXPLOSION,200+100*(range-1));
                break;
            default:
                break;
        }
    }
}


void bomb_explode(struct Bomb* bomb, struct map* map, struct player* player,uint8_t is_current_map){
    int x =bomb->x;
    int y = bomb->y;
    if (is_current_map){
        audio_add_sound(AUDIO_EXPLOSION);
        if (player_get_x(player)==x && player_get_y(player)==y){
            player_damage(player);
        }
    }
    map_set_cell_type(map,x,y,CELL_EMPTY);
    int range= bomb_get_range(bomb);
    vfx_set_add_pos_vfx(bomb_get_level(bomb),x,y,EFFECT_EXPLOSION,200+100*range);
    bomb_explode_direction(x,y,0,1,range,bomb_get_level(bomb),map,player,is_current_map);
    bomb_explode_direction(x,y,0,-1,range,bomb_get_level(bomb),map,player,is_current_map);
    bomb_explode_direction(x,y,1,0,range,bomb_get_level(bomb),map,player,is_current_map);
    bomb_explode_direction(x,y,-1,0,range,bomb_get_level(bomb),map,player,is_current_map);
    if(is_current_map){
        mob_update_map_ia(map,player_get_bomb_range(player),player_get_x(player),player_get_y(player));
    }
}


struct Bomb* bomb_init(uint8_t x, uint8_t y, uint8_t level, uint8_t range){ 
	struct Bomb *bomb=(struct Bomb*) malloc(sizeof(struct Bomb));
	bomb->initialTimer = DEFAULT_BOMB_TIMER*1000;
	bomb->timer = DEFAULT_BOMB_TIMER*1000;
	bomb->levelAndRange= (level<<5) + (range-1);
	bomb->x=x;
	bomb->y=y;
    return(bomb);
}

struct Bomb* bomb_make(int x, int y,int  levelandrange,int timer){
	struct Bomb *bomb=(struct Bomb*) malloc(sizeof(struct Bomb));
	bomb->initialTimer = DEFAULT_BOMB_TIMER*1000;
	bomb->timer = timer;
	bomb->levelAndRange= levelandrange;
	bomb->x=x;
	bomb->y=y;
    return(bomb);
}