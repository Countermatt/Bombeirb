#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <save.h>
#include <game.h>
#include <player.h>
#include <map.h>
#include <constant.h>
#include <bomb.h>
#include <lists.h>
#include <bombs_set.h>
#include <mob.h>
#include <mob_set.h>

#define PLAYER_FILE 	"save/player.txt"
#define GAME_FILE	"save/game.txt"	
#define BOMB_FILE	"save/bomb.txt"

//player save
void save_player(struct player* player){
	FILE *fichierplayer = fopen(PLAYER_FILE, "w");
	 uint16_t x = player_get_x(player);
	 uint16_t y = player_get_y(player);
	 uint16_t direction = player_get_direction(player);
	 uint8_t nbbomb = player_get_nb_bomb(player);
	 uint8_t nbmaxbomb = player_get_nb_max_bomb(player);
	 uint8_t range = player_get_bomb_range(player);
	 uint8_t lifes = player_get_lifes(player);
	 uint8_t key = player_get_nb_keys(player);
	 uint16_t movecooldown = player_get_movecooldown(player);
	 uint16_t invincibilityduration = player_get_invincibilityduration(player);
	//gcc in cigwyn does not support the identifiers with hh, so we are forced to save in hu instead of hhu
	fprintf(fichierplayer, "%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n%hu", x, y, direction, nbbomb, nbmaxbomb, range, lifes, key, movecooldown, invincibilityduration);
	fclose(fichierplayer);
}

// map save
char *save_map_get_file(int level){
        char* lv;
	switch(level){
                case 0:
                        lv = "save/1.txt";
                break;
                case 1:
                        lv = "save/2.txt";
                break;
                case 2:
                        lv = "save/3.txt";
                break;
                case 3:
                        lv = "save/4.txt";
                break;
                case 4:
                        lv = "save/5.txt";
                break;
                case 5:
                        lv = "save/6.txt";
                break;
                case 6:
			lv = "save/7.txt";
                break;
                case 7:
                        lv = "save/8.txt";
                break;
                default:
			lv = "save/1.txt";
                break;
        }
	return lv;

}

void save_map(struct map* map, int level){
	FILE *fichiermap = fopen(save_map_get_file(level), "w");
	int width = map_get_width(map); 
	int height = map_get_height(map);
	unsigned char mapgrid[width * height];
	fprintf(fichiermap, "%i:%i\n",width, height);
        for (int y = 0; y < height; y++){
                for (int x = 0; x < width; x++){
                        mapgrid[y*height + x] = map_get_cell( map, y, x) ;
                }
        }
	for(int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){
			if (x == height-1){
				fprintf( fichiermap, "%i \n",mapgrid[x*height + y]);
			}
			else{
				fprintf( fichiermap, "%i ",mapgrid[x*height + y]);
			}
		}
	}
	fclose(fichiermap);
}

void save_bomb(struct game* game){
        struct List* list = get_list_bomb();
        struct Bomb* bomb;
        FILE* fichierbomb = fopen(BOMB_FILE, "w+");
        uint8_t levelandrange;
        uint16_t timer;
        uint8_t x,y;
        while(list!=NULL){
                bomb = (struct Bomb*) list->element;
                levelandrange = bomb_get_levelAndRange(bomb);
                timer = bomb_get_timer(bomb);
                x = bomb_get_x(bomb);
                y = bomb_get_y(bomb);
                fprintf(fichierbomb, "%u %u %u %u \n",levelandrange, timer, x, y);
                list = list->next;

        }
        fclose(fichierbomb);

}

void save_game(struct game* game){
	uint16_t nblevel =(uint16_t) game_get_nb_of_level(game);
	for(int lv = 0; lv < nblevel; lv++){	
		save_map( game_get_map( game, lv), lv);
	}
	save_player(game_get_player(game));
	save_bomb(game);
	save_mobs();
	FILE* fichiergame = fopen(GAME_FILE, "w");
	uint16_t level = (uint16_t) game_get_level(game);
	uint16_t elapsedtime = game_get_elapsed_time(game);
	uint32_t actualupdate = game_get_actual_update(game);
	uint32_t previousupdate = game_get_previous_update(game);
	fprintf( fichiergame, "%hu\n%hu\n%hu\n%u\n%u",nblevel,level,elapsedtime,actualupdate,previousupdate);
	fclose(fichiergame);

}

void save_mobs_level(uint8_t level)
{
	char lv[20] = "save/mob/_.txt";
	lv[9]= '1' + level;
	FILE *filemob = fopen(lv,"w");
	uint16_t status;
	uint16_t is_in_box;
	uint16_t range;
	uint16_t lifes;
	uint16_t moveCooldown;
	uint16_t moveCooldownInitial;
	uint16_t direction;
	for (int k=0 ; k<mob_set_mob_in_level(level) ; k++){
		mob_get_infos(mob_set_get_mob(level,k),	&status,&is_in_box,&range,&lifes,&moveCooldown,&moveCooldownInitial,&direction);
		fprintf(filemob,"%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n%hu\n\n",status,is_in_box,range,lifes,moveCooldown,moveCooldownInitial,direction);
	}
	fclose(filemob);
}


void save_mobs(){
	for (uint8_t level=0;level<8;level++){
		save_mobs_level(level);
	}
}
