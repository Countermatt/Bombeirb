#ifndef WORLD_H_
#define WORLD_H_

#include <game.h>

//verify if  door open
int is_door_open(int door);

//verify where door lead
int door_lead(int door);

//switch world
void world_switch(struct game* game, int door);

#endif /* WORLD_H_ */
