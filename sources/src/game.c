/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <stdint.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bombs_set.h>
#include <vfx_set.h>
#include <audio.h>
#include <save.h>
#include <load.h>
#include <mob_set.h>
#include <menu.h>
struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	uint8_t isPaused;
	uint32_t actualUpdate;
	uint32_t previousUpdate;
	uint16_t elapsedTime;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->levels = 8;
	game->level = 0;
	game->actualUpdate = SDL_GetTicks();
	game->previousUpdate=game->actualUpdate;
	game->elapsedTime=0;
	game->isPaused=0;
	game->maps = malloc((game->levels)*sizeof(struct game));
	for (int k = 0; k < game->levels; k++){
		 game->maps[k] = map_get_static(k,0);
	}
	//Set default location of the player
	game->player = player_init(5,3);
	// Set default location of the player
	player_set_position(game->player, 0, 0);
	return game;
}

uint32_t game_get_actual_update(struct game* game){
	return game->actualUpdate;
}

uint32_t game_get_previous_update(struct game* game){
	return game->previousUpdate;
}

short game_is_paused(struct game* game){
	return game->isPaused;
}

void game_free(struct game* game) {
	assert(game);
	if (game->player!=NULL){
		free(game->player);
		game->player=NULL;
	}

	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
	free(game);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 10 * SIZE_BLOC) / 4;
	int x = 0;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_lifes(game_get_player(game))), x, y);

	x =  white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x =  white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 2 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 2 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);

	x = 3 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 3 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_keys(game_get_player(game))), x, y);
	
	x = 4 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_level(), x, y);
	x = 4 * white_bloc + 9* SIZE_BLOC;
	window_display_image(sprite_get_number(game_get_level(game)+1), x, y);

}



void game_display(struct game* game) {
	assert(game);
	if (!(game->isPaused)){
		window_clear();
		game_banner_display(game);
		map_display(game_get_current_map(game),game->level,game->actualUpdate);
		player_display(game->player);
		mob_set_display(game->level);
		vfx_set_display(game_get_level(game),game_get_elapsed_time(game));
		vfx_set_foreground_display();
		window_refresh();
	}
}

static uint8_t game_arrow_pressed=0;
	/*a byte that keeps the information on wich arrow key is pressed */
	/*bit 0 up , bit 1 right, bit 2 down and bit 3 left*/

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);



	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				game_arrow_pressed |= 0x01;
				break;
			case SDLK_DOWN:
				game_arrow_pressed |= 0x04;
				break;
			case SDLK_RIGHT:
				game_arrow_pressed |= 0x02;
				break;
			case SDLK_LEFT:
				game_arrow_pressed |= 0x08;
				break;
			case SDLK_SPACE:
				if (!(game->isPaused)){
					player_place_bomb(game);
				}
				break;
			case SDLK_j:
				audio_music_switch();	
				break;
			case SDLK_p:
				game->isPaused = !(game->isPaused);
				if (game->isPaused){
					window_display_image(sprite_get_pause(),5*40,5*40);
					window_refresh();
				}
				else{
					game->actualUpdate=SDL_GetTicks();
					game->previousUpdate=game->actualUpdate-game->elapsedTime; //we make a fake previous update, so we can keep the same elapsed time in order to insure that the game was paused
					bombs_set_update(game);
				}
				break;
			case SDLK_s:
					save_game(game);	
				break;
			case SDLK_l:
					game_load(game);
				break;
			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				game_arrow_pressed &= 0xFE; /* FF - 01*/
				break;
			case SDLK_DOWN:
				game_arrow_pressed &= 0xFB; /* FF - 04*/
				break;
			case SDLK_RIGHT:
				game_arrow_pressed &= 0xFD; /* FF - 02*/
				break;
			case SDLK_LEFT:
				game_arrow_pressed &= 0xF7; /* FF - 08*/
				break;
			default:
				break;
			}
			break;
		default:
			break;

		}
	}
	int move=0;
	if (!(game->isPaused)){
		if (game_arrow_pressed & 0x01){
			move=player_move(player, map, game,NORTH);
		}
		else if (game_arrow_pressed & 0x02){
			move=player_move(player, map, game,EAST);
		}
		else if (game_arrow_pressed & 0x04){
			move=player_move(player, map, game,SOUTH);
		}
		else if (game_arrow_pressed & 0x08){
			move=player_move(player, map, game,WEST);
		}
		if (move==2){
			window_create(600,450);
			window_display_image(sprite_get_win(),0,0);
			window_refresh();
			audio_add_sound(AUDIO_DOOR_OPEN);
			audio_add_sound(AUDIO_COINS);
			game_wait(4000);
			return(1);
		}
	}       

	return 0;
}
void game_wait(uint16_t duration){
	SDL_Delay(duration);
	SDL_Event event;
	while (SDL_PollEvent(&event)) {};
	game_arrow_pressed=0;
}

int game_update(struct game* game) {
	if (!(game->isPaused)){
		game->previousUpdate=game->actualUpdate;
		game->actualUpdate=SDL_GetTicks();
		game->elapsedTime=game->actualUpdate - game->previousUpdate;
		bombs_set_update(game);
		mob_set_update(game_get_current_map(game),game->player,game->level,game->elapsedTime);

	}
	if (player_get_lifes( game->player)==0){
		window_create(600,450);
		window_display_image(sprite_get_game_over(),0,0);
		window_refresh();
		audio_add_sound(AUDIO_GAME_OVER);
		game_wait(5000);
		return(1);
	}
	if (input_keyboard(game)){
		// exit game
		return(1);
	}
	else{
		return(0);
	}
}

uint16_t game_get_elapsed_time(struct game* game){
	return(game->elapsedTime);
}
struct map* game_get_map(struct game* game, uint8_t level){
	return(game->maps[level]);
}
uint8_t game_get_level(struct game* game){
	return(game->level);
}

uint8_t game_get_nb_of_level(struct game* game){
	return(game->levels);
}

void game_map_update(struct game* game,int level){
	game->level = level;
}

short game_get_levels(struct game* game){
	return game->levels;
}

void game_setelapsedtime(struct game* game, uint16_t elapsedtime){
	game->elapsedTime = elapsedtime;
}

void game_setactualupdate(struct game* game, uint32_t actualupdate){
	game->actualUpdate = actualupdate;
}

void game_setpreviousupdate(struct game* game, uint32_t previousupdate){
	game->previousUpdate = previousupdate;
}

void game_setnblevel(struct game* game, int nblevel){
	game->levels = nblevel;
}

void game_set_map(struct game* game, int level){
	game->maps[level] = load_map(game->maps[level],level);
}

void game_set_player(struct game* game){
	game->player = player_init(1,1);
	load_player(game->player);
}

void game_load(struct game* game){
	bombs_set_free();
	game_setnblevel(game,load_game(1));
	for (int k = 0; k < game->levels; k++){
     		game_set_map(game, k);
        }
	game_setelapsedtime(game, load_game(3));
	game_setpreviousupdate(game, load_game(5));
	game_setactualupdate(game, load_game(4));
	world_switch(game, load_game(2)+1);
	game_set_player(game);
	load_bomb(game);
	mob_set_load(game);
    game_update(game);
}
