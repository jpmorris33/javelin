/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//		Mark 2 DPMI Javelin Engine	: Header file for support routines			//
//                                                                         //
//				Javelin V2.3  - Copyright (C) J.P. Morris 1995                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#ifndef __JAV_INC
#define __JAV_INC

#define VER "2.31"
#define VNUM 0x400	// Jug Version no. 0x100-0xF00 in steps of 0x100
#define SPRSIZE 512

#include "itg/itg.hpp"
#include "compat.h"
#include "graphics/irekeys.hpp"

//extern unsigned char *screen;
extern unsigned char *swap_screen;
extern unsigned char *spare_screen;
extern unsigned char *bg_screen;
extern unsigned char *fg_screen;
extern char rootname[];
extern char jugname[];

extern unsigned short (*conn)[512][4];

extern char diags,skip_on;
extern char fade_ovr,f_red,f_green,f_blue;
extern char JUMP,LIVES,INFLIV;

extern SPRITE sp[SPRSIZE];
extern SPRITE tbg[4];
extern SPRITE tfg[15];
extern SPRITE sfont[192];
//extern SPRITE icons[20];
extern SPRITE HELP1,HELP2;
extern char ambsnd[256];

extern unsigned short *backing;
extern short *levels[512];
extern short lev[6][10];
extern unsigned short levsize;
extern int no_of_sprites;
extern short no_of_mons;
extern int no_of_objs;
extern int no_of_rooms;
extern unsigned int vc;			// validation check
extern palette pal,pog,redpal;
extern unsigned char *giflib[32];
extern int libsize;
extern char (*bfont)[256][8];

struct MoNtYpE
	{
	unsigned int ident;		// The ID sprite (as shown in editor)
	unsigned int alist;		// Animation list L (start)
	unsigned int alist2;	// Animation list R (start)
	char alength;			// length of anim.
	char deadly;		// 1 is deadly
	char lift;			// 1 is lift
	unsigned char itemno;	// 0 if not collectable, else item no.
	char spare;		// for later
	};

struct TuTl
	{
	unsigned char XY;
	unsigned char mtype;
	char dx,dy;
	};
extern struct TuTl (*mons)[512][12];

struct SG_struct
	{
	char name[30];
	char lives;
	short l;
	char sjs;
	short sx;
	short sy;
	char gotit[255];
	char no_of_clothes;
	};

extern struct TuTl mon2b[12];
extern struct MoNtYpE *mt;
extern char sts[1024];
extern char disregard;

extern void T_error(char *errm,long number);
extern void G_error(char *errm,long number);
extern void S_error(char *errm,char *errm2);
extern void ST_error(char *errm);
extern void Black_Spot(FILE *fp);
extern void game(int los);
extern int menu();
void setup_screen(int l);
void printn(unsigned long thenum,int x,int y);
int test_BG_hit(int x,int y,int s);
extern "C" void block_and(int length,unsigned char *source,unsigned char *dest);
extern "C" int collide(int length,unsigned char *source);
extern "C" void scroll(unsigned char *screen,unsigned char *fontaddr,int fontoffs);
extern "C" void zmov(unsigned char *in,unsigned char *out);
extern char *message;

int test_FG_hit(int x,int y,int s,int t,int l);
void do_star(int x,int y,int c);
void demo();
extern FILE *JUG4load(const char *jugfile,const char *entryname,long *len);
extern FILE *load(char *entry);
void loadGIF(char *entry,unsigned char *dest, palette pal);

#endif

#define BACKING	0
#define SPRITES	1
#define LEVELS		2
#define PALETTES	3
#define TEXT		4
#define HELP		5
#define FONT		6

#define BLACK 0
