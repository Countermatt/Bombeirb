#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include <player.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <bombs_set.h>
#include <mob.h>
#include <audio.h>
#include <SDL/SDL.h>

struct player* load_player(struct player* player_save)
{

	char* lv = "save/player.txt";
	FILE *fichierplayer = fopen(lv,"r");
	uint16_t x;
	uint16_t y;
	uint16_t direction;
	//gcc in cigwyn does not support the identifiers with hh, so we are forced to save in hu instead of hhu
	uint16_t nbbombs;
	uint16_t nbmaxbombs;
	uint16_t range;
	uint16_t lifes;
	uint16_t nbkeys;
	uint16_t movecooldown;
	uint16_t invincibilityduration;
	fscanf(fichierplayer,"%hu",&x);
	fscanf(fichierplayer,"%hu",&y);
  	fscanf(fichierplayer,"%hu",&direction);
  	fscanf(fichierplayer,"%hu",&nbbombs);
  	fscanf(fichierplayer,"%hu",&nbmaxbombs);
	fscanf(fichierplayer,"%hu",&range);
	fscanf(fichierplayer,"%hu",&lifes);
 	fscanf(fichierplayer,"%hu",&nbkeys);
	fscanf(fichierplayer,"%hu",&movecooldown);
	fscanf(fichierplayer,"%hu",&invincibilityduration);
	fclose(fichierplayer);
	player_from_save(x, y ,direction, nbbombs, nbmaxbombs, range, lifes, nbkeys, movecooldown, invincibilityduration, player_save);
	return player_save;	
}

void load_array_mob_number(uint8_t* mob_number)
{
	char lv[20] = "mob/mob_number.txt";
	FILE *filemob = fopen(lv,"r");
	//gcc in cigwyn does not support the identifiers with hh, so we can't save uint8_t
	int convtmp;
	for (int k=0 ; k<8 ; k++){
		fscanf(filemob,"%i",&convtmp);
		mob_number[k]=convtmp;
	}
	fclose(filemob);
}

void load_mobs_level(struct Mob*** mob_grid,uint8_t level,uint8_t mob_in_level)
{
	char lv[10] = "mob/_.txt";
	lv[4]= '1' + level;
	FILE *filemob = fopen(lv,"r");
	uint16_t status;
	uint16_t is_in_box;
	uint16_t range;
	uint16_t lifes;
	uint16_t moveCooldownInitial;
	for (int k=0 ; k<mob_in_level ; k++){
		fscanf(filemob,"%hu",&status);
		fscanf(filemob,"%hu",&is_in_box);
		fscanf(filemob,"%hu",&range);
		fscanf(filemob,"%hu",&lifes);
		fscanf(filemob,"%hu",&moveCooldownInitial);
		fscanf(filemob,"\n");//line space
		mob_grid[level][k]=mob_init(status, is_in_box, range ,lifes, moveCooldownInitial, NORTH);	
	}
	fclose(filemob);
}

void load_mobs_level_from_save(struct Mob*** mob_grid,uint8_t level,uint8_t mob_in_level)
{
	char lv[20] = "save/mob/_.txt";
	lv[9]= '1' + level;
	FILE *filemob = fopen(lv,"r");
	uint16_t status;
	uint16_t is_in_box;
	uint16_t range;
	uint16_t lifes;
	uint16_t moveCooldown;
	uint16_t moveCooldownInitial;
	uint16_t direction;
	for (int k=0 ; k<mob_in_level ; k++){
		fscanf(filemob,"%hu",&status);
		fscanf(filemob,"%hu",&is_in_box);
		fscanf(filemob,"%hu",&range);
		fscanf(filemob,"%hu",&lifes);
		fscanf(filemob,"%hu",&moveCooldown);
		fscanf(filemob,"%hu",&moveCooldownInitial);
		fscanf(filemob,"%hu",&direction);
		fscanf(filemob,"\n");//line space
		mob_grid[level][k]=mob_init(status, is_in_box, range ,lifes, moveCooldownInitial, direction);
		mob_set_moveCooldown(mob_grid[level][k],moveCooldown);
	}
	fclose(filemob);
}


char *load_map_get_file(int level)
{
        char* lv;
        switch(level){
                case 0:
                        return lv = "save/1.txt";
                break;
                case 1:
                        return lv = "save/2.txt";
                break;
                case 2:
                        return lv = "save/3.txt";
                break;
                case 3:
                        return lv = "save/4.txt";
                break;
                case 4:
                        return lv = "save/5.txt";
                break;
                case 5:
                        return lv = "save/6.txt";
                break;
                case 6:
                        return lv = "save/7.txt";
                break;
                case 7:
                        return lv = "save/8.txt";
                break;
                default:
                        return lv = "save/1.txt";
                break;

        }
}

struct map* load_map(struct map* map,int level)
{
	struct map* map_save = malloc(sizeof(map));
	map_save = map_get_static(level, 1);
	return map_save;
}

int load_game(int type) //1 nblevel 2 level 3 elaps 4 actual 5 prev
{
	FILE* fichiergame = fopen("save/game.txt", "r");
	uint16_t nblevel;
        uint16_t level;
	uint16_t elapsedtime;
	uint32_t actualupdate;
	uint32_t previousupdate;
        fscanf(fichiergame, "%hu", &nblevel);
	fscanf(fichiergame, "%hu", &level);
 	fscanf(fichiergame, "%hu", &elapsedtime);
 	fscanf(fichiergame, "%u", &actualupdate);
 	fscanf(fichiergame, "%u", &previousupdate);
	fclose(fichiergame);
	switch(type){
		case 1:
			return (int)nblevel;
		break;
		case 2:
			return (int)level;
		break;
		case 3:
			return (int)elapsedtime;
		break;
		case 4:
			return (int)actualupdate;
		break;
		case 5:
			return (int)previousupdate;
		break;
	}
	return EXIT_SUCCESS ;
}

void load_bomb(struct game* game){
	FILE* fichierbomb = fopen("save/bomb.txt", "r");	
	char tampon[80];
	int k = 0;
	const char space[2] = " ";
	char *var;
	uint16_t levelandrange;
	uint16_t timer;
	uint16_t x, y;
	uint32_t loading_time;
	while(fgets(tampon, 80, fichierbomb) != NULL){
		var = strtok(tampon, space);
		while (k<4){
			switch (k){
				case 0:
					levelandrange=atoi(var);
					break;
				case 1:
					timer=atoi(var);
					break;
				case 2:
					x=atoi(var);
					break;
				case 3:
					y=atoi(var);
					break;
			}
			k++;
			var = strtok(NULL, space);
		}
		k = 0;
		loading_time=SDL_GetTicks()- game_get_actual_update(game);
		bombs_set_load_bomb(x,y,levelandrange,timer+loading_time);
			
	}
	fclose(fichierbomb);
}
