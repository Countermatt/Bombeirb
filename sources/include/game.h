/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_



// Abstract data type
struct game;


#include <player.h>
#include <map.h>
#include <stdint.h>
#include <world.h>

// Create a new game
struct game* game_new();

uint32_t game_get_actual_update(struct game* game);

uint32_t game_get_previous_update(struct game* game);

short game_is_paused(struct game* game);

void game_free(struct game* game);

struct map* game_get_current_map(struct game* game);

struct player* game_get_player(struct game* game);

void game_banner_display(struct game* game);

void game_display(struct game* game);

uint8_t game_get_nb_of_level(struct game* game);

int game_update(struct game* game);

uint16_t game_get_elapsed_time(struct game* game);

struct map* game_get_map(struct game* game, uint8_t level);

void game_map_update(struct game* game, int level);

uint8_t game_get_level(struct game* game);

void game_setelapsedtime(struct game* game, uint16_t elapsedtime);

void game_setactualupdate(struct game* game, uint32_t actualupdate);

void game_setpreviousupdate(struct game* game, uint32_t previousupdate);

void game_setnblevel(struct game* game, int nblevel);

void game_set_map(struct game* game, int level);

void game_set_player(struct game* game);

void game_load(struct game* game);

void game_wait(uint16_t duration);
#endif /* GAME_H_ */
