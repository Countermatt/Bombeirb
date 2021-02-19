/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <bombs_set.h>
#include <vfx_set.h>
#include <audio.h>
#include <sprite.h>
#include <map.h>
#include <mob_set.h>
#include <mob.h>
#include <menu.h>

//initialise random genrator
static void init_rand(){
	time_t t;
	srand((unsigned) time(&t));
}

int main(int argc, char *argv[]) {
	init_rand();
	audio_init();
	

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

		
	window_create(600, 450);
	SDL_EnableKeyRepeat(0, 0);
	 //to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	// game loop
	// static time rate implementation
	int done = 0;
	int menu_status = 1;
	int load_status = 0;
	while (!done){
		struct game* game = game_new();
		struct menu* menu = menu_init();
		while(menu_status){
			SDL_Delay(ideal_speed);
			done=menu_update(menu, game);
			menu_status = menu_get_status(menu);

		}
		load_status = menu_get_load(menu);
		if(!(done)){
			if(load_status){
				game_load(game);
				load_status = 0;
			}
			mob_set_init(game);
			mob_init_map_ia(map_get_height(game_get_current_map(game)),map_get_width(game_get_current_map(game)));
			mob_update_map_ia(game_get_current_map(game),player_get_bomb_range(game_get_player(game)),player_get_x(game_get_player(game)),player_get_y(game_get_player(game)));
		
			window_create(SIZE_BLOC * map_get_width(game_get_current_map(game)),SIZE_BLOC * map_get_height(game_get_current_map(game)) + BANNER_HEIGHT + LINE_HEIGHT);
																																										
			map_foreground_load(game_get_current_map(game));
			while (!done) {
				timer = SDL_GetTicks();
				game_display(game);
				execution_speed = SDL_GetTicks() - timer;
				done = game_update(game);
				if (execution_speed < ideal_speed){
					SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time.let's wait.
				}
			}
			mob_set_free(game_get_nb_of_level(game));
			mob_free_map_ia();
			bombs_set_free();
			vfx_set_free();
			vfx_set_foregrounds_free();
			done=0;
			menu_status=1;

		}
		game_free(game);
		menu_free(menu);
	}
	audio_close();
	audio_free();
	
    sprite_free();
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
