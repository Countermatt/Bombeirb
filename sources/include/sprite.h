/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>

// load game's sprites
void sprite_load();
// free game's sprites
void sprite_free();

SDL_Surface* sprite_get_chest();
SDL_Surface* sprite_get_player(enum direction direction);
SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type);
SDL_Surface* sprite_get_wall(uint8_t wall_id);
SDL_Surface* sprite_get_bomb(short tilt);
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_door_opened();
SDL_Surface* sprite_get_door_closed();
SDL_Surface* sprite_get_torch(uint8_t id);

SDL_Surface* sprite_get_keys(uint8_t id);
SDL_Surface* sprite_get_blasts(uint8_t id);
SDL_Surface* sprite_get_bites(uint8_t id);
SDL_Surface* sprite_get_smokes(uint8_t id);

SDL_Surface* sprite_get_wall_top(uint8_t wall_id);
SDL_Surface* sprite_get_floor(uint8_t id);

SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();
SDL_Surface* sprite_get_banner_level();
SDL_Surface* sprite_get_explosion(uint8_t frame);
SDL_Surface* sprite_get_shield();
SDL_Surface* sprite_get_player_walking(enum direction direction, uint8_t variation);
SDL_Surface* sprite_get_mob(enum direction direction);
SDL_Surface* sprite_get_pause();
SDL_Surface* sprite_get_menu(int type);
SDL_Surface* sprite_get_game_over();
SDL_Surface* sprite_get_win();
#endif /* SPRITE_H_ */
