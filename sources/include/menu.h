#ifndef MENU_H_
#define MENU_H
#include <game.h>

struct menu;

struct menu* menu_init(void);

int menu_update(struct menu* menu, struct game* game);

void menu_display(struct menu* menu);

short menu_get_status(struct menu* menu);

short menu_get_type(struct menu* menu);

short menu_get_exit(struct menu* menu);

short menu_get_load(struct menu* menu);

void menu_switch_status(struct menu* menu);

void menu_switch_type(struct menu* menu, int way);

void menu_set_type(struct menu* menu, short type);

void menu_set_status(struct menu* menu, short status);

void menu_set_game_exit(struct menu* menu);

short input_keyboard_menu(struct menu* menu, struct game* game);

void menu_set_game_load(struct menu* menu);

void menu_free(struct menu* menu);
#endif /* MENU_H_ */

