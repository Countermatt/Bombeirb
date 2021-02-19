#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <vfx.h>
#include <lists.h>
#include <player.h>




static struct List* list_vfx= NULL;
static struct List* list_fg= NULL;


void vfx_set_free(){
    list_free(list_vfx,&vfx_free);
    list_vfx=NULL;
}
void vfx_set_foregrounds_free(){
    list_free(list_fg,NULL);
    list_fg=NULL;
}

void vfx_set_add_pos_vfx(uint8_t level, int x, int y,enum vfx_type type,int duration)
{
    list_vfx = list_add(list_vfx,(void*) vfx_pos_init(level, x, y,type,duration));
}
void vfx_set_add_player_vfx(struct player* player,enum vfx_type type,int duration)
{
    list_vfx = list_add(list_vfx,(void*) vfx_onplayer_init(player,type,duration));
}
void vfx_set_add_foreground(uint8_t x, uint8_t y,uint8_t type)
{
    list_fg = list_add(list_fg,(void*) vfx_foreground_init(x, y,type));
}

void vfx_set_foreground_display(){
    struct List* list= list_fg;
    while (list!=NULL){
        vfx_foreground_display((struct Foreground*) list->element);
        list=list->next;
    }
}

void vfx_set_display(uint8_t level,uint16_t elapsed_time){
    struct List* list= list_vfx;
    struct List* prev= NULL;
    while (list!=NULL){
        if (vfx_update(list->element,level,elapsed_time)){
            if (prev==NULL){
                list_vfx=list->next;
                vfx_free(list->element);
                free(list);
                list=list_vfx;
            }
            else{
                prev->next=list->next;
                vfx_free(list->element);
                free(list);
                list=prev->next;

            }

        }
        else{
            prev=list;
            list=list->next;
        }
    }
}
