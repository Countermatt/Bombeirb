/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <world.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <vfx_set.h>
#include <save.h>
#include <load.h>
#include <mob_set.h>
#include <mob.h>
#include <audio.h>

#define NB_FRAME_FLOOR 20
#define FLOOR_DISPERSION(i,j,level) ((149*(i+j+1)*(level+j+1)%127))%NB_FRAME_FLOOR
#define TORCH_CYCLING_TIME 1000
#define NB_FRAME_TORCH 10

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int level)
{
	int width = map_set_dimensions(level,0);
	int height = map_set_dimensions(level,1);
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}
char *map_get_file(int level)
{
	char* lv;
	switch(level){
		case 0:
			return lv = "map/1.txt";
		break;
		case 1:
			return lv = "map/2.txt";
		break;
		case 2:
			return lv = "map/3.txt";
		break;
		case 3:
			return lv = "map/4.txt";
		break;
		case 4:
			return lv = "map/5.txt";
		break;
		case 5:
			return lv = "map/6.txt";
		break;
		case 6:
			return lv = "map/7.txt";
		break;
		case 7:
			return lv = "map/8.txt";
		break;
		default:
			return lv = "map/1.txt";
		break;

	}
}
int map_set_dimensions(int level,int k) //set map dimensions if k = 0 set width if k = 1 set height
{
	int i = 0;
	char* lv = map_get_file(level);
	FILE *fichiermap = fopen(lv,"r");
	char tampon[7];
	const char separator[2] = ":";
	char *dimensions;
	int result = 0;
	fgets(tampon, 7, fichiermap);
	fclose(fichiermap);
	dimensions = strtok(tampon, separator);
	while (dimensions != NULL){
		if (k == 0 && i == 0){
			result = atoi(dimensions);
			dimensions = NULL;
		}
		else{
			if (k == 1 && i == 1){
			result = atoi(dimensions);
			dimensions = NULL;
			}
		}
		dimensions = strtok(NULL, separator);
		i++;
	}
	return result;
}

int map_is_inside(struct map* map, int x, int y) //verify is (x,y) is in the map
{
	assert(map);
	int width = map->width ;
	int height = map->height;
	if (x>=0 && x < width && y >= 0 && y < height ){
		return 1;
	}
	return 0;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

uint8_t map_get_cell_subtype(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] & 0x0f);
}

void map_place_bomb(struct map* map,int x, int y,int tilt)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = CELL_BOMB | tilt;
}


void map_break_box(struct map* map,int x, int y,uint8_t level)
{
	assert(map && map_is_inside(map, x, y));
	if (map_get_cell_type(map,x,y) == CELL_BOX){
		short bonus = map->grid[CELL(x,y)] & 0x0f;
		if (bonus == 0){
			map->grid[CELL(x,y)] = CELL_EMPTY;
		}
		else if(bonus == BONUS_KEY){
			map->grid[CELL(x,y)] = CELL_KEY;
		}
		else if(bonus>=BONUS_MONSTER0){
			mob_set_xy(mob_set_get_mob(level,bonus-BONUS_MONSTER0),x,y);
			map->grid[CELL(x,y)]= CELL_MONSTER | (bonus-BONUS_MONSTER0);
		}
		else {
			map->grid[CELL(x,y)] = CELL_BONUS | bonus;
		}
	}

}
void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	switch (type & 0x0f) {
	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y);
		break;
	default:
		window_display_image(sprite_get_bonus(type & 0x0f), x, y);
		break;

	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type,uint16_t time)
{
	uint8_t subtype = type & 0x0f;// sub-types are encoded with the 4 less significant bits
	if (subtype<14) {
		window_display_image(sprite_get_wall(subtype), x, y);
	}
	else if (subtype == SCENERY_WALL_HORIZONTAL_TORCH){
		window_display_image(sprite_get_wall(SCENERY_WALL_HORIZONTAL), x, y);
		window_display_image(sprite_get_torch(((NB_FRAME_TORCH * time)/TORCH_CYCLING_TIME)%NB_FRAME_TORCH), x+13, y);
	}
	else if (subtype == SCENERY_TREASURE){
		window_display_image(sprite_get_chest(),x,y);
	}
}
void map_display(struct map* map,uint8_t level,uint16_t time)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
					case CELL_EMPTY:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
						break;
					case CELL_SCENERY:
				  	display_scenery(map, x, y, type,time);
				  	break;
			    case CELL_BOX:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
			      window_display_image(sprite_get_box(), x, y);
			      break;
			    case CELL_BONUS:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
			      display_bonus(map, x, y, type);
			      break;
			    case CELL_KEY:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
			      window_display_image(sprite_get_key(), x, y);
			      break;
			    case CELL_DOOR:
			      // pas de gestion du type de porte
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
						if (is_door_open(type & 0x01)){
							window_display_image(sprite_get_door_opened(), x, y);
							}
						else{
							window_display_image(sprite_get_door_closed(),x, y);
							}
			      break;
					case CELL_BOMB:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
			      window_display_image(sprite_get_bomb(type & 0x0f), x, y);
			      break;
					case CELL_MONSTER:
						window_display_image(sprite_get_floor(FLOOR_DISPERSION(i,j,level)), x, y);
						break;
	    }
	  }
	}
}

struct map* map_get_static(int level,int type) // type = 0 if map init, 1 if map load
{
	struct map* map = map_new(level);
	unsigned char themap[map->height * map->width];
	char* lv;
	if (type == 0){
		lv = map_get_file(level);
	}
	else{
		lv = load_map_get_file(level);	
	}
	FILE *fichiermap=fopen(lv,"r");
	char tampon[80];
	int i=0;
	int k = 0;
	const char space[2] = " ";
	char *cell;
	while (fgets(tampon, 80, fichiermap) != NULL ){
			cell = strtok(tampon, space);
			if( k != 0){
			while (cell != NULL){
				int a = atoi(cell);
				themap[i] = a;
				i++;
				cell = strtok(NULL, space);
			}
			i--;
			}
			k++;
	}
	fclose(fichiermap);

	/*unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	};*/

	for (int i = 0; i < map_get_width(map) * map_get_height(map); i++)
		map->grid[i] = themap[i];

	return map;
}

int door_info(struct map* map, int x,int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

void map_open_door(struct map* map, int x,int y){
	map->grid[CELL(x,y)]=map->grid[CELL(x,y)]|0x01;
	audio_add_sound(AUDIO_DOOR_OPEN);
}
void map_move_cell(struct map* map,uint8_t x, uint8_t y ,uint8_t newx , uint8_t newy){
	map->grid[CELL(newx,newy)]  =map->grid[CELL(x,y)];
	map->grid[CELL(x,y)]= CELL_EMPTY;

}

void map_foreground_load(struct map* map){
	uint8_t cell;
	vfx_set_foregrounds_free();
	for (uint8_t i=1;i < map->height;i++){ //first line cannot have a foreground
		for (uint8_t j=0;j < map->width;j++){
			cell= map->grid[CELL(j,i)];
			switch (cell & 0xf0)
			{
				case CELL_SCENERY:
					if ((cell & 0xf)<14){
						vfx_set_add_foreground(j,i-1,cell & 0xf);
					}
					if (cell==CELL_WALL_HORIZONTAL_TORCH){
						vfx_set_add_foreground(j,i-1,SCENERY_WALL_HORIZONTAL);
					}
					break;

				default:
					break;
			}

	}
}
}

int map_get_cell(struct map* map, int x, int y){
	return map->grid[CELL(x,y)];
}
