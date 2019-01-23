//
//	sound.h - audio functions
//

#ifdef USE_SDL2SOUND

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

struct SMTab
{
    char *name;
	char *fname;
	Mix_Chunk *sample;
    char nodrift;
	char loopsong;
};

extern struct SMTab *wavtab;
extern struct SMTab *mustab;

// functions

extern int  S_Init();              // Start the sound engine
extern void S_Term();              // Stop the sound engine
extern void S_Load();              // Load the sound and music
extern void S_SoundVolume(int v);        // Set the Sound volume
extern void S_MusicVolume(int v);        // Set the Music volume
extern void S_PlaySample(char *s,int v); // Play a sound sample
extern void S_PlayMusic(char *s);       // Play a song
extern void S_PollMusic();               // Keep it ticking over
extern void S_StopMusic();               // Stop the music
extern int S_IsPlaying();                // Is music playing?

extern int StreamSong_start(char *filename); // Directly stream audio
extern void StreamSong_stop();

#endif // USE_SDLSOUND
