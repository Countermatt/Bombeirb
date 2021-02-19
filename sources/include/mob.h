#ifndef MOB_H_
#define MOB_H_

#include <map.h>
#include <player.h>
#include <constant.h>
#include <stdlib.h>
#include <mob.h>

struct Mob;
//initialize a mob with given parameters and return a pointer to it
struct Mob* mob_init(uint8_t status, uint8_t is_in_box, uint8_t range , uint8_t lifes, uint16_t moveCooldownInitial, enum direction direction);
//make a mob update (basicly decides where to move and move)
uint8_t mob_update(struct Mob* mob, struct map* map, struct player* player,uint16_t elapsedTime);
//set a mob's position in order to insure synchronisation with the map, and set mob status to not in box
void mob_set_xy(struct Mob* mob,uint8_t x,uint8_t y);
//display mob
void mob_display(struct Mob* mob );
//generates the map (djikstra) that gives instructions to reach the point x,y in the map : 4 means that there is no way to reach the point
void mob_update_map_ia(struct map* map,uint8_t bomb_range ,uint8_t x, uint8_t y);
//init map with right size
void mob_init_map_ia(uint8_t h,uint8_t w);
//free the map
void mob_free_map_ia();
//applies damage to given monster, and if it reach 0 hp procceed to delete it
void mob_damage(struct map* map,struct Mob* mob);
uint16_t mob_get_moveCooldownInitial(struct Mob* mob);
uint8_t mob_get_x(struct Mob* mob);
uint8_t mob_get_y(struct Mob* mob);
//fetch infos used for saving
void mob_get_infos(struct Mob* mob,	uint16_t* status,	uint16_t* is_in_box,	uint16_t* range,	uint16_t* lifes,uint16_t* moveCooldown,	uint16_t* moveCooldownInitial,	uint16_t* direction);
void mob_set_moveCooldown(struct Mob* mob,uint16_t moveCooldown);

#endif /* MOB_H_ */
