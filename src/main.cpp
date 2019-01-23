/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//		Mark 2 DPMI Javelin Engine      : Startup and Support routines			//
//                                                                         //
//				Javelin V2.10  - Copyright (C) J.P. Morris 1995                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

//
//      FUTURE
//
//Music modules and sound effects
//Speech system for the B52
//

//	Components are as follows:
//
//	jav1.cpp			startup and shutdown
//	jav2.cpp		 	main game functions
// itg16.lib	 	graphics system
// its1.obj		 	collision detection
// gate.c		 	gateway to sound server
// message.cpp		scrolly message
// scrolla.asm		message scroller
// gr2.c				keyboard driver
// gr_int9.c		keyboard interrupt handler
//
// also necessary are the following #defines, stored in the project file
// LOCATION="Joe's digs in Bristol";			where javelin was built
// ITG16LOC="d:\ithe\src\itg\16\itg.hpp";	path to the ITG


#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
//#include <dos.h>
#include <string.h>
//#include <alloc.h>
//#include <io.h>
//#include "gr.h"

#include "itg/itg.hpp"

#include "javelin.hpp"

#define VER "2.31"
#define VNUM 0x400      // Jug Version no. 0x100-0xF00 in steps of 0x100
#define SPRSIZE 512

#include "soundsys.h"

int Jload_RSD();

unsigned _stklen =32700;

struct
{
	long ID;
	char dev;
	char freq;
	int mode;
	char ems;
	char vol;
	char sfx;
} setups;

char square,jll=0;
unsigned char *swap_screen;
unsigned char *spare_screen;
unsigned char *bg_screen;
unsigned char *fg_screen;
char rootname[]="j16";
char jugname[128];
char diags=1,loadmem=1;
char fade_ovr,f_red,f_green,f_blue,skip_on,invul;
char JUMP,LIVES,INFLIV;
char *message;
extern char inv;
palette HPAL1,HPAL2;
char disregard=0,windows=0;
unsigned char curvol=50;
unsigned char sfxvol=100;

int _argc = 0;
char **_argv = NULL;

char (*bfont)[256][8];

SPRITE sp[SPRSIZE];
SPRITE tbg[4];
SPRITE tfg[15];
//SPRITE icons[20];
SPRITE sfont[192];
SPRITE HELP1,HELP2;

char ambsnd[256];
unsigned short (*conn)[512][4];
unsigned short *backing;
unsigned char *giflib[32];
extern long surveygif();
int libsize;
short *levels[512];
short lev[6][10];
unsigned short levsize;
unsigned int vc;                        // validation check
palette pal,pog,redpal;
int no_of_sprites=0;
short no_of_mons=512;
int no_of_objs=512;
int no_of_rooms=512;
void btos();
void quote();
void step();
FILE *load(char *entry);
void JUG4loadall(const char *jugfile);
void loadGIF(const char *entry,unsigned char *dest,palette pal);
void load_waffle();
extern char gr_keys[128];
char *bingetstr(FILE *fp);
void loadc(char *fn,SPRITE *cell);
char *juglist[25];

struct TuTl (*mons)[512][12];
struct SG_struct savegame;

struct MoNtYpE *mt;
char sts[1024];


extern void setup_options();
extern int memGIF(unsigned char *src,unsigned char *dest,palette pal,unsigned int len2);
void setup();
void oscli();
int OSCLI_tryfor(char param[]);
void T_error(char *errm,long number);
void G_error(char *errm,long number);
void S_error(char *errm,char *errm2);
void ST_error(char *errm);
extern void play747(char *filename);
extern void rec747(char *filename);
static void read_monsters(FILE *fp, long length);


//int down();
//int out();

void writesetup()
{
FILE *fp;

if(!setups.ID)
	return;

fp=fopen("javelin.cfg","wb");


setups.sfx=sfxvol;
if(!setups.sfx)		// if the volume 0, it gets set back to 75!
	setups.sfx=1;
setups.vol=curvol;
if(!setups.vol)		// if the volume 0, it gets set back to 75!
	setups.vol=1;

if(fp)
	{
	fwrite(&setups,1,sizeof(setups),fp);
	fclose(fp);
	return;
	}
else
	setups.vol=75;
return;
}

void readsetup()
{
FILE *fp;
int res;
fp=fopen("javelin.cfg","rb");

setups.ID=0;
setups.vol=0;
if(fp)
	{
	res=fread(&setups,1,sizeof(setups),fp);
	fclose(fp);
	curvol=setups.vol;
	sfxvol=setups.sfx;
	return;
	}
if(!setups.vol)
	setups.vol=75;
if(!setups.sfx)
	setups.sfx=48;
curvol=setups.vol;
sfxvol=setups.sfx;
return;
}

int main(int argc, char **argv)
{
_argc=argc;
_argv=argv;

puts("Init os");
char choice=0;
FILE *fp;

oscli();
puts("Os ok");

choice=0;
setup();

puts("setup ok");

readsetup();
puts("Rsetup ok");

on();
//if(square)
//	outportb(0x3c2,0xe3);

S_SetMusicVol(curvol);
S_SetFXVol(sfxvol);
S_PlaySong(0);

fp=load("changes.fli");
if(fp) {
	fclose(fp);
	FP_play_flic(load("changes.fli"),swap_screen);
	IRE_ClearKeyBuf();
	}
FP_play_flic(load("prologue.fli"),swap_screen);
IRE_ClearKeyBuf();

FP_play_flic(load("intro.fli"),swap_screen);
IRE_ClearKeyBuf();

FP_play_flic(load("story.fli"),swap_screen);
IRE_ClearKeyBuf();


do	{
	choice=menu();
	switch(choice)
		{
		case 1:
		game(0);
		break;

		case 4:
		game(1);
		break;

		case 2:
//		demo();
		break;
		}
}while (choice!=3);

S_Fadeout();
//off();
writesetup();

if(loadmem)
	for(int ctr=0;ctr<libsize;ctr++)
		if(giflib[ctr]!=NULL)
			free(giflib[ctr]);

}

void oscli()
{
if(OSCLI_tryfor("diags"))                       // engage diagnostics display
	diags=1;
else
	diags=0;

if(OSCLI_tryfor("h")||OSCLI_tryfor("help")||OSCLI_tryfor("?"))
	{
	clrscr();
//	printf("16 bit DPMI Javelin - version %s\n",VER);
	printf("Javelin, linux port - version %s\n",VER);
//	printf("\nCompiled at %s, on %s at %s\n",LOCATION,__DATE__,__TIME__);
	printf("by Joseph Morris of IT-HE Software\n");
	printf("This version of the ITG16 was compiled on %s at %s.\n\n",date_stamp,time_stamp);
	printf("You might try the following:\n\n");
	printf("\tjavelin -h \t\t or\n");
	printf("\tjavelin -help\t\t gives this screen\n\n");
	printf("\tjavelin -fade R(nn) G(nn) B(nn) \t\tto set the fade colour:\n\n");
	printf("\t\t e.g. javelin -fade 30 0 0 \t (for fans of DOOM)\n");
	printf("\t\t or   javelin -fade 63 63 45 \t (for a nasty cream colour)\n");
	printf("\t\t     (javelin -fade 666 666 666 is really good!)\n\n");
	printf("\tjavelin -noload\t\tTo stop javelin loading backgrounds into memory\n");
	printf("\tjavelin -square\t\tIf you want square pixels\n");
	printf("\tjavelin -winjav\t\tIf Windows can't take it\n");
	printf("\tjavelin @[jugfile]\tTo add external JUG file (see FAQ)\n");
	printf("\n");
	exit(1);
	}

if(OSCLI_tryfor("i_love_satan_and_all_his_little_wizards"))
	{
	clrscr();
	printf("16 bit DPMI Javelin - version %s\n",VER);
	printf("by Joseph Morris of IT-HE Software\n\n");
	printf("\n\n");
	printf("Ancient and evil cheat menu:\n\n");
	printf("\tjavelin PURIFY!\tInfinite lives\n");
	printf("\tjavelin ANIMUS!\tInvulnerable\n");
	printf("\tjavelin SINNER! <no of lives>\n");
	printf("\tjavelin EXALT!  <jump height>\n");
	printf("\tjavelin HERESY!\tskip levels\n");
	printf("\n\nThose who have wisdom will know that something important is missing..\n");
	exit(1);
	}

fade_ovr=OSCLI_tryfor("fade");
if(fade_ovr)
	{
	f_red=atoi(_argv[fade_ovr+1]);
	f_green=atoi(_argv[fade_ovr+2]);
	f_blue=atoi(_argv[fade_ovr+3]);
	}

square=OSCLI_tryfor("square");
windows=OSCLI_tryfor("winjav");

skip_on=OSCLI_tryfor("-HERESY!");

JUMP=OSCLI_tryfor("-EXALT!");
if(JUMP)
	{
	JUMP=atoi(_argv[JUMP+1]);
	if(JUMP<0)
		JUMP=127;
	}
else
	JUMP=12;


LIVES=OSCLI_tryfor("-SINNER!");
if(LIVES)
	{
	LIVES=atoi(_argv[LIVES+1]);
	if(LIVES<0)
		LIVES=9;
	}
else
	LIVES=9;

if(OSCLI_tryfor("-PURIFY!"))
	INFLIV=1;
else
	INFLIV=0;

if(OSCLI_tryfor("-noload"))
	loadmem=0;
else
	loadmem=1;

if(OSCLI_tryfor("-ANIMUS!"))
	inv=1;
else
	inv=0;
}

void setup()
{
int ctr,res;
char fn[128];
FILE *fp;

strcpy(jugname,rootname);
strcat(jugname,".jug");


for(ctr=0;ctr<25;ctr++)
	{
	juglist[ctr]=(char *)malloc(128);
	if(!juglist[ctr])
		{
		puts("Need MUCH more memory than that.");
		exit(1);
		}
	}

int xctr=0;
for(ctr=0;ctr<_argc;ctr++)
	if(_argv[ctr][0]=='@')
		if(strlen(_argv[ctr])>1)
			{
			fp=fopen(_argv[ctr],"rb");
			if(fp)
				strcpy(juglist[xctr++],&_argv[ctr][1]);
			else
				fclose(fp);
			}

strcpy(juglist[xctr++],jugname);

if(diags)
	{
	printf("\n");
	for(int ctr=0;ctr<=xctr;ctr++)
		printf("%s,",juglist[ctr]);
	printf("\b \n");
   getch();
	}

jll=xctr;

off();
clrscr();
//textbackground(BLACK);
//textcolor(BLACK);
printf("JAVELIN %s STARTUP:\r\n",VER);

randomize();
quote();
//gotoxy(x1,y1);

printf("-0x%lx bytes available\n\r",coreleft());
S_Init();

printf("-Allocating buffers:[%c%c%c%c%c%c]\b\b\b\b\b\b\b",1,1,1,1,1,1);


//delay(100);

//screen=(char *)MK_FP(__SegA000,0);
//if(!screen)
//	T_error("Out of memory. Core remaining :",coreleft());

spare_screen=(unsigned char *)malloc(64000L);
if(!spare_screen)
	T_error("Out of memory. Core remaining :",coreleft());
step();

//delay(100);

swap_screen=(unsigned char *)malloc(64000L);
if(!swap_screen)
	T_error("Out of memory. Core remaining :",coreleft());
step();
//delay(100);

bg_screen=(unsigned char *)malloc(64000L);
if(!bg_screen)
	T_error("Out of memory. Core remaining :",coreleft());
step();

//delay(100);


fg_screen=(unsigned char *)malloc(64000L);
if(!fg_screen)
	T_error("Out of memory. Core remaining :",coreleft());
step();

//delay(100);

for(ctr=0;ctr<4;ctr++)
	if(!tbg[ctr].allocate(32,32))
		T_error("Out of memory. Core remaining :",coreleft());


for(ctr=0;ctr<15;ctr++)
	if(!tfg[ctr].allocate(32,32))
		T_error("Out of memory. Core remaining :",coreleft());

step();
//delay(100);

load_waffle();

printf("\r\n-Allocating Level memory:[%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\b",
												  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
									  printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
backing=(unsigned short *)calloc(512,2);
if(!backing)
		T_error("¥íç ¤¢£Ÿ M’m§R˜. Core remaining :",coreleft());

mons=(struct TuTl (*)[512][12])malloc((512*12)*sizeof(TuTl));
if(!mons)
	T_error("¥íç ¤¢£Ÿ M’m§R˜. Core remaining :",coreleft());

bfont=(char (*)[256][8])calloc(256,8);
if(!bfont)
		T_error("¥íç ¤¢£Ÿ M’m§R˜. Core remaining :",coreleft());

conn=(unsigned short(*)[512][4])calloc(512,8);
if(!conn)
		T_error("¥íç ¤¢£Ÿ M’m§R˜. Core remaining :",coreleft());

for(ctr=0;ctr<512;ctr++)
	{
	levels[ctr]=(short *)malloc(sizeof(lev));
	if(!levels[ctr])
		T_error("¥íç ¤¢£Ÿ M’m§R˜. Core remaining :",coreleft());
	if(ctr%12==1)
		step();
	}

	mt=(struct MoNtYpE *)malloc(sizeof(MoNtYpE)*1024);
if(!mt)
	{
	T_error("Out of memory. Core remaining :",coreleft());
	exit(1);
	}


printf("\n\r-Initialising Jugfile:[%c%c%c%c%c%c%c%c%c%c%c%c]\b\b\b\b\b\b\b\b\b\b\b\b\b",1,1,1,1,1,1,1,1,1,1,1,1);

JUG4loadall(jugname);

step();

fp=load("hpa.dfl");
if(!fp)
	{
	printf("\n\n");
	printf("URK: hpa.dfl is missing.\n");
	exit(1);
	}
else
	{
	res=fread(HPAL1,768,1,fp);
	res=fread(HPAL2,768,1,fp);
	fclose(fp);
	}

step();

loadc("help1.cel",&HELP1);
step();
loadc("help2.cel",&HELP2);
step();
if(!HELP1.spr||!HELP2.spr)
	T_error("Not enough memory. Core remaining :",coreleft());

no_of_sprites=Jload_RSD();
if(!no_of_sprites)
	T_error("Not enough memory. Core remaining :",coreleft());

//if(!Jload_RSD(font,"font1.rsd","font #1"))

/*if(!Jload_RSD(icons,"icons.rsd","icons"))
	T_error("Not enough memory. Core remaining :",coreleft());*/


long zlen;

	fp=load("joe.fnt");
	if(!fp)
	T_error("Cannot load font. Core remaining :",coreleft());
	res=fread((*bfont),1,2048,fp);
	fclose(fp);

btos();

if(diags)
	printf("[%ld bytes used in %d entries]\n",zlen,libsize);

if(loadmem)
	{
	zlen=surveygif();
	if(zlen>coreleft())
		{
		printf("\n\r\rNot enough memory to store all backgrounds.\n");
		printf("Backgrounds will be loaded as they are needed.\n");
		loadmem=0;
		}
	}

if(loadmem)
	{
	printf("\nLoading backgrounds:[");
	for(ctr=0;ctr<libsize;ctr++)
		printf("%c",1);
	printf("]");
	for(ctr=0;ctr<libsize+1;ctr++)
		printf("\b");

	for(ctr=0;ctr<libsize;ctr++)
		{
		itoa(ctr,fn,10);
		strcat(fn,".gif");
		giflib[ctr]=(unsigned char *)calloc(64768L,1);
		if(!giflib[ctr])
			T_error("Memory estimate failure. Use the -noload switch. free memory: ",coreleft());
//		fp=load(fn);
		loadGIF(fn,giflib[ctr]+768,giflib[ctr]);
//			S_error("Error loading background:",fn);
		step();
		}
	}

if(!fade_ovr)
	for(ctr=0;ctr<=255;ctr++)
		{
		redpal[ctr*3]=63;
		redpal[ctr*3+1]=63;
		redpal[ctr*3+2]=63;
		}
else
	for(ctr=0;ctr<=255;ctr++)
		{
		redpal[ctr*3]=f_red;
		redpal[ctr*3+1]=f_green;
		redpal[ctr*3+2]=f_blue;
		}
//getch();
//for(ctr=0;ctr<15;ctr++) __M_delay(1);
}

void T_error(char *errm,long number)
{
if(number==-1)
	printf("\n\nInteresting.. %s.\n",errm);
else
	printf("\n\nInteresting.. %s %ld.\n",errm,number);
exit(1);
}

void S_error(char *errm,char *errm2)
{
printf("\n\nInteresting.. %s '%s'.\n",errm,errm2);
exit(1);
}

void ST_error(char *errm)
{
printf("\n\nInteresting.. %s.\n",errm);
exit(1);
}

void G_error(char *errm,long number)
{
off();
if(number==-1)
	printf("Interesting.. %s.\n",errm);
else
	printf("Interesting.. %s %ld.\n",errm,number);
exit(1);
}

int OSCLI_tryfor(char param[])
{
int ctr;
char tp[1024];

//strlwr(param);
strcpy(tp,param);
for(ctr=1;ctr<_argc;ctr++)
	if(strcmp(tp,_argv[ctr])==0)
		return(ctr);

strcpy(tp,"-");
strcat(tp,param);
for(ctr=1;ctr<_argc;ctr++)
	if(strcmp(tp,_argv[ctr])==0)
		return(ctr);

strcpy(tp,"/");
strcat(tp,param);
for(ctr=1;ctr<_argc;ctr++)
	if(strcmp(tp,_argv[ctr])==0)
		return(ctr);

strcpy(tp,"\\");
strcat(tp,param);
for(ctr=1;ctr<_argc;ctr++)
	if(strcmp(param,_argv[ctr])==0)
		return(ctr);

return 0;
}

void btos()
{
for(int ctr=0;ctr<192;ctr++)
	{
	if(!sfont[ctr].allocate(8,8))
		T_error(" Not enough memory. Core remaining :",coreleft());

	for(int ctr4=0;ctr4<7;ctr4++)
		{
		if((*bfont)[ctr][ctr4+1]&128)
			sfont[ctr].spr[ctr4*8]=0;
		else
			sfont[ctr].spr[ctr4*8]=144;

		if((*bfont)[ctr][ctr4+1]&64)
			sfont[ctr].spr[(ctr4*8)+1]=0;
		else
			sfont[ctr].spr[(ctr4*8)+1]=144;

		if((*bfont)[ctr][ctr4+1]&32)
			sfont[ctr].spr[(ctr4*8)+2]=0;
		else
			sfont[ctr].spr[(ctr4*8)+2]=144;

		if((*bfont)[ctr][ctr4+1]&16)
			sfont[ctr].spr[(ctr4*8)+3]=0;
		else
			sfont[ctr].spr[(ctr4*8)+3]=144;

		if((*bfont)[ctr][ctr4+1]&8)
			sfont[ctr].spr[(ctr4*8)+4]=0;
		else
			sfont[ctr].spr[(ctr4*8)+4]=144;

		if((*bfont)[ctr][ctr4+1]&4)
			sfont[ctr].spr[(ctr4*8)+5]=0;
		else
			sfont[ctr].spr[(ctr4*8)+5]=144;

		if((*bfont)[ctr][ctr4+1]&2)
			sfont[ctr].spr[(ctr4*8)+6]=0;
		else
			sfont[ctr].spr[(ctr4*8)+6]=144;

		if((*bfont)[ctr][ctr4+1]&1)
			sfont[ctr].spr[(ctr4*8)+7]=0;
		else
			sfont[ctr].spr[(ctr4*8)+7]=144;
		}
	for(int ctr4=56;ctr4<64;ctr4++)
		sfont[ctr].spr[ctr4]=144;

	}
}

void step()
{
if(kbhit())
	if(getch()==27)
		{
		gotoxy(1,24);
		exit(1);
		}
printf("%c",2);
}

void quote()
{
int ctr=0;
char *quotelist[]={	"\"..at least you two are working on your project, unlike Concorde over there..\"\n\n\t - Ray Burcham, New college",
							"\"I would not have given it for a wilderness of monkeys.\"\n\n\t - The Merchant of Venice, Shakespeare",
							"Martyn: \"What the hell happened to that sheep? Is it dead..?\"\nJoseph: \"No, it's just gone mindless.\"",
							"\"I've used up all my logic for today.\"\n\n\t - Andrew 'logic' North",
							"\"You may have got rid of the sheep on the SCREEN, but they're still knocking\n about in the memory.\"\n\n\t - James Grant",
							"\"What is a country? A country is a piece of land surrounded on all sides\n by boundaries, usually unnatural. Englishmen are dying for England,\n Americans are dying for America, Germans are dying for Germany,\n Russians are dying for Russia. There are now fifty or sixty countries\n fighting in this war. Surely so many countries can't ALL be worth dying for!\"\n\n\t Catch-22, Joseph Heller",
							"\"We could enter the IBM competition, sure, and you could send Javelin to them,\n but we'll be competing against the people who wrote JAZZ JACKRABBIT\n and SECOND REALITY.\"\n\n\t - Martyn Smart",
							"\"Beeeeeeehh.\"\n\n\t - E.T.",
							"\"I think you should add some welshmen to chase the sheep, saying 'TAFTAFTAFTAF'\"\n\t - Steve Bindon\n\n\"No way.\"\n\n\t - Joseph Morris",
							"\"I have only one purpose, the destruction of Hitler, and my life is much\n simplified thereby. If Hitler invaded Hell I would at least make a favourable\n reference to the Devil in the House of Commons.\"\n\n\t - Winston Churchill",
							"\"Have you ever designed a room that YOU can't complete?\"\n\n\t - Paul 'microns' Brown: Network Supervisor, New College",
							"\"Tom, would you like to see an elephant?\"\n\n\t - THE STAND, Stephen King",
							"\"Don't buy that Joe, it's got more bugs than your EGA game.\"\n\n\t - James Grant",
							"\"Aw, have you been replacing DOS with your Javelin game again?\"\n\n\t - James Grant",
							"\"Do you realise that since we set up this demonstration, the word 'IT-HE'\n appears more times in this room than the word 'Intel'?\n We should rename it the IT-HE learning resource centre.\"\n\n\t - Joseph Morris",
							"\"I'm gonna fry your brains out, boy, would you like hotwax with THAT?\"\n\n\t - THE STAND, Stephen King",
							"James Grant:   \"Do you kill the pigs yourselves?\"\nSteve Bindon:  \"No,we send them off to a farmer, and he sends us the sausages.\"\nJoseph Morris: \"How do you know that the sausages are YOUR pig?\"\nSteve Bindon:  \"Because no other pig would taste as good as our Betty.\"\n\n",
							"\"Begin at the Beginning, and go on till you come to the end: then stop.\"\n\n\t - Through the Looking Glass, Lewis Carroll\n\n",
							"\"It will always be possible to crash the system.\"\n\n\t - Paul Brown\n\n",
							"\"If there's no Marmite, use Vegemite: if there's no Vegemite, use honey.\"\n\n\t - Anonymous\n\n",
							"Have you found the secret message in the title screen yet?\n\n",
							"\"The wonderful thing about Continental Drift is that America is getting\n further away all the time...\"\n\n\t - John Little\n\n",
							"JOSEPH:  \"Let's face it: we just can't draw a big native.\"\nBRENDAN: \"Wouldn't it be good to scan someone in and turn them into a native?\"\nJOSEPH:  \"Yes! But who?\"\nArthur enters the room...\n\n",
							"\"Have some more of my beautiful drink, guards, for I have killed King Duncan\n and now I must wipe his blood on you.\"\n\n\t - Macbeth (fun size editon)\n\n",
							"\"This Protected Mode isn't very well protected, is it?\"\n\n\t - Joseph Morris\n\n",
							"\"A bird in hand is worth two in the bush, UNLESS you are holding a sparrow\n and the bush contains a pair of live Dodos.\"\n\n",
							"TALES FROM THE USENET #1\n\n> > >   first  problem  is I want to know many games can make from ?\n> > >   second is i know c,c++.Will i make games like DOOM.\n> >\n> > Where do they grow these people?\n>\n> Dont worry, we've isolated the problem and it should be fixed in version 3.62\n",
							"\"Everything different is good.\"\n\n\t - Groundhog Day\n\n",
							"TALES FROM THE USENET #2\n\n> > > from .VOX and .WAV sound files?  Preferrably it'd be PD or\n> > > shareware,\n> > do you mean .VOC - the Creative Labs sound sample? .VOX is the type\n> > of sample file used in programming telephone exchanges...\n> OOOPS! I misread - sorry! Yes, I meant VOC.\nWhat the hell?  I posted the original question and I meant VOX.  Get your\nown question, buddy, instead of screwing with mine!",
							"\"Pork and beans, pork and beans, Holy God, pork and beans!\"\n\n\t - Next of Kin, Eric Frank Russell\n\n",
							"\"Ah, but God has two left feet on his right ankle.\"\n\n\t - 'God' Shakespeare\n\n",
							"\"As one approaches the speed of light, ones inhibitions shrink to zero.\"\n\n\t - Next of Kin, Eric Frank Russell\n\n",
							"So they poured out for the men to eat.\nAnd it came to pass, as they were eating of the pottage, that they cried out,\nand said, O thou man of God, there is death in the pot.  And they could not eat\nthereof.  But he said, Then bring meal.  And he cast it into the pot; and he\nsaid, pour out for the people, that they may eat.  And there was no harm in\nthe pot.\n\n\tThe second book of Kings, Chapter 4, verses 40-41\n",
							"\"Earth is more than 98% full.  Please delete all unnecessary people from\n your user town.\"\n",
							"*"};

for(ctr=0;quotelist[ctr][0]!='*';ctr++);

/*for(int ctr2=0;ctr2<ctr;ctr2++)
	{
	clrscr();
	puts("QUOTE OF THE DAY:\n");
	printf("\n\n\n\n\n\n\n\n\n%s\n\n",quotelist[ctr2]);
	getch();
	}*/

gotoxy(1,16);

printf("%s\n",quotelist[getrandom(ctr)]);
}

FILE *JUG4load(const char *jugfile,const char *entryname,long *len)
{
unsigned short vc;
char entry[15];
FILE *fp;
int32_t fptr;


fp=NULL;
memset(entry,0,15);

for(int xctr=0;xctr<jll;xctr++)
	{
//	printf("JLL: %s\n",juglist[xctr]);
	if(fp)
   		fclose(fp);
	fp=fopen(juglist[xctr],"rb");
	if(!fp)
		return NULL;
	vc=getw16(fp);
	if(vc!=0x7074+0x400)
		{
		S_error("Unsupported JUG version",NULL);
		return NULL;
		}
	do {
//		printf("read entry from %ld\n",ftell(fp));
		if(!fread(entry,1,13,fp)) {
//			puts("XX");
			break;
		}
//		printf("E> %s\n",entry);
		for(vc=0;vc<14;vc++)
			if(entry[vc]<33)
				entry[vc]=0;
		if(!fread(&fptr,4,1,fp)) {
//			puts("X");
			return NULL;
	        }
		*len=fptr;
//		printf(">entry = '%s', entryname = '%s'\n", entry, entryname);
//		printf("Next item = %d, %x\n",fptr,fptr);
		if(!strcmpi(entry,entryname)) {
			return(fp);
		}
		fseek(fp,fptr,SEEK_CUR);
//		printf("file pointer now %ld\n",ftell(fp));
//		printf("feof says %d\n",feof(fp));
//			return NULL;
		} while(!feof(fp));
	*len=0;
	}
return NULL;
}

void JUG4loadall(const char *jugfile)
{
FILE *fp;
long length=1;
int res;
char filename[64];

	fp=JUG4load(jugfile,"sizes.dfl",&length);
	if(fp)
		{
		no_of_mons=getw16(fp);
		no_of_objs=getw16(fp);
		no_of_rooms=getw16(fp);
		fclose(fp);
		}

	step();	// 1

	fp=JUG4load(jugfile,"monsters.dfl",&length);
	if(fp)
		{
		read_monsters(fp, length);
		fclose(fp);
		}

	step();	// 2

	fp=JUG4load(jugfile,"solidity.dfl",&length);
	if(fp)
		{
		res=fread(sts,no_of_objs,1,fp);
		fclose(fp);
		}

	step();	// 3

	fp=JUG4load(jugfile,"connect.dfl",&length);
	if(fp)
		{
/*		for(int ctr=0;ctr<no_of_rooms;ctr++)
			{
			//move_sheep(0);
			fread(conn[ctr],4,2,fp);
			}*/
		res=fread(conn,4*no_of_rooms,2,fp);
		fclose(fp);
		}

	step();	// 4

	fp=JUG4load(jugfile,"backing.dfl",&length);
	if(fp)
		{
		res=fread(backing,no_of_rooms,2,fp);
		fclose(fp);
		}

	step();	// 5

	fp=JUG4load(jugfile,"palette.col",&length);
	if(fp)
		{
		res=fread(pal,length,1,fp);
		fclose(fp);
		}

	step();	// 6

	fp=JUG4load(jugfile,"rooms.lev",&length);
	if(fp)
		{
		for(int ctr=0;ctr<no_of_rooms;ctr++)
			{
			//move_sheep(0);
			res=fread(levels[ctr],1,sizeof(lev),fp);
			}
		fclose(fp);
		}

	step();	// 7

	fp=JUG4load(jugfile,"monsters.lev",&length);
	if(fp)
		{
		res=fread(mons,1,/*sizeof(TuTl)*12*no_of_mons*/length,fp);
		fclose(fp);
		}

	step();	// 8

	fp=JUG4load(jugfile,"sounds.dfl",&length);
	if(fp)
		{
		res=fread(ambsnd,1,length,fp);
		fclose(fp);
		}

	for(int ctr=0;ctr<MAXSONGS;ctr++) {
		sprintf(filename, "%d.MOD", ctr);
		fp=JUG4load(jugfile, filename, &length);
		S_LoadSong(fp,length);
	}

	for(int ctr=0;ctr<MAXWAVS;ctr++) {
		sprintf(filename, "%d.WAV", ctr);
		fp=JUG4load(jugfile, filename, &length);
		if(fp) {
			if(S_LoadWave(fp,length) == -1) {
				printf("Error loading sound %d\n");
			}
		}
	}

}

FILE *load(char *entry)
{
FILE *fp;
long length;

fp=JUG4load(jugname,entry,&length);
if(fp)
	return fp;

// scan other JUG files later..

return NULL;
}

void load_waffle()
{
FILE *fp;
long length;
int res;

fp=JUG4load(jugname,"WAFFLE.DFL",&length);
if(!fp)
	ST_error("Something important is missing.");
message=(char *)malloc(length);
if(!message)
		T_error("Out of memory. Core remaining :",coreleft());
res=fread(message,length,1,fp);
fclose(fp);
step();
}

void loadGIF(char *entry,unsigned char *dest, palette pal)
{
FILE *fp;
long length;
int res;

fp=JUG4load(jugname,entry,&length);
if(fp)
	{
	res=fread(bg_screen,length,1,fp);
	fclose(fp);
	memGIF(bg_screen,dest,pal,length);
	return;
	}

// scan other JUG files later..

/*fp=JUG4load(the_filename,entry,&length);
if(fp)
	{
	fread(bg_screen,length,1,fp);
	fclose(fp);
	memGIF(bg_screen,dest,pal,length);
	return;
	}*/

S_error("Can not load picture",entry);
//S_error("Can not load from ",GetRjugFile(entry));
return;
}

int Jload_RSD()
{
int ctr;//,w,h;
int quantity;
char qdiv=0;
int qd10;
//unsigned int I1;
char ID[12];
char CMP[]={"SAVEtheB52s"};
FILE *fp;
long length;
int res;

fp=JUG4load(jugname,"sprites.rsd",&length);
if(!fp)
	T_error("Sprite entry not found.",-1);
res=fread(ID,strlen(CMP)+1,1,fp);
if(strcmp(ID,CMP))
	T_error("Sprites must be saved in an RSD file.",-1);
quantity=getw16(fp);
quantity=no_of_mons;
printf("\nLoading sprites:[");
//fseek(fp,+4L,SEEK_CUR);

qdiv=1;
if(quantity>60)
	qdiv=2;
if(quantity>120)
	qdiv=3;
if(quantity>180)
	qdiv=4;
if(quantity>240)
	qdiv=5;
if(quantity>300)
	qdiv=6;

qd10=quantity/qdiv;

for(ctr=0;ctr<qd10;ctr++)
	printf("%c",1);
printf("]");
for(ctr=0;ctr<qd10;ctr++)
	printf("\b");
printf("\b");

qd10=0;

for(ctr=0;ctr<quantity;ctr++)
	{
//	for(ctr=0;ctr<no_of_mons;ctr++)
//		{
		sp[ctr].WxH=getw16(fp);

		if(!sp[ctr].allocate(32,32))
			return 0;
		res=fread(sp[ctr].spr,1024,1,fp);
		if(kbhit())
			if(getch()==27)
				{
				gotoxy(1,24);
				exit(1);
				}
		qd10++;
		if(qd10==qdiv)
			{
			printf("%c",2);
			qd10=0;
			}
//		}

	}
fclose(fp);
printf("\r");
return quantity;
}

void loadc(char *fn,SPRITE *cell)
{
FILE *fp;
unsigned int ID;
int w,h,res;

fp=load(fn);
if(!fp)
	{
	S_error("Argh! HELPless",NULL);
	return;
	}

ID=getw16(fp);

if(ID!=0x9119)
	{
	fclose(fp);
	S_error("Not a CEL file.",NULL);
	return;
	}

w=getw16(fp);
h=getw16(fp);

fseek(fp,794L,SEEK_CUR);

if(!cell->allocate(w,h))
	S_error("Out of memory.",NULL);

res=fread(cell->spr,w*h,1,fp);
fclose(fp);
return;
}

#define MTSIZE 11

// The original game read all the structs in directly with fread
// Do something a bit more compiler-agnostic
// This still won't work on big-endian, fix if that becomes a problem

void read_monsters(FILE *fp, long length) {
	unsigned char *block = (unsigned char *)calloc(1,length);
	if(block == NULL) {
		G_error("Error allocating block for monsters", length);
	}

	unsigned char *blockptr = block;
	unsigned short value;
	int res = fread(block,1,length,fp);
	if(res < length) {
		G_error("Internal error reading monsters", res);
	}

	int num_mons = length / MTSIZE;  // I think that's right...
	for(int ctr=0;ctr<num_mons;ctr++) {
		memcpy(&value,blockptr,2);
		blockptr+=2;
		mt[ctr].ident = value;

		memcpy(&value,blockptr,2);
		blockptr+=2;
		mt[ctr].alist = value;

		memcpy(&value,blockptr,2);
		blockptr+=2;
		mt[ctr].alist2 = value;

//		memcpy(&value,blockptr,2);
//		blockptr+=2;
		mt[ctr].alength = *blockptr++;

//		memcpy(&value,blockptr,2);
//		blockptr+=2;
		mt[ctr].deadly = *blockptr++;

//		memcpy(&value,blockptr,2);
//		blockptr+=2;
		mt[ctr].lift = *blockptr++;

//		memcpy(&value,blockptr,2);
//		blockptr+=2;
		mt[ctr].itemno = *blockptr++;

//		memcpy(&value,blockptr,2);
//		blockptr+=2;
		mt[ctr].spare =*blockptr++;
		if(mt[ctr].spare != 0) {
//			printf("Monster %d has spare flag of %d\n",ctr,mt[ctr].spare);
//			G_error("That may not be right", mt[ctr].spare);
		}

	}

	free(block);

//		res=fread((struct MoNtYpE *)&mt[0],length/*sizeof(struct MoNtYpE)*no_of_mons*/,1,fp);
}

