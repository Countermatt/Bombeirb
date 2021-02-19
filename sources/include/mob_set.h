#ifndef MOB_SET_H_
#define MOB_SET_H_

#include <map.h>
#include <player.h>
#include <constant.h>
#include <stdlib.h>
#include <mob.h>
#include <game.h>

void mob_set_init(struct game* game);
void mob_set_free(uint8_t nb_level);
struct Mob* mob_set_get_mob(uint8_t level,uint8_t id);
void mob_set_update(struct map* map,struct player* player,uint8_t level,uint16_t elapsedTime);
void mob_set_display(uint8_t level);
void mob_set_damage(struct map* map,uint8_t level,uint8_t id);
void mob_sync_pos_w_map(struct game* game);
//fetch the number of monster in the given level
uint8_t mob_set_mob_in_level(uint8_t level);
//loads mob_set from save  !!! warnings must be loaded after the maps (they are used to sync mob's positions)
void mob_set_load(struct game* game);






#endif /* MOB_SET_H_ */
