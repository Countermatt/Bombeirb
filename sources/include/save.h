#ifndef SAVE_H_
#define SAVE_H_
#include <player.h>
#include <game.h>

void save_player(struct player* player);
char* save_map_get_file(int level);
void save_map(struct map* map, int level);
void save_game(struct game* game);
void save_bomb(struct game* game);
uint8_t mob_set_mob_in_level(uint8_t level);
void save_mobs();
#endif /*SAVE_h_*/
