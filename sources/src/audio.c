#include <SDL/SDL.h>
#include <audio.h>
#include <assert.h>



#define WAV_PATH_MUSIC "sounds/music.wav"
#define WAV_PATH_STEP1 "sounds/steps/hard-footstep1.wav"
#define WAV_PATH_STEP2 "sounds/steps/hard-footstep2.wav"
#define WAV_PATH_STEP3 "sounds/steps/hard-footstep3.wav"
#define WAV_PATH_STEP4 "sounds/steps/hard-footstep4.wav"
#define WAV_PATH_START "sounds/UI_039.wav"
#define WAV_PATH_FUSE "sounds/fuse.wav"
#define WAV_PATH_EXPLOSION "sounds/explosion.wav"
#define WAV_PATH_PLACE_BOMB "sounds/place.wav"
#define WAV_PATH_BOX_SCRAPE1 "sounds/box_slide/scrape-1.wav"
#define WAV_PATH_BOX_SCRAPE2 "sounds/box_slide/scrape-2.wav"
#define WAV_PATH_BOX_BREAK "sounds/box_break.wav"
#define WAV_PATH_PLAYER_OUCH "sounds/ouch.wav"
#define WAV_PATH_MOB_HURT "sounds/mob_hurt.wav"
#define WAV_PATH_MOB_BITE "sounds/mob_bite.wav"
#define WAV_PATH_MOB_BITE "sounds/mob_bite.wav"
#define WAV_PATH_KEY_FOUND "sounds/key_found.wav"
#define WAV_PATH_BONUS "sounds/bonus.wav"
#define WAV_PATH_DOOR_OPEN "sounds/door_open.wav"
#define WAV_PATH_COINS "sounds/coins.wav"
#define WAV_PATH_GAME_OVER "sounds/game_over.wav"
// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, uint8_t *stream, int len);

// variable declarations
static uint8_t *audio_initial_pos;
static uint8_t *audio_pos; // global pointer to the audio buffer to be played
static uint32_t audio_len; // remaining length of the sample we have to play
static uint8_t is_playing= 0;

static uint8_t* steps_p[4];
static uint32_t steps_length[4];

static uint8_t* explosion;
static uint32_t explosion_length;

static uint8_t* place;
static uint32_t place_length;

static uint8_t* fuse;
static uint32_t fuse_length;

static uint8_t* scrap[2];
static uint32_t scrap_length[2];

static uint8_t* box_break;
static uint32_t box_break_length;

static uint8_t* player_ouch;
static uint32_t player_ouch_length;

static uint8_t* mob_hurt;
static uint32_t mob_hurt_length;

static uint8_t* mob_bite;
static uint32_t mob_bite_length;

static uint8_t* door_open;
static uint32_t door_open_length;

static uint8_t* bonus;
static uint32_t bonus_length;

static uint8_t* key_found;
static uint32_t key_found_length;

static uint8_t* coins;
static uint32_t coins_length;

static uint8_t* game_over;
static uint32_t game_over_length;

static uint8_t* music;
static uint32_t music_length;
static uint8_t is_music_on=0;
static uint8_t *music_pos;
static uint32_t music_len;


static void load_steps(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_STEP1, &wav_spec,&(steps_p[0]), &(steps_length[0])) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_STEP2, &wav_spec,&(steps_p[1]), &(steps_length[1])) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_STEP3, &wav_spec,&(steps_p[2]), &(steps_length[2])) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_STEP4, &wav_spec,&(steps_p[3]), &(steps_length[3])) != NULL );
}

static void free_steps(){
	SDL_FreeWAV(steps_p[0]);
	SDL_FreeWAV(steps_p[1]);
	SDL_FreeWAV(steps_p[2]);
	SDL_FreeWAV(steps_p[3]);
}
static void load_entity_sounds(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_PLAYER_OUCH, &wav_spec,&(player_ouch), &(player_ouch_length)) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_MOB_HURT, &wav_spec,&(mob_hurt), &(mob_hurt_length)) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_MOB_BITE, &wav_spec,&(mob_bite), &(mob_bite_length)) != NULL );
}

static void free_entity_sounds(){
	SDL_FreeWAV(player_ouch);
	SDL_FreeWAV(mob_hurt);
	SDL_FreeWAV(mob_bite);
}

static void load_boxes(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_BOX_SCRAPE1, &wav_spec,&(scrap[0]), &(scrap_length[0])) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_BOX_SCRAPE2, &wav_spec,&(scrap[1]), &(scrap_length[1])) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_BOX_BREAK, &wav_spec,&box_break, &box_break_length) != NULL );
}
static void free_boxes(){
	SDL_FreeWAV(scrap[0]);
	SDL_FreeWAV(scrap[1]);
	SDL_FreeWAV(box_break);
}

static void load_bombs(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_PLACE_BOMB,&wav_spec,&place, &place_length ) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_EXPLOSION,&wav_spec, &explosion, &explosion_length) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_FUSE, &wav_spec,&fuse, &fuse_length) != NULL );
}

static void free_bombs(){
	SDL_FreeWAV(place);
	SDL_FreeWAV(explosion);
	SDL_FreeWAV(fuse);
}
static void load_world_sounds(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_DOOR_OPEN,&wav_spec,&door_open, &door_open_length ) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_KEY_FOUND,&wav_spec, &key_found, &key_found_length) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_BONUS, &wav_spec,&bonus, &bonus_length) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_COINS, &wav_spec,&coins, &coins_length) != NULL );
	assert(SDL_LoadWAV(WAV_PATH_GAME_OVER, &wav_spec,&game_over, &game_over_length) != NULL );

}

static void free_world_sounds(){
	SDL_FreeWAV(door_open);
	SDL_FreeWAV(bonus);
	SDL_FreeWAV(key_found);
	SDL_FreeWAV(coins);
}

static void load_music(){
	SDL_AudioSpec wav_spec;
	assert(SDL_LoadWAV(WAV_PATH_MUSIC,&wav_spec,&music, &music_length ) != NULL );
	music_pos=music;
	music_len=music_length;

}
static void free_music(){
	SDL_FreeWAV(music);
}

static void audio_load(){
	load_steps();
	load_bombs();
	load_boxes();
	load_entity_sounds();
	load_world_sounds();
	load_music();
}
void audio_free(){
	free_bombs();
	free_steps();
	free_boxes();
	free_entity_sounds();
	free_world_sounds();
	free_music();
	SDL_FreeWAV(audio_initial_pos);
}
void audio_init(){
	SDL_AudioSpec wav_spec;
	uint8_t *wav_buffer;
	uint32_t wav_length;
	audio_load();
	assert(SDL_LoadWAV(WAV_PATH_START,&wav_spec,&wav_buffer,&wav_length)!=NULL);
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	audio_initial_pos=wav_buffer;
	audio_pos=wav_buffer;
	audio_len=wav_length;
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  fflush(stderr);
	  exit(-1);
	}
	SDL_PauseAudio(0);

}

void audio_music_switch(){
	is_music_on = !is_music_on;
}
int audio_add_sound(enum audio_type type ){
	uint8_t* tmp;
	uint32_t tmp_length;

	uint8_t *wav_buffer;
	uint32_t wav_length;
	uint8_t rd;
	switch (type)
	{	
		case AUDIO_BOX_BREAK:
			wav_buffer=box_break;
			wav_length=box_break_length;
			break;
		case AUDIO_BOX_SCRAP:
			rd=rand()%2;
			wav_buffer=scrap[rd];
			wav_length=scrap_length[rd];
			break;
		case AUDIO_FUSE:
			wav_buffer=fuse;
			wav_length=fuse_length;
			break;
		case AUDIO_EXPLOSION:
			wav_buffer=explosion;
			wav_length=explosion_length;
			break;
		case AUDIO_BOMB_PLACE:
			wav_buffer=place;
			wav_length=place_length;
			break;
		case AUDIO_STEP:
			rd=rand()%5;
			if (rd>3){
				return(0);
			} else {
				wav_buffer=steps_p[rd];
				wav_length=steps_length[rd];
			}
			break;
		case AUDIO_PLAYER_OUCH:
			wav_buffer=player_ouch;
			wav_length=player_ouch_length;
			break;
		case AUDIO_MOB_HURT:
			wav_buffer=mob_hurt;
			wav_length=mob_hurt_length;
			break;
		case AUDIO_MOB_BITE:
			wav_buffer=mob_bite;
			wav_length=mob_bite_length;
			break;
		case AUDIO_DOOR_OPEN:
			wav_buffer=door_open;
			wav_length=door_open_length;
			break;
		case AUDIO_KEY_FOUND:
			wav_buffer=key_found;
			wav_length=key_found_length;
			break;
		case AUDIO_BONUS:
			wav_buffer=bonus;
			wav_length=bonus_length;
			break;
		case AUDIO_COINS:
			wav_buffer=coins;
			wav_length=coins_length;
			break;
		case AUDIO_GAME_OVER:
			wav_buffer=game_over;
			wav_length=game_over_length;
			break;
		default:
			return(1);
			break;
	}
	
	if (audio_len<wav_length){
		tmp_length=(32768>wav_length ? 32768: wav_length ) ;
		tmp= (uint8_t*) malloc(tmp_length);
		SDL_memset(tmp,0,tmp_length);
		SDL_MixAudio(tmp,audio_pos,audio_len,SDL_MIX_MAXVOLUME);
		SDL_MixAudio(tmp,wav_buffer,wav_length,SDL_MIX_MAXVOLUME);
		SDL_FreeWAV(audio_initial_pos);
		audio_initial_pos=tmp;
		audio_pos=tmp;
		audio_len=tmp_length;
	}
	else {
		SDL_MixAudio(audio_pos,wav_buffer,wav_length,SDL_MIX_MAXVOLUME);
	}
	return(0);
}
void audio_close(){
	SDL_PauseAudio(1);
	SDL_CloseAudio();
	is_playing=0;
}
void my_audio_callback(void *userdata, uint8_t *stream, int len) {
	int lentmp;
	if (is_music_on){
		if (music_len==0){
			music_pos=music;
			music_len=music_length;
		}
		if (audio_len==0){
			len = ( len > music_len ? music_len : len );
			SDL_memcpy (stream, music_pos, len);
			music_pos += len;
			music_len -= len;
		}
		else{
			lentmp = ( music_len > audio_len ? audio_len : music_len  );
			len = ( len > lentmp ? lentmp : len );
			SDL_memcpy (stream, audio_pos, len);
			SDL_MixAudio(stream,music_pos,len,SDL_MIX_MAXVOLUME);
			music_pos += len;
			music_len -= len;
			audio_pos += len;
			audio_len -= len;
		}
	}
	else{
		if (audio_len ==0 ){
			return;
		}
		len = ( len > audio_len ? audio_len : len );
		SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other

		
		audio_pos += len;
		audio_len -= len;
	}

}