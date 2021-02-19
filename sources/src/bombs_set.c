#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <bomb.h>
#include <bombs_set.h>
#include <lists.h>
#include <mob.h>
#include <map.h>
#include <audio.h>


static struct List* list_bomb= NULL;

struct List* get_list_bomb(){
	return list_bomb;
}

void bombs_set_free(){
    list_free(list_bomb,NULL);
    list_bomb=NULL;
}

void bombs_set_update(struct game* game){   //type 0 in normal, type =1 for save
    struct player* player =game_get_player(game);
    uint16_t elapsed_time= game_get_elapsed_time(game);
    struct List* list= list_bomb;
    struct List* prev= NULL;
    struct Bomb* bomb;
    uint8_t bomb_level;
    struct map* map;
    uint8_t is_current_level;

    uint8_t status;
    while (list!=NULL){
        bomb=(struct Bomb*) list->element;
        if (bomb==NULL){
            if (prev==NULL){
                list_bomb=list->next;
                free(list);
                list=list_bomb;
            }
            else{
                prev->next=list->next;
                free(list);
                list=prev->next;
            }
            continue;
        }
        bomb_level=bomb_get_level(bomb);
        map=game_get_map(game,bomb_level);
        is_current_level= (game_get_level(game)==bomb_level);
        status=bomb_update(bomb,map,elapsed_time,is_current_level);
        if (status==2){
            mob_update_map_ia(map,player_get_bomb_range(player),player_get_x(player),player_get_y(player));
        }
        if (!(status)){

            player_inc_nb_bomb(player);
            if (prev==NULL){
                list_bomb=list->next;
                free(list);
                list=list_bomb;
            }
            else{
                prev->next=list->next;
                free(list);
                list=prev->next;
            }
            bomb_explode(bomb,map,player,is_current_level);
            free(bomb);

        }
        else{
            prev=list;
            list=list->next;
        }
    }
}
uint8_t bombs_set_at_pos_explode(int x, int y, uint8_t level,struct player* player,struct map* map,uint8_t is_current_level){
     struct List* list= list_bomb;
    if (list== NULL){return(1);}
    struct Bomb* bomb=(struct Bomb*) list->element;
    if (bomb_get_x(bomb)==x && bomb_get_y(bomb)==y && bomb_get_level(bomb)==level ){
        list_bomb=list->next;
        bomb_explode(bomb,map,player,is_current_level);
        list->element=NULL;
        free(bomb);
        player_inc_nb_bomb(player);
        return(0);
    }
    while (list->next!=NULL){
        struct Bomb* bomb=(struct Bomb*) list->next->element;
      if (bomb_get_x(bomb)==x && bomb_get_y(bomb)==y && bomb_get_level(bomb)==level ){
          struct Bomb* bomb=list->next->element;
          list->next->element=NULL;
          bomb_explode(bomb,map,player,is_current_level);
          free(bomb);
          player_inc_nb_bomb(player);
          return(0);
      }
      else {
          list=list->next;
      }
    }
    return(1); 
}

void bombs_set_create_bomb(int x, int y, int level,int range){
    list_bomb = list_add(list_bomb,(void*) bomb_init(x,y,level,range));
}

void bombs_set_load_bomb(int x, int y, int levelandrange, int timer){
	list_bomb = list_add(list_bomb, (void*) bomb_make(x, y, levelandrange,timer));
}


