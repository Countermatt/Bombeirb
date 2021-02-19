#include <assert.h>
#include <time.h>
#include <stdint.h>

#include <window.h>
#include <sprite.h>
#include <menu.h>
#include <audio.h>
#include <load.h>
#include <game.h>

struct menu {
	short type;
	short status;	//1 if in menu 0 else
	short game_exit;
	short game_load; //0 if noload before play 1 else
};

struct menu* menu_init(void){
	window_create(600, 450);
	struct menu* menu = malloc(sizeof(*menu));
	menu->type = 0;
	menu->status = 1;
	menu->game_exit = 0;
	menu->game_load = 0;
	menu_display(menu);
	return menu;
}

int menu_update(struct menu* menu,struct game* game){
	int type = menu_get_type(menu);
	uint8_t return_value = input_keyboard_menu(menu,game);
	if (type!=menu->type){ //we only need to display a new image if the menu's status changed
		menu_display(menu);
	}
	return(return_value);
}

void menu_display(struct menu* menu){
	int type = menu_get_type(menu);

	window_clear();
	
	window_display_image(sprite_get_menu(type), 0, 0);
	window_refresh();
}

short menu_get_status(struct menu* menu){
	assert(menu);
	return menu->status;
}

short menu_get_type(struct menu* menu){
	assert(menu);
	return menu->type;
}

short menu_get_exit(struct menu* menu){
	assert(menu);
	return menu->game_exit;
}

short menu_get_load(struct menu* menu){
        assert(menu);
        return menu->game_load;
}


void menu_switch_status(struct menu* menu){
	assert(menu);
	if (menu->status)
		menu->status = 0;
	else
		menu->status = 1;
}

void menu_switch_type(struct menu* menu, int way){		//way = 0 meny->type-1 , way = 1 menu->type+1
	if (way){
		if (menu->type<3)
			menu->type++;	
	}
	else
	{
		if(menu->type>0)
			menu->type--;
	}
}

void menu_set_type(struct menu* menu, short type){
	assert(menu);
	menu->type = type;
}

void menu_set_status(struct menu* menu, short status){
	assert(menu);
	menu->status = status;
}

void menu_set_game_exit(struct menu* menu){
	assert(menu);
	menu->game_exit = 1;
}

short input_keyboard_menu(struct menu* menu, struct game* game){
	SDL_Event event;
	uint8_t pressed=0;

	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				menu->status=0;
				return 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_UP:
						pressed |= 0x01;
						break;
					case SDLK_DOWN:
						pressed |= 0x04;
						break;
					case SDLK_SPACE:
						switch (menu_get_type(menu)){
							case 0:
								menu_set_status(menu,0);
								break;
							case 1:
								menu_set_status(menu,0);			
								menu_set_game_load(menu);
								break;
							case 2:
								audio_music_switch();
								break;
							case 3:
								menu->status=0;
								return 1;
								break;
						}
						break;
					case SDLK_ESCAPE:
						menu->status=0;
						return 1;
						break;
					default:
						break;
			}
			break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
						pressed &= 0xFE; /* FF - 01*/
						break;
				case SDLK_DOWN:
						pressed &= 0xFB; /* FF - 04*/
						break;
				default:
						break;
			}
			break;

		}
	}
        if (pressed & 0x01){
        	menu_switch_type(menu,0);
		menu_display(menu);
        }
        else if (pressed & 0x04){
		menu_switch_type(menu,1);
		menu_display(menu);
        }
	return 0;
}

void menu_set_game_load(struct menu* menu){
        assert(menu);
        menu->game_load = 1;
}

void menu_free(struct menu* menu){
	assert(menu);
	free(menu);
}
