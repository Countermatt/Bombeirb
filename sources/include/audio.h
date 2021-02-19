#ifndef AUDIO_H_
#define AUDIO_H_
enum audio_type{
    AUDIO_STEP,
    AUDIO_EXPLOSION,
    AUDIO_FUSE,
    AUDIO_BOMB_PLACE,
    AUDIO_BOX_SCRAP,
    AUDIO_BOX_BREAK,
    AUDIO_PLAYER_OUCH,
    AUDIO_MOB_HURT, 
    AUDIO_MOB_BITE,
    AUDIO_DOOR_OPEN,
    AUDIO_KEY_FOUND,
    AUDIO_BONUS,
    AUDIO_COINS,
    AUDIO_GAME_OVER
};
int audio_add_sound(enum audio_type type );
void audio_close();
void audio_init(); 
void audio_free();
void audio_music_switch();
#endif /* AUDIO_H_ */