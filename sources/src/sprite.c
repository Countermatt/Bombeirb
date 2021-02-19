/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE                "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	        "sprite/door_opened.png"
#define MAP_DOOR_CLOSED	        "sprite/door_closed.png"

// Scenery elements
#define MAP_CHEST 				"sprite/chest.png"			

#define	MAP_PILLAR 		        "sprite/walls/pillar.png"
#define	MAP_PILLAR_OLD  		"sprite/walls/pillar_broken.png"
#define	MAP_PILLAR_BROKEN  		"sprite/walls/pillar_old.png"

#define	MAP_WALL_SOUTH_WEST  	        "sprite/walls/wall_angle_south_west.png"	
#define	MAP_WALL_SOUTH_EAST 		"sprite/walls/wall_angle_south_east.png"
#define	MAP_WALL_SOUTH_DOUBLE		"sprite/walls/wall_angle_double.png"
#define	MAP_WALL_NORTH_EAST  		"sprite/walls/wall_angle_north_east.png"
#define	MAP_WALL_NORTH_WEST  		"sprite/walls/wall_angle_north_west.png"
#define	MAP_WALL_VERTICAL 		"sprite/walls/wall_vertical.png"
#define	MAP_WALL_VERTICAL_START         "sprite/walls/wall_vertical_start.png"
#define	MAP_WALL_HORIZONTAL		"sprite/walls/wall_horizontal.png"
#define	MAP_WALL_HORIZONTAL_PILLAR 	"sprite/walls/wall_horizontal_pillar.png"	
#define	MAP_WALL_HORIZONTAL_CRACKED     "sprite/walls/wall_horizontal_cracked.png"		
#define	MAP_WALL_HORIZONTAL_BREACH 	"sprite/walls/wall_horizontal_breach.png"	

//foreground
#define	TOP_PILLAR 		        "sprite/walls_top/top_pillar.png"
#define	TOP_PILLAR_OLD  		"sprite/walls_top/top_pillar_broken.png"
#define	TOP_PILLAR_BROKEN  		"sprite/walls_top/top_pillar_old.png"

#define	TOP_WALL_SOUTH_WEST  	        "sprite/walls_top/top_angle_south_west.png"	
#define	TOP_WALL_SOUTH_EAST 		"sprite/walls_top/top_angle_south_east.png"
#define	TOP_WALL_SOUTH_DOUBLE		"sprite/walls_top/top_angle_double.png"
#define	TOP_WALL_NORTH_EAST  		"sprite/walls_top/top_angle_north_east.png"
#define	TOP_WALL_NORTH_WEST  		"sprite/walls_top/top_angle_north_west.png"
#define	TOP_WALL_VERTICAL 		"sprite/walls_top/top_vertical.png"
#define	TOP_WALL_VERTICAL_START         "sprite/walls_top/top_vertical_start.png"
#define	TOP_WALL_HORIZONTAL		"sprite/walls_top/top_horizontal.png"
#define	TOP_WALL_HORIZONTAL_PILLAR 	"sprite/walls_top/top_horizontal_pillar.png"	
#define	TOP_WALL_HORIZONTAL_CRACKED     "sprite/walls_top/top_horizontal_cracked.png"		
#define	TOP_WALL_HORIZONTAL_BREACH 	"sprite/walls_top/top_horizontal_breach.png"	

#define FLOOR_0  	"sprite/floors/floor0.png"
#define FLOOR_1  	"sprite/floors/floor1.png"
#define FLOOR_2  	"sprite/floors/floor2.png"
#define FLOOR_3  	"sprite/floors/floor3.png"
#define FLOOR_4  	"sprite/floors/floor4.png"
#define FLOOR_5  	"sprite/floors/floor5.png"
#define FLOOR_6  	"sprite/floors/floor6.png"
#define FLOOR_7  	"sprite/floors/floor7.png"
#define FLOOR_8  	"sprite/floors/floor8.png"
#define FLOOR_9 	"sprite/floors/floor9.png"
#define FLOOR_10 	"sprite/floors/floor10.png"
#define FLOOR_11 	"sprite/floors/floor11.png"
#define FLOOR_12 	"sprite/floors/floor12.png"
#define FLOOR_13 	"sprite/floors/floor13.png"
#define FLOOR_14 	"sprite/floors/floor14.png"
#define FLOOR_15 	"sprite/floors/floor15.png"
#define FLOOR_16 	"sprite/floors/floor16.png"
#define FLOOR_17 	"sprite/floors/floor17.png"
#define FLOOR_18 	"sprite/floors/floor18.png"
#define FLOOR_19 	"sprite/floors/floor19.png"

//bites
#define BITES0 "sprite/bites/bites0.png"
#define BITES1 "sprite/bites/bites1.png"
#define BITES2 "sprite/bites/bites2.png"
#define BITES3 "sprite/bites/bites3.png"
#define BITES4 "sprite/bites/bites4.png"
#define BITES5 "sprite/bites/bites5.png"
#define BITES6 "sprite/bites/bites6.png"
#define BITES7 "sprite/bites/bites7.png"
#define BITES8 "sprite/bites/bites8.png"

//blasts
#define BLAST0 "sprite/blasts/blast0.png"
#define BLAST1 "sprite/blasts/blast1.png"
#define BLAST2 "sprite/blasts/blast2.png"
#define BLAST3 "sprite/blasts/blast3.png"
#define BLAST4 "sprite/blasts/blast4.png"
#define BLAST5 "sprite/blasts/blast5.png"
#define BLAST6 "sprite/blasts/blast6.png"
#define BLAST7 "sprite/blasts/blast7.png"
#define BLAST8 "sprite/blasts/blast8.png"
#define BLAST9 "sprite/blasts/blast9.png"

//keys for animations
#define KEY0 "sprite/keys/key0.png"
#define KEY1 "sprite/keys/key1.png"
#define KEY2 "sprite/keys/key2.png"
#define KEY3 "sprite/keys/key3.png"

//smoke effect
#define SMOKE0 "sprite/smoke/smoke0.png"
#define SMOKE1 "sprite/smoke/smoke1.png"
#define SMOKE2 "sprite/smoke/smoke2.png"
#define SMOKE3 "sprite/smoke/smoke3.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE		"sprite/banner_range.png"
#define BANNER_LEVEL		"sprite/level.png"
#define DIGIT_0			"sprite/banner_0.jpg"
#define DIGIT_1			"sprite/banner_1.jpg"
#define DIGIT_2			"sprite/banner_2.jpg"
#define DIGIT_3			"sprite/banner_3.jpg"
#define DIGIT_4			"sprite/banner_4.jpg"
#define DIGIT_5			"sprite/banner_5.jpg"
#define DIGIT_6			"sprite/banner_6.jpg"
#define DIGIT_7			"sprite/banner_7.jpg"
#define DIGIT_8			"sprite/banner_8.jpg"
#define DIGIT_9			"sprite/banner_9.jpg"

//sprites of torches
#define TORCH_0			"sprite/torches/torch0.png"
#define TORCH_1			"sprite/torches/torch1.png"
#define TORCH_2			"sprite/torches/torch2.png"
#define TORCH_3			"sprite/torches/torch3.png"
#define TORCH_4			"sprite/torches/torch4.png"
#define TORCH_5			"sprite/torches/torch5.png"
#define TORCH_6			"sprite/torches/torch6.png"
#define TORCH_7			"sprite/torches/torch7.png"
#define TORCH_8			"sprite/torches/torch8.png"
#define TORCH_9 		"sprite/torches/torch9.png"


// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

//Sprites of Effects
#define EXPLOSION_1		"sprite/explosion1.png"
#define EXPLOSION_2		"sprite/explosion2.png"
#define EXPLOSION_3		"sprite/explosion3.png"
#define EXPLOSION_4		"sprite/explosion4.png"
#define SHIELD                  "sprite/shield.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

// Sprites of Players walking
#define PLAYER_WALKING_WEST_RIGHT     	"sprite/walking_west_right.png"
#define PLAYER_WALKING_WEST_LEFT      	"sprite/walking_west_left.png"
#define PLAYER_WALKING_EAST_RIGHT     	"sprite/walking_east_right.png"
#define PLAYER_WALKING_EAST_LEFT      	"sprite/walking_east_left.png"
#define PLAYER_WALKING_NORTH_RIGHT    	"sprite/walking_north_right.png"
#define PLAYER_WALKING_NORTH_LEFT     	"sprite/walking_north_left.png"
#define PLAYER_WALKING_SOUTH_RIGHT    	"sprite/walking_south_right.png"
#define PLAYER_WALKING_SOUTH_LEFT     	"sprite/walking_south_left.png"

//Sprites of mobs
#define MOB_DOWN     "sprite/monster_down.png"
#define MOB_LEFT     "sprite/monster_left.png"
#define MOB_UP     "sprite/monster_up.png"
#define MOB_RIGHT     "sprite/monster_right.png"



//Sprites of mobs
#define MOB_DOWN     "sprite/monster_down.png"
#define MOB_LEFT     "sprite/monster_left.png"
#define MOB_UP     "sprite/monster_up.png"
#define MOB_RIGHT     "sprite/monster_right.png"

//Sprites of menu
#define MENU_NEW	"sprite/menu/menu_nouvellepartie.png"
#define MENU_LOAD	"sprite/menu/menu_charger.png"
#define MENU_OPTION	"sprite/menu/menu_option.png"
#define MENU_QUIT	"sprite/menu/menu_quitter.png"
SDL_Surface* menu[4];

#define GAME_OVER	"sprite/menu/game_over.png"
#define WIN	"sprite/menu/win.png"
//pause
#define PAUSE_LOGO 	"sprite/pause.png"
SDL_Surface* pause;

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;
SDL_Surface* banner_level;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* walls[14];
SDL_Surface* chest;
SDL_Surface* bombs[4];

SDL_Surface* torches[10];

SDL_Surface* floors[20];
//foreground
SDL_Surface* walls_foreground[14];

//effects
SDL_Surface* explosion[4];
SDL_Surface* shield;

SDL_Surface* bites[9];
SDL_Surface* blasts[10];
SDL_Surface* keys[4];
SDL_Surface* smokes[4];

//win and lose screens
SDL_Surface* game_over_screen;
SDL_Surface* win_screen;

// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS + 1];

// player
SDL_Surface* player_img[4];
SDL_Surface* player_walking[8];

//mob
SDL_Surface* mob_img[4];


static void banner_load() {
	// numbers imgs
	numbers[0] = image_load(DIGIT_0);
	numbers[1] = image_load(DIGIT_1);
	numbers[2] = image_load(DIGIT_2);
	numbers[3] = image_load(DIGIT_3);
	numbers[4] = image_load(DIGIT_4);
	numbers[5] = image_load(DIGIT_5);
	numbers[6] = image_load(DIGIT_6);
	numbers[7] = image_load(DIGIT_7);
	numbers[8] = image_load(DIGIT_8);
	numbers[9] = image_load(DIGIT_9);

	// other banner sprites
	banner_life = image_load(BANNER_LIFE);
	banner_bomb = image_load(BANNER_BOMB);
	banner_range = image_load(BANNER_RANGE);
	banner_line = image_load(BANNER_LINE);
	banner_level=image_load(BANNER_LEVEL);
}

static void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_level);
}

static void map_load() {
	// Sprite loading
	chest = image_load(MAP_CHEST);
	walls[0] = image_load(MAP_PILLAR);
	walls[1] = image_load(MAP_PILLAR_OLD  );
	walls[2] = image_load(MAP_PILLAR_BROKEN  );
	walls[3] = image_load(MAP_WALL_SOUTH_WEST  );
	walls[4] = image_load(MAP_WALL_SOUTH_EAST );
	walls[5] = image_load(MAP_WALL_SOUTH_DOUBLE);
	walls[6] = image_load(MAP_WALL_NORTH_EAST  );
	walls[7] = image_load(MAP_WALL_NORTH_WEST  );
	walls[8] = image_load(MAP_WALL_VERTICAL );
	walls[9] = image_load(MAP_WALL_VERTICAL_START );
	walls[10] = image_load(MAP_WALL_HORIZONTAL);
	walls[11] = image_load(MAP_WALL_HORIZONTAL_PILLAR );
	walls[12] = image_load(MAP_WALL_HORIZONTAL_CRACKED); 
	walls[13] = image_load(MAP_WALL_HORIZONTAL_BREACH );

	torches[0] = image_load(TORCH_0);	
	torches[1] = image_load(TORCH_1);	
	torches[2] = image_load(TORCH_2);	
	torches[3] = image_load(TORCH_3);	
	torches[4] = image_load(TORCH_4);	
	torches[5] = image_load(TORCH_5);	
	torches[6] = image_load(TORCH_6);	
	torches[7] = image_load(TORCH_7);	
	torches[8] = image_load(TORCH_8);	
	torches[9] = image_load(TORCH_9); 

	box = image_load(MAP_CASE);
	key = image_load(MAP_KEY);


	door_opened = image_load(MAP_DOOR_OPENED);
	door_closed = image_load(MAP_DOOR_CLOSED);
	bombs[0] = image_load(BOMB_TTL4);
	bombs[1] =  image_load(BOMB_TTL3);
	bombs[2] = image_load(BOMB_TTL2);
	bombs[3] = image_load(BOMB_TTL1);
}


static void map_unload() {
	SDL_FreeSurface(chest);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
	for (int k = 0; k < 4; k++){
		SDL_FreeSurface(bombs[k]);
	}
	for (int k = 0; k < 14; k++){
		SDL_FreeSurface(walls[k]);
	}
	for (int k = 0; k < 10; k++){
		SDL_FreeSurface(torches[k]);
	}

}
static void foreground_load(){
	walls_foreground[0] = image_load(TOP_PILLAR);
	walls_foreground[1] = image_load(TOP_PILLAR_OLD  );
	walls_foreground[2] = image_load(TOP_PILLAR_BROKEN  );
	walls_foreground[3] = image_load(TOP_WALL_SOUTH_WEST  );
	walls_foreground[4] = image_load(TOP_WALL_SOUTH_EAST );
	walls_foreground[5] = image_load(TOP_WALL_SOUTH_DOUBLE);
	walls_foreground[6] = image_load(TOP_WALL_NORTH_EAST  );
	walls_foreground[7] = image_load(TOP_WALL_NORTH_WEST  );
	walls_foreground[8] = image_load(TOP_WALL_VERTICAL );
	walls_foreground[9] = image_load(TOP_WALL_VERTICAL_START );
	walls_foreground[10] = image_load(TOP_WALL_HORIZONTAL);
	walls_foreground[11] = image_load(TOP_WALL_HORIZONTAL_PILLAR );
	walls_foreground[12] = image_load(TOP_WALL_HORIZONTAL_CRACKED); 
	walls_foreground[13] = image_load(TOP_WALL_HORIZONTAL_BREACH );
}
static void foreground_unload() {
	for (int k = 0; k < 14; k++){
			SDL_FreeSurface(walls_foreground[k]);
	}

}
static void floor_load(){
	floors[0]=image_load(FLOOR_0);
	floors[1]=image_load(FLOOR_1);
	floors[2]=image_load(FLOOR_2);
	floors[3]=image_load(FLOOR_3);
	floors[4]=image_load(FLOOR_4);
	floors[5]=image_load(FLOOR_5);
	floors[6]=image_load(FLOOR_6);
	floors[7]=image_load(FLOOR_7);
	floors[8]=image_load(FLOOR_8);
	floors[9]=image_load(FLOOR_9);
	floors[10]=image_load(FLOOR_10);
	floors[11]=image_load(FLOOR_11);
	floors[12]=image_load(FLOOR_12);
	floors[13]=image_load(FLOOR_13);
	floors[14]=image_load(FLOOR_14);
	floors[15]=image_load(FLOOR_15);
	floors[16]=image_load(FLOOR_16);
	floors[17]=image_load(FLOOR_17);
	floors[18]=image_load(FLOOR_18);
	floors[19]=image_load(FLOOR_19);


}
static void floor_unload(){
	for (int k = 0; k < 20; k++){
		SDL_FreeSurface(floors[k]);
	}
}
static void bonus_load(){
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = image_load(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = image_load(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = image_load(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = image_load(IMG_BONUS_BOMB_NB_DEC);
}

static void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

static void player_load() {
	player_img[WEST] = image_load(PLAYER_LEFT);
	player_img[EAST] = image_load(PLAYER_RIGHT);
	player_img[NORTH] = image_load(PLAYER_UP);
	player_img[SOUTH] = image_load(PLAYER_DOWN);
}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}
static void mob_load() {
	mob_img[WEST] = image_load(MOB_LEFT);
	mob_img[EAST] = image_load(MOB_RIGHT);
	mob_img[NORTH] = image_load(MOB_UP);
	mob_img[SOUTH] = image_load(MOB_DOWN);
}

static void mob_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(mob_img[i]);
}

static void player_walking_load(){
	player_walking[2*NORTH] = image_load(PLAYER_WALKING_NORTH_RIGHT);
	player_walking[2*NORTH+1] = image_load(PLAYER_WALKING_NORTH_LEFT);
	player_walking[2*EAST] = image_load(PLAYER_WALKING_EAST_RIGHT);
	player_walking[2*EAST+1] = image_load(PLAYER_WALKING_EAST_LEFT);
	player_walking[2*SOUTH] = image_load(PLAYER_WALKING_SOUTH_RIGHT);
	player_walking[2*SOUTH+1] = image_load(PLAYER_WALKING_SOUTH_LEFT);
	player_walking[2*WEST] = image_load(PLAYER_WALKING_WEST_RIGHT);
	player_walking[2*WEST+1] = image_load(PLAYER_WALKING_WEST_LEFT);
}
static void player_walking_unload(){
	for (int k = 0; k < 8; k++){
		SDL_FreeSurface(player_walking[k]);
	}
}

static void effect_load(){
	explosion[0] = image_load(EXPLOSION_1);
	explosion[1] = image_load(EXPLOSION_2);
	explosion[2] = image_load(EXPLOSION_3);
	explosion[3] = image_load(EXPLOSION_4);
	shield = image_load(SHIELD);
	player_walking_load();
	bites[0]=image_load(BITES0);
	bites[1]=image_load(BITES1);
	bites[2]=image_load(BITES2);
	bites[3]=image_load(BITES3);
	bites[4]=image_load(BITES4);
	bites[5]=image_load(BITES5);
	bites[6]=image_load(BITES6);
	bites[7]=image_load(BITES7);
	bites[8]=image_load(BITES8);	

	blasts[0]=image_load(BLAST0);
	blasts[1]=image_load(BLAST1);
	blasts[2]=image_load(BLAST2);
	blasts[3]=image_load(BLAST3);
	blasts[4]=image_load(BLAST4);
	blasts[5]=image_load(BLAST5);
	blasts[6]=image_load(BLAST6);
	blasts[7]=image_load(BLAST7);
	blasts[8]=image_load(BLAST8);
	blasts[9]=image_load(BLAST9);

	keys[0]=image_load(KEY0);
	keys[1]=image_load(KEY1);
	keys[2]=image_load(KEY2);
	keys[3]=image_load(KEY3);

	smokes[0]=image_load(SMOKE0);
	smokes[1]=image_load(SMOKE1);
	smokes[2]=image_load(SMOKE2);
	smokes[3]=image_load(SMOKE3);

}
static void effect_unload(){
	for (int k = 0; k < 4; k++){
		SDL_FreeSurface(explosion[k]);
		SDL_FreeSurface(smokes[k]);
		SDL_FreeSurface(keys[k]);
	}
	for (int k = 0; k < 9; k++){
		SDL_FreeSurface(bites[k]);
	}
	for (int k = 0; k < 10; k++){
		SDL_FreeSurface(blasts[k]);
	}
	SDL_FreeSurface(shield);
	player_walking_unload();
}

static void pause_load(){
	pause = image_load(PAUSE_LOGO);
	}

static void pause_unload(){
	SDL_FreeSurface(pause);
	}

static void menu_load(){
    menu[0] = image_load(MENU_NEW);
    menu[1] = image_load(MENU_LOAD);
 	menu[2] = image_load(MENU_OPTION);
	menu[3] = image_load(MENU_QUIT);

	game_over_screen=image_load(GAME_OVER);
	win_screen=image_load(WIN);

        }

static void menu_unload(){
    SDL_FreeSurface(menu[0]);
	SDL_FreeSurface(menu[1]);
	SDL_FreeSurface(menu[2]);
	SDL_FreeSurface(menu[3]);

	SDL_FreeSurface(game_over_screen);
	SDL_FreeSurface(win_screen);
    }


void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	effect_load();
	pause_load();
	foreground_load();
	floor_load();
	mob_load();
	menu_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	effect_unload();
	pause_unload();
	foreground_unload();
	floor_unload();
	mob_unload();
	menu_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 9);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}
SDL_Surface* sprite_get_mob(enum direction direction) {
	assert(mob_img[direction]);
	return mob_img[direction];
}
SDL_Surface* sprite_get_explosion(uint8_t frame) {
	assert(frame >= 0 && frame < 4 );
	return explosion[frame];
}
SDL_Surface* sprite_get_shield() {
	assert(shield );
	return shield;
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_banner_level() {
	assert(banner_level);
	return banner_level;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}


SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}
SDL_Surface* sprite_get_bomb(short tilt) {
//	assert(tilt >= 0 && tilt < 4);
	assert(bombs);
	return bombs[tilt];
}
SDL_Surface* sprite_get_torch(uint8_t id) {
	assert(id >= 0 && id < 10);
	assert(torches);
	return torches[id];
}

SDL_Surface* sprite_get_floor(uint8_t id) {
	assert(id >= 0 && id < 20);
	assert(floors);
	return floors[id];
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}
SDL_Surface* sprite_get_keys(uint8_t id) {
	assert(id < 4);
	assert(keys);
	return keys[id];
}
SDL_Surface* sprite_get_blasts(uint8_t id) {
	assert(id < 10);
	assert(blasts);
	return blasts[id];
}
SDL_Surface* sprite_get_bites(uint8_t id) {
	assert(id < 9);
	assert(bites);
	return bites[id];
}
SDL_Surface* sprite_get_smokes(uint8_t id) {
	assert(id < 4);
	assert(smokes);
	return smokes[id];
}
SDL_Surface* sprite_get_wall(uint8_t wall_id) {
	assert(wall_id < 14);
	assert(walls);
	return walls[wall_id];
}
SDL_Surface* sprite_get_wall_top(uint8_t wall_id) {
	assert(wall_id < 14);
	assert(walls_foreground);
	return walls_foreground[wall_id];
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}

SDL_Surface* sprite_get_player_walking(enum direction direction, uint8_t variation) {
	assert((variation == 0 || variation == 1) && player_walking[2*direction+variation]);
	return player_walking[2*direction+variation];
}

SDL_Surface* sprite_get_pause(){
	assert(pause);
	return pause;
}

SDL_Surface* sprite_get_menu(int type){
	assert(type < 4);
	assert(menu[type]);
	return menu[type];
}

SDL_Surface* sprite_get_game_over(){
	assert(game_over_screen);
	return game_over_screen;
}


SDL_Surface* sprite_get_win(){
	assert(win_screen);
	return win_screen;
}

SDL_Surface* sprite_get_chest(){
	assert(chest);
	return chest;
}
