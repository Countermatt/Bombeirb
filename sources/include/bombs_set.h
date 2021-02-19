#ifndef BOMBS_SET_H_
#define BOMBS_SET_H_

#include <map.h>
#include <constant.h>
#include <game.h>
#include <player.h>
#include <bomb.h>

void bombs_set_create_bomb(int x, int y, int level,int range);

void bombs_set_update(struct game* game);

//If there is a bomb at given pos in given level make it explode and return 0 else it returns 1
uint8_t bombs_set_at_pos_explode(int x, int y, uint8_t level,struct player* player,struct map* map,uint8_t is_current_level);

void bombs_set_free();

struct List* get_list_bomb();

void bombs_set_load_bomb(int x, int y, int levelandrange, int timer);


#endif /* BOMBS_SET_H_ */
