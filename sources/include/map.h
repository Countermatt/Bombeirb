/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

#include <stdint.h>

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70,  		// 	0111 0000
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC=2,
	BONUS_BOMB_NB_DEC=3,
	BONUS_BOMB_NB_INC=4,
	BONUS_LIFE=5,
	BONUS_KEY=6,
	BONUS_MONSTER0=7,
	BONUS_MONSTER1=8,
	BONUS_MONSTER2=9,
	BONUS_MONSTER3=10,
	BONUS_MONSTER4=11,
	BONUS_MONSTER5=12,
	BONUS_MONSTER6=13,
	BONUS_MONSTER7=14,
	BONUS_MONSTER8=15
};

enum scenery_type {
	SCENERY_PILLAR = 0,    				//0000
	SCENERY_PILLAR_OLD  = 1,    		//0001
	SCENERY_PILLAR_BROKEN  = 2, 		//0010
	SCENERY_WALL_SOUTH_WEST  = 3, 		//0011
	SCENERY_WALL_SOUTH_EAST  = 4, 		//0100
	SCENERY_WALL_SOUTH_DOUBLE  = 5, 	//0101
	SCENERY_WALL_NORTH_EAST  = 6,   	//0110
	SCENERY_WALL_NORTH_WEST  = 7,   	//0111
	SCENERY_WALL_VERTICAL  = 8,			//1000
	SCENERY_WALL_VERTICAL_START =9,		//1001
	SCENERY_WALL_HORIZONTAL =10,		//1010
	SCENERY_WALL_HORIZONTAL_PILLAR =11, //1011
	SCENERY_WALL_HORIZONTAL_CRACKED =12,//1100
	SCENERY_WALL_HORIZONTAL_BREACH =13, //1101
	SCENERY_WALL_HORIZONTAL_TORCH=14,	//1110
	SCENERY_TREASURE = 15  				//1111
};



enum compose_type {
	CELL_PILLAR = CELL_SCENERY | SCENERY_PILLAR,
	CELL_PILLAR_OLD  = CELL_SCENERY | SCENERY_PILLAR_OLD,
	CELL_PILLAR_BROKEN  = CELL_SCENERY | SCENERY_PILLAR_BROKEN,
	CELL_WALL_SOUTH_WEST  = CELL_SCENERY | SCENERY_WALL_SOUTH_WEST,
	CELL_WALL_SOUTH_EAST  = CELL_SCENERY | SCENERY_WALL_SOUTH_EAST,
	CELL_WALL_SOUTH_DOUBLE  = CELL_SCENERY | SCENERY_WALL_SOUTH_DOUBLE,
	CELL_WALL_NORTH_EAST  = CELL_SCENERY | SCENERY_WALL_NORTH_EAST,
	CELL_WALL_NORTH_WEST  = CELL_SCENERY | SCENERY_WALL_NORTH_WEST,
	CELL_WALL_VERTICAL  = CELL_SCENERY | SCENERY_WALL_VERTICAL,
	CELL_WALL_VERTICAL_START =CELL_SCENERY | SCENERY_WALL_VERTICAL_START,
	CELL_WALL_HORIZONTAL =CELL_SCENERY | SCENERY_WALL_HORIZONTAL,
	CELL_WALL_HORIZONTAL_PILLAR =CELL_SCENERY | SCENERY_WALL_HORIZONTAL_PILLAR,
	CELL_WALL_HORIZONTAL_CRACKED =CELL_SCENERY | SCENERY_WALL_HORIZONTAL_CRACKED,
	CELL_WALL_HORIZONTAL_BREACH =CELL_SCENERY | SCENERY_WALL_HORIZONTAL_BREACH,
	CELL_WALL_HORIZONTAL_TORCH =CELL_SCENERY | SCENERY_WALL_HORIZONTAL_TORCH,
	CELL_TREASURE = CELL_SCENERY | SCENERY_TREASURE,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,
	CELL_BOX_KEY  = CELL_BOX | BONUS_KEY,
	CELL_BOX_MONSTER0=CELL_BOX | BONUS_MONSTER0,
	CELL_BOX_MONSTER1=CELL_BOX | BONUS_MONSTER1,
	CELL_BOX_MONSTER2=CELL_BOX | BONUS_MONSTER2,
	CELL_BOX_MONSTER3=CELL_BOX | BONUS_MONSTER3,
	CELL_BOX_MONSTER4=CELL_BOX | BONUS_MONSTER4,
	CELL_BOX_MONSTER5=CELL_BOX | BONUS_MONSTER5,
	CELL_BOX_MONSTER6=CELL_BOX | BONUS_MONSTER6,
	CELL_BOX_MONSTER7=CELL_BOX | BONUS_MONSTER7,
	CELL_BOX_MONSTER8=CELL_BOX | BONUS_MONSTER8
};

struct map;

// Create a new empty map
struct map* map_new(int level);
void map_free(struct map* map);

//set map width and height
int map_set_dimensions(int level,int k);

//return map file
char *map_get_file(int level);

// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

//get the subtype of a cell
unsigned char map_get_cell_subtype(struct map* map, int x, int y);

//place a bomb at given position
void map_place_bomb(struct map* map,int x, int y,int tilt);

//break box and place associated bonus
void map_break_box(struct map* map,int x, int y,uint8_t level);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static(int level, int type);

// Display the map on the screen
void map_display(struct map* map,unsigned char level,unsigned short time);

//Display door info
int door_info(struct map* map,int x,int y);

//move cell at pos x,y to pos newx,newy and give old pos (x,y) value CELL_EMPTY
void map_move_cell(struct map* map,uint8_t x, uint8_t y ,uint8_t newx , uint8_t newy);

void map_open_door(struct map* map, int x,int y);

void map_foreground_load(struct map* map);

int map_get_cell(struct map* map, int x, int y);
#endif /* MAP_H_ */
