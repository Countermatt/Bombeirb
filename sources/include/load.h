#ifndef LOAD_H_
#define LOAD_H_
#include <mob.h>

struct player* load_player(struct player* player);
char* load_map_get_file(int level);
struct map* load_map(struct map* map, int level);
int load_game(int type);
void load_bomb(struct game* game);
void load_mobs_level(struct Mob*** mob_grid,uint8_t level,uint8_t mob_in_level);
void load_mobs_level_from_save(struct Mob*** mob_grid,uint8_t level,uint8_t mob_in_level);
void load_array_mob_number(uint8_t* mob_number);
#endif /* LOAD_H_ */
