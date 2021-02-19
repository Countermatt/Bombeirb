#include <map.h>
#include <player.h>
#include <constant.h>
#include <stdlib.h>
#include <mob.h>
#include <mob_set.h>
#include <vfx.h>
#include <vfx_set.h>
#include <load.h>
#include <game.h>

static uint8_t mob_grid_size[8];
static struct Mob*** mob_grid;


void mob_sync_pos_w_map(struct game* game){
  uint8_t k,i,j;
  uint8_t w,h;
  struct map* map;
  for (k=0;k<game_get_nb_of_level(game);k++){
    map=game_get_map(game,k);
    w = map_get_width(map);
    h = map_get_height(map);
    for (i = 0; i < w; i++){
      for (j = 0; j < h; j++){
        if (map_get_cell_type(map,i,j)==CELL_MONSTER ) {
          mob_set_xy(mob_grid[k][map_get_cell_subtype(map,i,j)],i,j);
        }
      }
    }
  }
}
uint8_t mob_set_mob_in_level(uint8_t level){
  return(mob_grid_size[level]);
}

void mob_set_init(struct game* game){
  mob_grid =(struct Mob***) (malloc(8*sizeof(struct Mob**)));
  load_array_mob_number(mob_grid_size);
  uint8_t k;
  for (k=0;k<game_get_nb_of_level(game);k++){
    mob_grid[k]= (struct Mob**) (malloc(mob_grid_size[k]*sizeof(struct Mob*)));
    load_mobs_level(mob_grid,k,mob_grid_size[k]);
  }
  mob_sync_pos_w_map(game);
}
void mob_set_load(struct game* game){
  uint8_t k;
  mob_grid =(struct Mob***) (malloc(8*sizeof(struct Mob**)));
  for (k=0;k<game_get_nb_of_level(game);k++){
    mob_grid[k]= (struct Mob**) (malloc(mob_grid_size[k]*sizeof(struct Mob*)));
    load_mobs_level_from_save(mob_grid,k,mob_grid_size[k]);
  }
  mob_sync_pos_w_map(game);
}
void mob_set_free(uint8_t nb_level){
  int k,i;
  for (k=0;k<nb_level;k++){
    for (i=0;i<mob_grid_size[k];i++){
      free(mob_grid[k][i]);
    }
    free(mob_grid[k]);
  }
  free(mob_grid);
  mob_grid=NULL;
}

void mob_set_update(struct map* map,struct player* player,uint8_t level,uint16_t elapsedTime){
  for (int i=0;i<mob_grid_size[level];i++){
    mob_update(mob_grid[level][i],map,player,elapsedTime);
  }
}
void mob_set_display(uint8_t level){
  for (int i=0;i<mob_grid_size[level];i++){
    mob_display(mob_grid[level][i]);
  }
}


struct Mob* mob_set_get_mob(uint8_t level,uint8_t id){
  if (id<mob_grid_size[level]){
    return(mob_grid[level][id]);
  }
  else {
    return(NULL);
  }
}

void mob_set_damage(struct map* map,uint8_t level,uint8_t id){
  struct Mob* mob =mob_grid[level][id];
  mob_damage(map,mob);
  vfx_set_add_pos_vfx(level,mob_get_x(mob),mob_get_y(mob),EFFECT_SMOKE,2*mob_get_moveCooldownInitial(mob));
}
