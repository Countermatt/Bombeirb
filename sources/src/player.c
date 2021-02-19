/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <map.h>
#include <game.h>
#include <player.h>
#include <bombs_set.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <vfx_set.h>
#include <world.h>
#include <audio.h>
#include <mob.h>


//player struct define + player init
struct player {
	int x, y;
	enum direction direction;
	uint8_t nbBombs;
	uint8_t nbMaxBombs;
	uint8_t range;
	uint8_t lifes;
	uint8_t nbKeys;
	uint16_t moveCooldown;
	uint16_t invincibilityDuration;
};

struct player* player_init(int bomb_number, int range) {
        struct player* player = malloc(sizeof(*player));
        if (!player)
                error("Memory error");

        player->direction = NORTH;
        player->nbBombs = bomb_number;
        player->nbMaxBombs = bomb_number;
        player->range = range;
        player->lifes = 3;
        player->nbKeys = 0;
	player->moveCooldown = 0;
        player->invincibilityDuration = 0;
        return player;
}

void player_from_save(int x,int y, int direction, uint8_t nbbombs, uint8_t nbmaxbombs, uint8_t range, uint8_t lifes, uint8_t nbkeys, uint16_t movecooldown, uint16_t invincibilityduration,struct player* player){
	player->x = x;
	player->y = y;
	player->direction = direction;
        player->nbBombs = nbbombs;
        player->nbMaxBombs = nbmaxbombs;                                                             
        player->range = range;
	player->lifes = lifes;
        player->nbKeys = nbkeys;
        player->moveCooldown = movecooldown;
        player->invincibilityDuration = invincibilityduration;
}

// get information from player
int player_get_x(struct player* player) {
        assert(player);
        return player->x;
}

int player_get_y(struct player* player) {
        assert(player);
        return player->y;
}

int player_get_direction(struct player* player){
	assert(player);
	return player->direction;
}

uint8_t player_get_nb_bomb(struct player* player) {
	assert(player);
        return player->nbBombs;
}

uint8_t player_get_nb_max_bomb(struct player* player){
	assert(player);
	return player->nbMaxBombs;
}

uint8_t player_get_bomb_range(struct player* player){
        assert(player);
        return player->range;
}

uint8_t player_get_lifes(struct player* player){
        assert(player);
        return player->lifes;
}

uint8_t player_get_nb_keys(struct player* player){
	assert(player);
	return player->nbKeys;
}

uint16_t player_get_movecooldown(struct player* player){
	assert(player);
	return player->moveCooldown;
}	

uint16_t player_get_invincibilityduration(struct player* player){
	assert(player);
	return player->invincibilityDuration;
}

//player position modifying
void player_set_position(struct player *player, int x, int y) {
        assert(player);
        player->x = x;
        player->y = y;
}

//player direction modifying
void player_set_current_way(struct player* player, enum direction way) {
        assert(player);
        player->direction = way;
}


//nb bombs modifying

void player_set_nb_bomb(struct player* player, uint8_t bomb){
	assert(player);
	player->nbBombs = bomb;
}

void player_inc_nb_bomb(struct player* player) {
        assert(player);
        if (player->nbBombs<player->nbMaxBombs) { player->nbBombs+=1;}
}

void player_dec_nb_bomb(struct player* player) {
        assert(player);
        if (player->nbBombs>0){player->nbBombs -= 1;}
}


//nb max bombs modifying

void player_set_max_nb_bombs(struct player* player, uint8_t nbbombsmax){
	assert(player);
	player->nbMaxBombs = nbbombsmax;
}

void player_inc_max_nb_bombs(struct player* player){
        assert(player);
        if (player->nbMaxBombs<9){player->nbMaxBombs += 1;}
}

void player_dec_max_nb_bombs(struct player* player){
        assert(player);
        if (player->nbMaxBombs > 1){
                player->nbMaxBombs -= 1;
                if (player->nbBombs>player->nbMaxBombs){
                        player->nbBombs=player->nbMaxBombs;
                }
        }
}


//range modifying

void player_set_range(struct player* player, uint8_t range){
	assert(player);
	player->range = range;
}

void player_inc_bomb_range(struct player* player){
        assert(player);
        if (player->range < 9){player->range += 1;}
}
void player_dec_bomb_range(struct player* player){
        assert(player);
        if (player->range > 1){ player->range -= 1;}
}


//nb life modifying

void player_set_lifes(struct player* player, uint8_t lifes){
	assert(player);
	player->lifes = lifes;
}

void player_inc_lifes(struct player* player){
        assert(player);
        if (player->lifes < 9){player->lifes += 1;}
}

void player_dec_lifes(struct player* player){
        assert(player);
        if (player->lifes > 0){
		player->lifes -= 1;
	}
}

//nb key modifying

void player_set_nb_keys(struct player* player, uint8_t nbkeys){
	assert(player);
	player->nbKeys = nbkeys;
}

void player_inc_nb_keys(struct player* player){
	assert(player);
	player->nbKeys = (player->nbKeys < 9 ? player->nbKeys + 1 : player->nbKeys);
	audio_add_sound(AUDIO_KEY_FOUND);
}

void player_dec_nb_keys(struct player* player){
	assert(player);
	player->nbKeys = (player->nbKeys > 0 ? player->nbKeys - 1 : player->nbKeys);
}

//movecooldown modifying

void player_set_movecooldown(struct player* player, uint16_t movecooldown){
	assert(player);
	player->moveCooldown = movecooldown;
}

//invincibilityduration modifying

void player_set_invincibilityduration(struct player* player, uint16_t invincibilityduration){
	assert(player);
	player->invincibilityDuration = invincibilityduration;
}

//player bomb placing
void player_place_bomb(struct game* game){
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int x = player_get_x(player);
	int y = player_get_y(player);
	if (player_get_nb_bomb(player) > 0 && map_get_cell_type( map, x, y) == CELL_EMPTY) {
		bombs_set_create_bomb( x, y, game_get_level(game), player_get_bomb_range(player));
		map_place_bomb(map, x, y, 0);
		player_dec_nb_bomb(player);
		audio_add_sound(AUDIO_BOMB_PLACE);

	}
}


//player damage control returns 1 player has realy been hurt
uint8_t player_damage(struct player* player){
	if (player->invincibilityDuration == 0){
		player_dec_lifes(player);
		player->invincibilityDuration = PLAYER_RECEIVE_DAMAGE_COOLDOWN;
		vfx_set_add_player_vfx( player, EFFECT_PLAYER_SHIELD, PLAYER_RECEIVE_DAMAGE_COOLDOWN*9/10);
		audio_add_sound(AUDIO_PLAYER_OUCH);
		return(1);
	}
	return(0);
}

//player erase
void player_free(struct player* player) {
	assert(player);
	free(player);
}


//player bonus impact
void player_bonus(enum bonus_type bonus, struct player* player){
	vfx_set_add_player_vfx(player,EFFECT_PLAYER_BLAST,1000);
	audio_add_sound(AUDIO_BONUS);
	switch (bonus)
	{
		case BONUS_LIFE:
			player_inc_lifes(player);
			break;
		case BONUS_BOMB_NB_INC:
			player_inc_max_nb_bombs(player);
			player_inc_nb_bomb(player);
			break;
		case BONUS_BOMB_NB_DEC:
			player_dec_max_nb_bombs(player);
			break;
		case BONUS_BOMB_RANGE_INC:
			player_inc_bomb_range(player);
			break;
		case BONUS_BOMB_RANGE_DEC:
			player_dec_bomb_range(player);
			break;
		default:
			break;
	}

}


//player mouvement
int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside( map, x, y))	//know if player in map
		return 0;

	switch (map_get_cell_type( map, x, y)) {			//know next  case
	case CELL_SCENERY:
		if (map_get_cell_subtype( map, x, y)==SCENERY_TREASURE && player_get_nb_keys(player)){
			return(5);
		}
		return 0;
		break;

	case CELL_BOX:
		return 3;		//know the next cell is a box
		break;
	case CELL_BOMB:
		return 0;
		break;
	case CELL_BONUS:
		player_bonus(map_get_cell_subtype( map, x, y), player);
		map_set_cell_type( map, x, y, CELL_EMPTY);
		return 2;
		break;

	case CELL_MONSTER:
		return 2;
		break;

	case CELL_KEY:
		player_inc_nb_keys(player);
		map_set_cell_type( map, x, y, CELL_EMPTY);
		vfx_set_add_player_vfx(player,EFFECT_PLAYER_KEY,500);
		return 2;
		break;

	case CELL_DOOR:
		return 4;
		break;

	default:
		return 1;
		break;
	}

	// Player has moved
}

int player_move(struct player* player, struct map* map, struct game* game,enum direction direction){
	if (!(player->moveCooldown)){
		int x = player->x;
		int y = player->y;
		player->direction = direction;
		int move = 0;
		switch (player->direction) {
		case NORTH:
			switch (player_move_aux(player, map, x, y-1)){
			case 2:
				player->y--;
				move = 1;
				break;

			case 1:
				player->y--;
				move = 1;
				break;

			case 3:
				if (player_move_aux(player, map, x, y-2)==1){
					audio_add_sound(AUDIO_BOX_SCRAP);
					player->y--;
					move = 1;
					map_move_cell(map,x,y-1,x,y-2);					
				}
				break;
			case 4:
				if(is_door_open(door_info(map,x,y-1))==1){
					world_switch(game,door_info(map,x,y-1));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				if(is_door_open(door_info(map,x,y-1))==0 && player_get_nb_keys(player)>0){
					player_dec_nb_keys(player);
					map_open_door(map,x,y-1);
					world_switch(game,door_info(map,x,y-1));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				break;
			case 5:
				return(2);
				break;
			default:
				break;
			}
			break;

		case SOUTH:
			switch (player_move_aux(player, map, x, y+1)){
			case 2:
				player->y++;
				move = 1;
				break;

			case 1:
				player->y++;
				move = 1;
				break;

			case 3:
				if (player_move_aux(player, map, x, y+2)==1){
					audio_add_sound(AUDIO_BOX_SCRAP);
					player->y++;
					move = 1;
					map_move_cell(map,x,y+1,x,y+2);
				}
				break;
			case 4:
				if(is_door_open(door_info(map,x,y+1))==1){
					player->direction=SOUTH;
					world_switch(game,door_info(map,x,y+1));
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				if(is_door_open(door_info(map,x,y+1))==0 && player_get_nb_keys(player)>0){
					player_dec_nb_keys(player);
					map_open_door(map,x,y+1);
					player->direction=SOUTH;
					world_switch(game,door_info(map,x,y+1));
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				break;
			case 5:
				return(2);
				break;
			default:
				break;
			}
			break;

		case WEST:
			switch (player_move_aux(player, map, x-1, y)){
			case 2:
				player->x--;
				move = 1;
				break;

			case 1:
				player->x--;
				move = 1;
				break;

			case 3:
				if (player_move_aux(player, map, x-2, y)==1){
					audio_add_sound(AUDIO_BOX_SCRAP);
					player->x--;
					move = 1;
					map_move_cell(map,x-1,y,x-2,y);
				}
				break;
			case 4:
				if(is_door_open(door_info(map,x-1,y))==1){
					world_switch(game,door_info(map,x-1,y));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				if(is_door_open(door_info(map,x-1,y))==0 && player_get_nb_keys(player)>0){
					player_dec_nb_keys(player);
					map_open_door(map,x-1,y);
					world_switch(game,door_info(map,x-1,y));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				break;
			case 5:
				return(2);
				break;
			default:
				break;
			}
			break;

		case EAST:
			switch (player_move_aux(player, map, x+1, y)){
			case 2:
				player->x++;
				move = 1;
				break;

			case 1:
				player->x++;
				move = 1;
				break;

			case 3:
				if (player_move_aux(player, map, x+2, y)==1){
					audio_add_sound(AUDIO_BOX_SCRAP);
					player->x++;
					move = 1;
					map_move_cell(map,x+1,y,x+2,y);
				}
				break;
			case 4:
				if(is_door_open(door_info(map,x+1,y))==1){
					world_switch(game,door_info(map,x+1,y));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				if(is_door_open(door_info(map,x+1,y))==0 && player_get_nb_keys(player)>0){
					player_dec_nb_keys(player);
					map_open_door(map,x+1,y);
					world_switch(game,door_info(map,x+1,y));
					player->direction=SOUTH;
					player->moveCooldown=PLAYER_MOVE_COOLDOWN/2;
				}
				break;
			case 5:
				return(2);
				break;
			default:
				break;
			}
			
			break;
		}
		if (move){
			player->moveCooldown = PLAYER_MOVE_COOLDOWN;
			audio_add_sound(AUDIO_STEP);
			mob_update_map_ia(map,player_get_bomb_range(player),player_get_x(player),player_get_y(player));
		}
		return move;
	}
	else{
		return 0;
	}
}


//player display

void player_get_visual_pos(struct player* player, uint16_t* xv,uint16_t* yv){
        uint16_t portion_of_block_traveled = (player->moveCooldown*SIZE_BLOC)/(PLAYER_MOVE_COOLDOWN);
        switch (player->direction)
        {
                case NORTH:
                        *xv = player->x*SIZE_BLOC;
                        *yv = player->y*SIZE_BLOC+portion_of_block_traveled;
                        break;
                case SOUTH:
                        *xv = player->x*SIZE_BLOC;
                        *yv = player->y*SIZE_BLOC-portion_of_block_traveled;
                        break;
                case EAST:
                        *xv = player->x*SIZE_BLOC-portion_of_block_traveled;
                        *yv = player->y*SIZE_BLOC;
                        break;
                case WEST:
                        *xv = player->x*SIZE_BLOC+portion_of_block_traveled;
                        *yv = player->y*SIZE_BLOC;
                        break;
                default:
                        break;
        }
}

void player_display(struct player* player) {
        static uint16_t previous_time = 0;
        static uint16_t time = 0;
        uint16_t elapsed_time;
        time = SDL_GetTicks();
        elapsed_time = time-previous_time;
        if (player->invincibilityDuration < elapsed_time){
                player->invincibilityDuration = 0;
        }
        else{
                player->invincibilityDuration -= elapsed_time;
        }
        if (player->moveCooldown < elapsed_time){
                player->moveCooldown = 0;
                assert(player);
                window_display_image( sprite_get_player(player->direction), player->x * SIZE_BLOC, player->y * SIZE_BLOC);
        }
        else{
                player->moveCooldown -= elapsed_time;
                uint16_t xv,yv;
                player_get_visual_pos( player, &xv, &yv);
                window_display_image( sprite_get_player_walking( player->direction, (player->x + player->y)%2), xv, yv);

        }
        previous_time = time;
}

