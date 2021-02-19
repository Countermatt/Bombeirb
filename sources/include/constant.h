/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2018-2019"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30
#define DEFAULT_BOMB_TIMER 4
// map size
#define STATIC_MAP_WIDTH  12
#define STATIC_MAP_HEIGHT 12

// number of frame for animations
#define NB_FRAME_BOMB 4


//player movement cooldown in ms
#define PLAYER_MOVE_COOLDOWN 100
#define PLAYER_RECEIVE_DAMAGE_COOLDOWN 1000
enum direction {
	NORTH = 0,
	EAST,  // 1
	SOUTH, // 2
	WEST   // 3
};

//define symbole for print f
//#define PRIu8 "hu"
//#define PRIu16 "hu"


#endif /* CONSTANT */
