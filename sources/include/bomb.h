#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <game.h>
#include <player.h>



struct Bomb;
int bomb_dec_timer(struct Bomb* bomb,struct map* map);
struct Bomb* bomb_init(uint8_t x, uint8_t y, uint8_t level, uint8_t range);
uint8_t bomb_update(struct Bomb* bomb,struct map* map,uint16_t elapsed_time,uint8_t is_current_level);
void bomb_explode(struct Bomb* bomb, struct map* map, struct player* player,uint8_t is_current_map);

uint8_t bomb_get_level(struct Bomb* bomb);
uint8_t bomb_get_range(struct Bomb* bomb);
int bomb_get_x(struct Bomb* bomb);
int bomb_get_y(struct Bomb* bomb);
uint8_t bomb_get_levelAndRange(struct Bomb* bomb);
uint16_t bomb_get_timer(struct Bomb* bomb);
struct Bomb* bomb_make(int x, int y,int  levelandrange,int timer);



#endif /* BOMB_H_ */
