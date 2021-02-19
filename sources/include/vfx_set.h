#ifndef VFX_SET_H_
#define VFX_SET_H_

#include <stdint.h>
#include <vfx.h>
#include <player.h>


void vfx_set_display(uint8_t level,uint16_t elapsed_time);
void vfx_set_foreground_display();

struct Vfx* vfx_set_add_pos_vfx(uint8_t level,int x, int y,enum vfx_type type,int duration);
struct Vfx* vfx_set_add_player_vfx(struct player* player,enum vfx_type type,int duration);
void vfx_set_add_foreground(uint8_t x, uint8_t y,uint8_t type);
void vfx_set_free();
void vfx_set_foregrounds_free();


#endif /* VFX_SET_H_ */