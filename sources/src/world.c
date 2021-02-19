#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <game.h>
#include <world.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <vfx_set.h>
#include <mob.h>

//verify if door is open
int is_door_open(int door){
	return door & 0x01;
}

//verify where door lead
int door_lead(int door){
	int world = door;
	world = world >> 1;
	world = world & 0x07;
	return world;
}

//switch world
void world_switch(struct game* game, int door){
	assert(game);
	int level_actual = game_get_level(game);
	int level = door_lead(door);
	struct map* map=game_get_map(game,level);
	int w = map_get_width(map);
	int h = map_get_height(map);
	int i, j;
	for (i = 0; i < w; i++){
	  for (j = 0; j < h; j++){
	    if (map_get_cell_type(map,i,j)==CELL_DOOR && door_lead(door_info(map,i,j))==level_actual) {
				player_set_position(game_get_player(game),i,j);
			}
		}
	}
	vfx_set_foregrounds_free();
	window_create(SIZE_BLOC * w,SIZE_BLOC * h + BANNER_HEIGHT + LINE_HEIGHT);
	game_map_update(game,level);
	//level switch animation
	window_clear();
	game_banner_display(game);

	j=(w%2==0 ?w/2+1 :w/2+2 );
	window_display_image(sprite_get_banner_level(),(w/2)*SIZE_BLOC,(h/2)*SIZE_BLOC);
	window_display_image(sprite_get_number(level_actual+1),j*SIZE_BLOC,(h/2)*SIZE_BLOC);
	window_refresh();
	game_wait(400);
	//part 2
	game_map_update(game,level);
	window_clear();
	game_banner_display(game);

	window_display_image(sprite_get_banner_level(),(w/2)*SIZE_BLOC,(h/2)*SIZE_BLOC);
	window_display_image(sprite_get_number(level+1),j*SIZE_BLOC,(h/2)*SIZE_BLOC);
	window_refresh();
	game_wait(400);
	//
	map_foreground_load(map);
	mob_init_map_ia(h,w);
	mob_update_map_ia(game_get_current_map(game),player_get_bomb_range(game_get_player(game)),player_get_x(game_get_player(game)),player_get_y(game_get_player(game)));
}
