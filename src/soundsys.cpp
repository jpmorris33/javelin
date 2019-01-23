#
#include <unistd.h>
#include <stdio.h>

#include <mikmod.h>
#include <SDL2/SDL.h>

#include "soundsys.h"
#include "compat.h"

#define MAXWAVS 255
#define MAXSONGS 25

static MODULE *cursong = NULL;
static volatile int running = 0;
static SAMPLE *wave[MAXWAVS];
static int waves=0;
static MODULE *song[MAXSONGS];
int no_of_tunes=0;
char songname[MAXSONGS][25];
static int cursfxvol = 0;
extern FILE *JUG4load(const char *jugfile,const char *entryname,long *len);

static int update(void *ptr) {
	while(running)	{
		MikMod_Update();
		SDL_Delay(3);
	}
	return 0;
}


void S_Init() {
	MikMod_RegisterAllDrivers();
	MikMod_RegisterAllLoaders();
	md_mode |= DMODE_HQMIXER;
	if(MikMod_Init("")) {
		printf("Mikmod startup failed\n");
		puts(MikMod_strerror(MikMod_errno));
		exit(1);
	}

	MikMod_SetNumVoices(-1, 2);
	MikMod_EnableOutput();

	running = 1;
	SDL_CreateThread(update, "audioengine", (void *)NULL);
}


void S_Update() {
}

void S_Term() {
	Player_Stop();
	running = 0;
	SDL_Delay(500);
	MikMod_DisableOutput();

	for(int ctr=0;ctr<waves;ctr++) {
		Sample_Free(wave[ctr]);
	}
	waves=0;

	for(int ctr=0;ctr<no_of_tunes;ctr++) {
		Player_Free(song[ctr]);
	}
	no_of_tunes=0;

	MikMod_Exit();
}

int S_LoadSong(FILE *fp, long length) {
	int pos = no_of_tunes;

	if(!fp) {
		return -1;
	}

	song[pos] = Player_LoadFP(fp,16,0);
	fclose(fp);

	if(!song[pos]) {
		return -1;
	}
	song[pos]->wrap=1; // Loop the song
	strcpy(songname[pos],song[pos]->songname);

	no_of_tunes++;

	return pos;

}

void S_PlaySong(int number) {
	S_StopSong();

	if(number >= no_of_tunes) {
		return;
	}
	cursong = song[number];
	if(cursong) {
		Player_Start(cursong);
		Player_SetPosition(0);
	}
}


void S_StopSong() {
	Player_Stop();
}

void S_Loop(int flag) {
	if(cursong) {
		cursong->wrap = flag;
	}
}

void S_SetMusicVol(int vol) {
	float v = (vol / 100.0) * 255.0;
	md_musicvolume = (int)v;
	//printf("Music volume now %d\n", (int)v);
}

void S_Fadeout() {
int curvol = md_musicvolume;
for(int vctr=0;vctr<curvol;vctr++)
        {
	SDL_Delay(5);
        S_SetMusicVol(curvol-vctr);
        }

S_StopSong();
S_SetMusicVol(curvol);
}



int S_LoadWave(FILE *fp, long length) {
	int pos = waves;
	char *buffer=NULL;

	if(!fp) {
		return -1;
	}
	buffer=(char *)calloc(1,length);
	if(!buffer) {
		fclose(fp);
		return -1;
	}
	fread(buffer,1,length,fp);
	fclose(fp);
	wave[pos] = Sample_LoadMem(buffer, length);
	free(buffer);

	if(!wave[pos]) {
		printf("Could not load the sound, reason: %s\n",
		MikMod_strerror(MikMod_errno));
		return -1;
	}

	waves++;
	return pos;
}

void S_PlayWave(int no, int channel) {
	int v,flags=0;

	if(no >= waves) {
		printf("Sound %d/%d out of range\n", no, waves);
		return;
	}

	if(!wave[no]) {
		return;
	}

	if(channel == SND_PLAYER) {
		flags = SFX_CRITICAL;
	}

	v = Sample_Play(wave[no],0,flags);
	Voice_SetVolume(v, cursfxvol);
	Voice_SetPanning(v,PAN_CENTER);
}

void S_SetFXVol(int vol) {
	float v = (vol / 100.0) * 255.0;
	cursfxvol = (int)v;
	//printf("SFX volume now %d\n", (int)v);
}
