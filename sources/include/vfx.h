#ifndef VFX_H_
#define VFX_H_


#include <stdint.h>
#include <player.h>

enum vfx_type{
    EFFECT_EXPLOSION,
    EFFECT_PLAYER_SHIELD,
    EFFECT_PLAYER_KEY,
    EFFECT_PLAYER_BLAST,
    EFFECT_PLAYER_BITE,
    EFFECT_SMOKE
    
};
struct Vfx;
struct Foreground;

void vfx_free(void* vfx_void);
struct Vfx* vfx_pos_init(uint8_t level, int x, int y,enum vfx_type type,int duration);
struct Vfx* vfx_onplayer_init(struct player* player,enum vfx_type type,int duration);
uint8_t vfx_update(struct Vfx* vfx,uint8_t level,uint16_t elapsed_time);
struct Foreground* vfx_foreground_init(uint8_t x, uint8_t y,uint8_t type);
void vfx_foreground_display(struct Foreground* fg);



#endif /* VFX_H_ */