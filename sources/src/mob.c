#include <map.h>
#include <player.h>
#include <constant.h>
#include <stdlib.h>
#include <window.h>
#include <sprite.h>
#include <lists.h>
#include <vfx.h>
#include <vfx_set.h>
#include <audio.h>

struct Mob{
	enum direction direction:2;
  uint8_t x,y;
  uint8_t status:2; 
  uint8_t is_in_box:1;
	uint8_t range;
	uint8_t lifes:3;
	uint16_t moveCooldown;
	uint16_t moveCooldownInitial;
};

#define GRID(i,j) w_map_ia*(i)+(j)


static void direction_to_pos(uint8_t x , uint8_t y,enum direction direction ,uint8_t* newx ,uint8_t* newy);
static void* make_pos(uint8_t x,uint8_t y);
static void* make_pos_and_direction(uint8_t x,uint8_t y,uint8_t dir);
static enum direction opposite_direction(enum direction direction);

static uint8_t* map_ia=NULL;
static uint8_t w_map_ia;
static uint8_t h_map_ia;

uint16_t mob_get_moveCooldownInitial(struct Mob* mob){
  return(mob->moveCooldownInitial);
}
uint8_t mob_get_x(struct Mob* mob){
  return(mob->x);
}
uint8_t mob_get_y(struct Mob* mob){
  return(mob->y);
}

void mob_init_map_ia(uint8_t h,uint8_t w){
  extern uint8_t* map_ia;
  extern uint8_t w_map_ia;
  extern uint8_t h_map_ia;
  if (map_ia!=NULL){
    free(map_ia);
    map_ia=NULL;
  }
  w_map_ia=w;
  h_map_ia=h;
  map_ia= (uint8_t*) (malloc(h*w));
  uint8_t i,j;
  for (i=0;i<h;i++){
    for (j=0;j<w;j++){
      map_ia[GRID(i,j)]=4;
   } 
  }
}

void mob_free_map_ia(){
  free(map_ia);
  map_ia=NULL;
}
static void* make_pos(uint8_t x,uint8_t y){
  uint8_t* pos= (uint8_t*) malloc(2*sizeof(uint8_t));
  pos[0]=x;
  pos[1]=y;
  return((void*) pos);
}
static void* make_pos_and_direction(uint8_t x,uint8_t y,uint8_t dir){
  uint8_t* posdir= (uint8_t*) malloc(3*sizeof(uint8_t));
  posdir[0]=x;
  posdir[1]=y;
  posdir[2]=dir;
  return((void*) posdir);
}

static enum direction opposite_direction(enum direction direction){
  switch (direction)
  {
    case NORTH:
      return(SOUTH);
      break;
    case SOUTH:
      return(NORTH);
      break;
    case WEST:
      return(EAST);
      break;
    case EAST:
      return(WEST);
      break;
    default:
      return(NORTH);
      break;
  }
}


static void aux_cross(struct List** list,struct map* map,uint8_t range,uint8_t x,uint8_t y, int8_t dx,int8_t dy){
  if ((range>0) && map_is_inside(map,x,y)){
    uint8_t cell=map_get_cell_type(map,x,y);
    if (cell==CELL_EMPTY || cell==CELL_MONSTER){
      (*list)= list_add((*list),make_pos(x,y));
      map_ia[GRID(y,x)]=6;
      aux_cross(list,map,range-1,x+dx,y+dy,dx,dy);
    }
  }
}
//list will be filled with the processed compartments (a cross of a size of range around x,y)
static void cross(struct List** list,struct map* map,uint8_t range,uint8_t x,uint8_t y){
  aux_cross(list,map,range,x+1,y,1,0);
  aux_cross(list,map,range,x-1,y,-1,0);
  aux_cross(list,map,range,x,y+1,0,1);
  aux_cross(list,map,range,x,y-1,0,-1);
}

static void aux_delete_from_list(struct List** list, struct List** prev , struct List** cursor ){
  if (*prev==NULL){
    *list=(*cursor)->next;
    free((*cursor)->element);
    free(*cursor);
    *cursor=*list;
  }
  else{
    (*prev)->next=(*cursor)->next;
    free((*cursor)->element);
    free(*cursor);
    *cursor=(*prev)->next;
  }
}
static void aux_door_surroundings_fobidden(uint8_t x, uint8_t y){
  if(1<x){
    map_ia[GRID(y,x-1)]=7;
  }
  if(1<y){
    map_ia[GRID(y-1,x)]=7;
  }
  if(x<w_map_ia-1){
    map_ia[GRID(y,x+1)]=7;
  }
  if(y<h_map_ia-1){
    map_ia[GRID(y+1,x)]=7;
  }
}

void mob_update_map_ia(struct map* map,uint8_t bomb_range ,uint8_t x, uint8_t y){
  /*0-3 cardinal directions
  4 undecided
  5 don't move
  6 danger
  7 forbbiden
  */
  struct List* list_to_do=NULL;
  struct List* list_start=NULL;
  struct List* list_next=NULL;
  struct List* list_danger=NULL;
  extern uint8_t* map_ia;
  extern uint8_t w_map_ia;
  extern uint8_t h_map_ia;
  for (int i=0;i<h_map_ia;i++){ 
    for (int j=0;j<w_map_ia;j++){
      map_ia[GRID(i,j)]=4;
   }
  }
  uint8_t cell;
  for (int i=0;i<h_map_ia;i++){ 
    for (int j=0;j<w_map_ia;j++){
      cell= map_get_cell_type(map,j,i);
      if (cell==CELL_DOOR){
        aux_door_surroundings_fobidden(j,i);
      }
      else if (cell==CELL_BOMB && (map_get_cell_subtype(map,j,i)==3)){
        cross(&list_danger,map,bomb_range,j,i);
      }
   }
  }

  uint8_t* pos;
  enum direction dir;
  list_to_do=list_add(list_to_do,make_pos(x,y));
  map_ia[GRID(y,x)]=5;
  list_start=list_to_do;
  do {
    while (list_to_do!=NULL){ //it is a djikstra like way to go through the map
      pos=(uint8_t*) list_to_do->element;
      for (dir=0; dir<4; dir++){
        direction_to_pos(pos[0],pos[1],dir,&x,&y);
        if (map_is_inside(map,x,y)  && (map_get_cell_type(map,x,y)==CELL_EMPTY || map_get_cell_type(map,x,y)==CELL_MONSTER)){
          if (map_ia[GRID(y,x)]==4 ){
            if (map_ia[GRID(pos[1],pos[0])]%8<6){
              map_ia[GRID(y,x)]=opposite_direction(dir);
              list_next= list_add(list_next,make_pos(x,y));
            }else{
              map_ia[GRID(y,x)]=5;
              list_next= list_add(list_next,make_pos(x,y));
            }
          }
          else if (map_ia[GRID(y,x)]==6){//we stack two directions (for two IA) on one matrice here it concerns the ia that continue it's way when it is in danger
            map_ia[GRID(y,x)]=8*(1+opposite_direction(dir))+6;
            list_next= list_add(list_next,make_pos(x,y)); 
          }
        }
      }
      list_to_do=list_to_do->next;
    }
    list_free(list_start,NULL);
    list_to_do=list_next;
    list_start=list_to_do;
    list_next=NULL;
  }while(list_to_do!=NULL);

  uint8_t danger_list_has_shrunk=1;
  struct List* cursor_danger=NULL;
  struct List* prev=NULL;
  while (danger_list_has_shrunk && (list_danger!=NULL)){
    danger_list_has_shrunk=0;
    cursor_danger=list_danger;
    prev=NULL;
    while (cursor_danger!=NULL)
    {
      pos=(uint8_t*) cursor_danger->element;
      x=pos[0];
      y=pos[1];
      if(map_is_inside(map,pos[0]+1,pos[1]) && (map_ia[GRID(pos[1],pos[0]+1)]%8<6)&& (map_get_cell_type(map,pos[0]+1,pos[1])==CELL_EMPTY)){
        list_to_do= list_add(list_to_do,make_pos_and_direction(pos[0],pos[1],EAST));
        danger_list_has_shrunk=1;
        aux_delete_from_list(&list_danger,&prev,&cursor_danger);
      }
      else if(map_is_inside(map,pos[0]-1,pos[1]) && (map_ia[GRID(pos[1],pos[0]-1)]%8<6)&& (map_get_cell_type(map,pos[0]-1,pos[1])==CELL_EMPTY)){
        list_to_do= list_add(list_to_do,make_pos_and_direction(pos[0],pos[1],WEST));
        danger_list_has_shrunk=1;
        aux_delete_from_list(&list_danger,&prev,&cursor_danger);
      }
      else if(map_is_inside(map,pos[0],pos[1]-1) && (map_ia[GRID(pos[1]-1,pos[0])]%8<6)&& (map_get_cell_type(map,pos[0],pos[1]-1)==CELL_EMPTY)){
        list_to_do= list_add(list_to_do,make_pos_and_direction(pos[0],pos[1],NORTH));
        danger_list_has_shrunk=1;
        aux_delete_from_list(&list_danger,&prev,&cursor_danger);
      }
      else if(map_is_inside(map,pos[0],pos[1]+1) && (map_ia[GRID(pos[1]+1,pos[0])]%8<6)&& (map_get_cell_type(map,pos[0],pos[1]+1)==CELL_EMPTY)){
        list_to_do= list_add(list_to_do,make_pos_and_direction(pos[0],pos[1],SOUTH));
        danger_list_has_shrunk=1;
        aux_delete_from_list(&list_danger,&prev,&cursor_danger);
      }
      else{
        prev=cursor_danger;
        cursor_danger=cursor_danger->next;
      }
    }
    while (list_to_do!=NULL)
    {
      pos=(uint8_t*) list_to_do->element;
      if ((map_ia[GRID(pos[1],pos[0])]%8==6)){
        map_ia[GRID(pos[1],pos[0])]-=6-pos[2];
      }
      list_next=list_to_do->next;
      free(list_to_do->element);
      free(list_to_do);
      list_to_do=list_next;
    }
    
    
  }
}




struct Mob* mob_init(uint8_t status, uint8_t is_in_box, uint8_t range , uint8_t lifes, uint16_t moveCooldownInitial, enum direction direction){
  struct Mob* mob = (struct Mob*) (malloc(sizeof(struct Mob)));
  mob->status = status;
  mob->is_in_box =is_in_box;
  mob->range = range;
  mob->lifes = lifes;
  mob->moveCooldownInitial = moveCooldownInitial;
  mob->moveCooldown = 0;
  mob->direction = direction;
	return(mob);
}

void mob_get_infos(struct Mob* mob,	uint16_t* status,	uint16_t* is_in_box,	uint16_t* range,	uint16_t* lifes,uint16_t* moveCooldown,	uint16_t* moveCooldownInitial,	uint16_t* direction){
  *status=mob->status;
  *is_in_box=mob->is_in_box;
  *range=mob->range;
  *lifes=mob->lifes;
  *moveCooldown=mob->moveCooldown;
  *moveCooldownInitial=mob->moveCooldownInitial;
  *direction=mob->direction;
}
void mob_set_xy(struct Mob* mob,uint8_t x,uint8_t y){
  mob->x=x;
  mob->y=y;
  mob->is_in_box=0;
}
void mob_set_moveCooldown(struct Mob* mob,uint16_t moveCooldown){
  mob->moveCooldown=moveCooldown;
}

enum direction mob_get_direction(struct Mob* mob){
  return(mob->direction);
}


static void direction_to_pos(uint8_t x , uint8_t y,enum direction direction ,uint8_t* newx ,uint8_t* newy){
  switch (direction) {
    case SOUTH:
      *newx = x;
      *newy = y+1;
    break;
    case WEST:
      *newx = x-1;
      *newy = y;
    break;
    case NORTH:
      *newx = x;
      *newy = y-1;
    break;
    case EAST:
      *newx = x+1;
      *newy = y;
    break;
    default:
    break;
  }
}

uint8_t mob_move(struct Mob* mob, struct map* map, uint8_t x , uint8_t y){
  uint8_t newx,newy;
  direction_to_pos(x,y,mob->direction,&newx,&newy);
  if (map_is_inside(map,newx,newy) && (map_ia[GRID(newy,newx)]%8!=7) && map_get_cell_type(map,newx,newy)==CELL_EMPTY && !( (mob->status) && (map_ia[GRID(newy,newx)]>7) && (map_ia[GRID(y,x)]<7) )){
    map_move_cell(map,x,y,newx,newy);
    mob->x=newx;
    mob->y=newy;
    mob->moveCooldown=mob->moveCooldownInitial;
    return(1);
  }
  else{
    return(0);
  }

}

uint8_t mob_update(struct Mob* mob, struct map* map, struct player* player,uint16_t elapsedTime){
  uint8_t x=mob->x;
  uint8_t y=mob->y;
  uint8_t xplayer,yplayer;
  uint8_t dir;
  uint16_t distance;
  xplayer=player_get_x(player);
  yplayer=player_get_y(player);
  if (mob->is_in_box){
      if (mob->moveCooldown>0){ //this means that the mob is stunned, not realy in a box : we are using uncompatibility between is_in_box and move cooldown
        if (mob->moveCooldown<elapsedTime){
          mob->is_in_box=0;
          mob->moveCooldown=0;
        }
        else{
          mob->moveCooldown=mob->moveCooldown-elapsedTime;
        }
      }
      return(0);
    }
  distance=(xplayer-x)*(xplayer-x)+(yplayer-y)*(yplayer-y);
  if (distance==0){
    if (player_damage(player)) {
      vfx_set_add_player_vfx(player,EFFECT_PLAYER_BITE,200);
      audio_add_sound(AUDIO_MOB_BITE);
    }
  }
  if (mob->moveCooldown<elapsedTime){

    if (distance <= mob->range*mob->range){
      dir=map_ia[GRID(y,x)];
      if (dir>7){
        if (mob->status & 0x01){
          dir=dir%8;
        }
        else{
          dir= dir/8-1;
        }
      }
      switch (dir)
      {
      case 4://no break
      case 7:
        mob->direction=rand()%4;
        break;
      case 6://no break
        mob->direction=SOUTH;
      case 5:
        return(0);
        break;
      default:
        mob->direction=dir;
        break;
      }
    }
    else {
      if ((mob->status & 0x02) && (map_ia[GRID(y,x)]>7)){
        dir=map_ia[GRID(y,x)]%8;
        switch (dir)
        {
        case 4://no break
        case 7:
          mob->direction=rand()%4;
          break;
        case 6://no break
          mob->direction=SOUTH;
        case 5:
          return(0);
          break;
        default:
          mob->direction=dir;
          break;
        }
      }
      mob->direction=rand()%4;

    }

    return (mob_move(mob, map,  x , y));
  }
  else{
    mob->moveCooldown=mob->moveCooldown - elapsedTime;
    return(0);
  }
}
void mob_get_visual_pos(struct Mob* mob, uint16_t* xv,uint16_t* yv){
	uint16_t portion_of_block_traveled= (mob->moveCooldown*SIZE_BLOC)/(mob->moveCooldownInitial);
  uint16_t jump_size=SIZE_BLOC/2- abs(SIZE_BLOC/2-portion_of_block_traveled);
	switch (mob->direction)
	{
		case NORTH:
			*xv = mob->x*SIZE_BLOC+jump_size/4;
			*yv= mob->y*SIZE_BLOC+portion_of_block_traveled;
			break;
		case SOUTH:
			*xv=mob->x*SIZE_BLOC+jump_size/4;
			*yv=mob->y*SIZE_BLOC-portion_of_block_traveled;
			break;
		case EAST:
			*xv= mob->x*SIZE_BLOC-portion_of_block_traveled;
			*yv= mob->y*SIZE_BLOC+jump_size/2;
			break;
		case WEST:
			*xv= mob->x*SIZE_BLOC+portion_of_block_traveled;
			*yv= mob->y*SIZE_BLOC+jump_size/2;
			break;
		default:
			break;
	}
}
void mob_display(struct Mob* mob ){
  uint16_t xv,yv;
  if (mob->is_in_box){
      if (mob->moveCooldown>0){ //this means that the mob is stunned, not realy in a box : we are using uncompatibility between is_in_box and move cooldown
        window_display_image(sprite_get_mob(mob->direction),mob->x*SIZE_BLOC,mob->y*SIZE_BLOC);
      }
      return;
    }
  mob_get_visual_pos(mob,&xv,&yv);
  window_display_image(sprite_get_mob(mob->direction),xv,yv);
}


void mob_damage(struct map* map,struct Mob* mob){
  audio_add_sound(AUDIO_MOB_HURT);
	if (--(mob->lifes)){
		mob->is_in_box=1;
		mob->moveCooldown=3*mob->moveCooldownInitial;
	}
	else{
		mob->moveCooldown=0;
		mob->is_in_box=1;
		map_set_cell_type(map,mob->x,mob->y,CELL_EMPTY);
	}
}
