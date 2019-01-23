
#define MAXWAVS 255
#define MAXSONGS 25
#define SND_PLAYER 0
#define SND_AMBIENT 1

extern void S_Init();
extern void S_Update();
extern void S_Term();
extern void S_PlaySong(int song);
extern void S_StopSong();
extern int S_LoadSong(FILE *fp, long length);
extern void S_SetMusicVol(int vol);
extern void S_SetLoop(int loopEnabled);
extern int S_LoadWave(FILE *fp, long length);
extern void S_PlayWave(int wave, int channel);
extern void S_Fadeout();
extern void S_SetFXVol(int vol);
extern void S_SetMasterVol(int vol);
