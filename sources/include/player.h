/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_




struct player;

#include <map.h>
#include <constant.h>
#include <game.h>
#include <stdint.h>


// player init
struct player* player_init(int bomb_number,int range);
void player_from_save(int x,int y, int direction, uint8_t nbbombs, uint8_t nbmaxbombs, uint8_t range, uint8_t lifes, uint8_t nbkeys, uint16_t movecooldown, uint16_t invincibilityduration, struct player* player);
//get information from player
int player_get_x(struct player* player);
int player_get_y(struct player* player);
int player_get_direction(struct player* player);
uint8_t player_get_nb_bomb(struct player* player);
uint8_t player_get_nb_max_bomb(struct player* player);
uint8_t player_get_bomb_range(struct player* player);
uint8_t player_get_lifes(struct player* player);
uint8_t player_get_nb_keys(struct player* player);
uint16_t player_get_movecooldown(struct player* player);
uint16_t player_get_invincibilityduration(struct player* player);

//player position modifying
void player_set_position(struct player* player, int x, int y);

//player direction modifying
void player_set_current_way(struct player* player, enum direction way);

//nb bombs modifying
void player_set_nb_bomb(struct player* player, uint8_t bomb);
void player_inc_nb_bomb(struct player* player);
void player_dec_ng_bomb(struct player* player);

//nb max bombs modifying
void player_set_max_nb_bombs(struct player* player, uint8_t nbbombmax);
void player_inc_max_nb_bombs(struct player* player);
void player_dec_max_nb_bombs(struct player* player);

//range modifying
void player_set_range(struct player* player, uint8_t range);
void player_inc_bomb_range(struct player* player);
void player_dec_bomb_range(struct player* player);

//nb life modifying
void player_set_lifes(struct player* player, uint8_t lifes);
void player_inc_lifes(struct player* player);
void player_dec_lifes(struct player* player);

//nb key modifying
void player_set_nb_keys(struct player* player, uint8_t nbkeys);
void player_inc_nb_keys(struct player* player);
void player_dec_nb_keys(struct player* player);

//movecooldown modifying
void player_set_movecooldown(struct player* player, uint16_t movecooldown);

//invincibilityduration modifying
void player_set_invincibilityduration(struct player* player, uint16_t invincibilityduration);

//player bomb placing
void player_place_bomb(struct game* game);

//player damage control
uint8_t player_damage(struct player* player);

//player erase
void player_free(struct player* player);

//player bonus impact
void player_bonus(enum bonus_type bonus, struct player* player);

//player movement
int player_move_aux(struct player* player, struct map* map, int x, int y);
//return 0 if the player didn't move, 1 if he did and 2 if he reached treasure (victory)
int player_move(struct player* player, struct map* map, struct game* game, enum direction direction);

//player display
void player_get_visual_pos(struct player* player, uint16_t* xv, uint16_t* yv);
void player_display(struct player* player);
#endif /* PLAYER_H_ */
