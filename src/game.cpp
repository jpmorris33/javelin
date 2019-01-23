/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//     Mark 2 DPMI Javelin Engine : Game loop and support                  //
//                                                                         //
//     Javelin V2.0  - Copyright (C) J.P. Morris 1995                      //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

/*

		Development History of Javelin DPMI MkII

		Started main game code 1/2/95
		80% working on 10/2/95
		90% working on 19/2/95

		Demo in 1996, final version Sept 1999

		Linux port 19-22nd Jan 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <io.h>
#include <fcntl.h>
#include <ctype.h>

#include "soundsys.h"
#include "demo.h"
#include "itg/itg.hpp"
#include "spira.idf"
#include "javelin.hpp"

int framerate=NULL;

pattern disintegrate = {
	0x01,0x76,0x17,0x63,0x23,0x59,0x29,0x81,0x55,0x04,
	0x97,0x95,0x30,0x51,0x25,0x67,0x83,0x37,0x21,0x89,
	0x43,0x90,0x46,0x75,0x32,0x64,0x78,0x06,0x70,0x12,
	0x94,0x58,0x34,0x72,0x10,0x54,0x28,0x74,0x48,0x50,
	0x66,0x14,0x19,0x92,0x08,0x88,0x45,0x03,0x57,0x61,
	0x27,0x41,0x68,0x22,0x87,0x52,0x99,0x60,0x38,0x62,
	0x36,0x20,0x82,0x56,0x49,0x86,0x15,0x79,0x93,0x31,
	0x05,0x91,0x33,0x77,0x24,0x85,0x35,0x80,0x16,0x11,
	0x73,0x44,0x18,0x96,0x40,0x13,0x00,0x69,0x07,0x84,
	0x26,0x42,0x98,0x02,0x71,0x47,0x65,0x09,0x39,0x53 };
extern char loadmem,curvol,sfxvol,windows;
extern palette HPAL1;
extern palette HPAL2;
char lives;
int ptr=1;
int mx[12],my[12],no_m=0,hx,hy;
char mdx[12],mdy[12],direct[12],direct2[12],aframe[12];
char inv;
char gotit[256];
char clpos[]={76,85,94,49,40,31,22,112,103};
char string[128];

char code,rx;
char **menulist;

extern struct SG_struct savegame;

/*struct HUD		// Strictly, Head-Up-Displays are obsolete, as all relevant
	{					// data from the aircraft's sensory perception is added
	int x,y;			// directly into the short-term memory - it just KNOWS.
	long number;
	void create(int x1,int y1,long number1);
	void destroy();
	void maintain();
	} hud[2];*/

char no_of_clothes;

extern char songname[25][25];
void maintain_monsters(int l);
//void maintain_huds();
void print(char x,char y,const char *prt,unsigned char *scr,unsigned char fc,unsigned char bc);
void print2(int x,int y,const char *prt,unsigned char *scr,unsigned char fc,unsigned char bc);
int save();
int load();
int sure();
void soundsetup();
void getsong();
unsigned char get_name();
void putcode747(char code);
char getcode747();
void play747(char *filename);
void rec747(char *filename);
void notify(char *one,char *two);
char* get_str(char *bufx,int lenn);
int scroll(char length,char width);
int savegame_load(const char *filename);
void savegame_save(const char *filename);
int savegame_loadname(const char *filename);
int savegame_loadDOS(const char *filename);
int savegame_loadnameDOS(const char *filename);

extern char no_of_tunes;
char vctr;

void game(int los)
{
int x,y,ox,oy,l=0,man_shape=0,w_ctr=0;
//int music_on=1;
int sx=48,sy=136,sjs=0;
char js=0;
unsigned char t1,choice;
char cycle=0;
int res,ctr;

SPRITE sback;
FILE *fp;
char alive=1;
unsigned int speed=50000;
clock_t start;
lives=LIVES;
rx=0;

S_Fadeout();
S_PlaySong(2);
//on();
memset(gotit,0,256);
no_of_clothes=0;

//gr_start_kbd_grab();

code=0;

while(lives>0&&lives!=-1) {

	if(los==1) {
		rgb(255,13,13,13);
		rgb(254,14,16,18);
		rgb(253,24,24,24);
		choice=load();
		if(choice!=11) {
			strcpy(string,"javsave.00x");
			string[10]='0'+choice;

			if(savegame_load(string)) {
//		fp=fopen(string,"rb");
//		res=fread(&savegame,sizeof(struct SG_struct),1,fp);
//		res=fread((struct TuTl (*)[512][12])mons,1,sizeof(TuTl)*12*no_of_mons,fp);
//		fclose(fp);

				lives=savegame.lives;
				l=savegame.l;
				sjs=savegame.sjs;
				sx=savegame.sx;
				sy=savegame.sy;
				memcpy(gotit,savegame.gotit,256);
				no_of_clothes=savegame.no_of_clothes;
				setup_screen(l);
				x=sx;
				y=sy;
			}
		}
		IRE_ClearKeyBuf();
		los=0;
	}

	setup_screen(l);
	x=sx;
	y=sy;
	ox=x;
	oy=y;
	js=sjs;
	tbg[0].get_sprite(ox,oy,swap_screen);


	alive=1;
	while(alive&&alive!=2)
		{
		cycle++;
		if(cycle>8)
			cycle=0;

		tbg[0].block_put_sprite(ox,oy,swap_screen);

		if(los==2)
			putcode747(code);
		code=0;
		if(los==3)
			code=getcode747();

		if(IRE_TestKey(IREKEY_Q))
			{
			alive=0;
			if(los==2)
				code=8;
			}

/*
		// DEMO stuff, refactor
		if(los>1)
			if(code&8||(los==3&&idx747>demolen))
				{
				alive=2;
				lives=0;
				}
*/
		if(IRE_TestKey(IREKEY_ESC))
			if(sure())
				{
				alive=2;
				lives=0;
				if(los==2)
					code=8;
				}


	if(speed!=0)
		{
		start=clock();
		while (clock()-start< speed);
		}

	if(skip_on&&IRE_TestKey(IREKEY_LCONTROL))
		{
		if(IRE_TestKey(IREKEY_PGUP)&&l<255)
			{
			l++;
			setup_screen(l);
			sx=288;
			sy=y;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}

		if(IRE_TestKey(IREKEY_PGDN)&&l>0)
			{
			l--;
			setup_screen(l);
			sx=288;
			sy=y;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}

		printn(l,112,0);
		}

		if(IRE_TestKey(IREKEY_TAB))
			{
			if(IRE_TestKey(IREKEY_UP)&&speed>0)
				speed--;
			if(IRE_TestKey(IREKEY_DOWN)&&(unsigned)speed<65535U)
				speed++;

			fbox2(64,0,128,8,BLACK,swap_screen);
			printn(speed,80,0);
			}

		maintain_monsters(l);

		if((IRE_TestKey(IREKEY_RIGHT)&&los!=3)||code&2) {
			if(los==2)
				code+=2;

			if(!test_BG_hit(x+8,y,w_ctr)) {
				x+=4;
				t1=point(x+27,y+31,swap_screen);
				if(t1>=244&&t1<=250)
					y=y-4;
				if(!js)	{
					man_shape=8+w_ctr;
					w_ctr++;
					if(w_ctr>7)
						w_ctr=0;
				} else
					man_shape=14;
			}
		}

		if(x>288&&(*conn)[l][1]!=l)
			{
			l=(*conn)[l][1];
			setup_screen(l);
			sx=4;
			sy=y;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}
		else
			if(x>288)
				x=288;

		if(x<0&&(*conn)[l][0]!=l)
			{
			l=(*conn)[l][0];
			setup_screen(l);
			sx=288;
			sy=y;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}
		else
			if(x<0)
				x=0;

		if(y<4&&(*conn)[l][2]!=l)
			{
			l=(*conn)[l][2];
			setup_screen(l);
			sx=x;
			sy=160;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}
		else
			if(y<4)
				y=4;

		if(y>164&&(*conn)[l][3]!=l)
			{
			l=(*conn)[l][3];
			setup_screen(l);
			sx=x;
			sy=8;
			x=sx;
			y=sy;
			ox=x;
			oy=y;
			sjs=js;
			tbg[0].get_sprite(ox,oy,swap_screen);
			}

		if(y>164)
			{
			js=0;
			y=164;
			}

//		maintain_monsters(l);

//		printf("Looking for key %d\n", IREKEY_LEFT);

		if((IRE_TestKey(IREKEY_LEFT)&&los!=3)||code&1)
			{
			if(los==2)
				code+=1;
			if(!test_BG_hit(x-8,y,w_ctr+8))
				{
				x-=4;
				t1=point(x+18,y+31,swap_screen);
				if(t1>=244&&t1<=250&&js==-1)
					y=y-4;

				if(!js)
					{
					man_shape=w_ctr;
					w_ctr++;
					if(w_ctr>7)
						w_ctr=0;
					}
				else
					man_shape=6;
				}

			}

		if(((IRE_TestKey(IREKEY_UP)&&los!=3)||code&4)&&!IRE_TestKey(IREKEY_TAB)&&js==0)
			{
//			callgate(DWAV1,1);
			S_PlayWave(0,SND_PLAYER);
			js=JUMP;
			if(los==2)
				code+=4;
			}

		if(js>0)
			{
			if(!test_BG_hit(x,y-4,man_shape))
				y-=4;
			else
				if(js!=1) js=2;	// cutout the jump if he bangs his head (by popular demand)
			js--;
			}

		if(!test_BG_hit(x,y+4,man_shape)&&js==0)
			js=-1;

		if(point(x+14,y+32,swap_screen)>250||point(x+16,y+32,swap_screen)>250||point(x+18,y+32,swap_screen)>250)
			if(js>0)
				js=1;
			else
				js=0;

		if(point(x+16,y+31,swap_screen)==243||point(x+14,y+31,swap_screen)==243||
				point(x+19,y+31,swap_screen)==243)
			alive=0;

		t1=point(x+23,y+35,swap_screen);
		if(t1>=244&&t1<=250&&js==-1)
			js=0;

		if(point(x+16,y+31,swap_screen)==242||point(x+14,y+31,swap_screen)==242||
				point(x+19,y+31,swap_screen)==242)
		if(cycle&&js==-1)
			js=0;
/*
		if(diags)
			{
			dot(x+18,y+32,30,screen);
			fbox2(258,0,62,8,BLACK,swap_screen);
			printn(coreleft(),258,0);
			tbg[1].block_put_sprite(10,10,swap_screen);
			tbg[2].block_put_sprite(48,10,swap_screen);
			fbox2(180,0,62,8,BLACK,screen);
			fbox2(170,0,62,8,BLACK,swap_screen);
//			printn(idx747,180,0);
			printn(no_of_clothes,230,0);
			}
*/
	for(int ctr=0;ctr<no_m;ctr++)
		if(mt[(*mons)[l][ctr].mtype].deadly==1)
		if(test_FG_hit(x,y,man_shape,ctr,l))
			alive=0;

	for(ctr=0;ctr<no_m;ctr++)
		if(mt[(*mons)[l][ctr].mtype].itemno>0&&mt[(*mons)[l][ctr].mtype].itemno<250)
		if(test_FG_hit(x,y,man_shape,ctr,l))
			{
			(*mons)[l][no_m].XY=255;
			tfg[ctr].block_put_sprite(mx[ctr],my[ctr],swap_screen);
			gotit[(*mons)[l][ctr].mtype]=1;
			no_of_clothes++;
			no_m--;
			S_PlayWave(2,SND_PLAYER);
			}

	for(ctr=0;ctr<no_m;ctr++)
		if(mt[(*mons)[l][ctr].mtype].lift==1||mt[(*mons)[l][ctr].mtype].spare==1)
		if(test_FG_hit(x,y+4,man_shape,ctr,l))
			{
			if(!test_BG_hit(x+mdx[ctr],y+4,man_shape))
				x+=mdx[ctr];
			if(!test_BG_hit(x,y+4+mdy[ctr],man_shape)&&js<1)
				{
				y+=mdy[ctr];
				js=0;
				}
			}

	for(ctr=0;ctr<no_m;ctr++)
		if((unsigned char)mt[(*mons)[l][ctr].mtype].itemno==255)
		if(test_FG_hit(x,y+4,man_shape,ctr,l)&&no_of_clothes==9)
			{
			dissolve(spira,5);
			alive=2;
			lives=-2;
			}

		if(js==-1)
			{
			if(!test_BG_hit(x,y+4,w_ctr))
				y+=4;
			else
				js=0;
			}

	if(IRE_TestKey(IREKEY_F11)) {
		blitToScreen(bg_screen);
		__M_delay(10);
	}

	if(los<2)
		{
		if(IRE_TestKey(IREKEY_F2))
			{
			choice=save();
			if(choice!=11)
				{
				savegame.lives=lives;
				savegame.l=l;
				savegame.sjs=sjs;
				savegame.sx=sx;
				savegame.sy=sy;
				memcpy(savegame.gotit,gotit,256);
				savegame.no_of_clothes=no_of_clothes;

				if(get_name()!=27)
					{
					strcpy(string,"javsave.00x");
					string[10]='0'+choice;

					savegame_save(string);

//					fp=fopen(string,"wb");
//					fwrite(&savegame,sizeof(struct SG_struct),1,fp);
//					fwrite((struct TuTl (*)[512][12])mons,1,sizeof(TuTl)*12*no_of_mons,fp);
//					fclose(fp);
					}
				}
			//blit(screen,swap_screen);
			blitToScreen(swap_screen);
			IRE_ClearKeyBuf();
			}

		if(IRE_TestKey(IREKEY_F3))
			{
			choice=load();
			if(choice!=11)
				{
				strcpy(string,"javsave.00x");
				string[10]='0'+choice;
				if(savegame_load(string)) {
					lives=savegame.lives;
					l=savegame.l;
					sjs=savegame.sjs;
					sx=savegame.sx;
					sy=savegame.sy;
					memcpy(gotit,savegame.gotit,256);
					no_of_clothes=savegame.no_of_clothes;
					setup_screen(l);
					x=sx;
					y=sy;
				}

//				fp=fopen(string,"rb");
//				res=fread(&savegame,sizeof(struct SG_struct),1,fp);
//				res=fread((struct TuTl (*)[512][12])mons,1,sizeof(TuTl)*12*no_of_mons,fp);
//				fclose(fp);

				}
//			blit(screen,swap_screen);
			IRE_ClearKeyBuf();
			blitToScreen(swap_screen);
			}
		}

if(IRE_TestKey(IREKEY_F4)) {
	soundsetup();
	blitToScreen(swap_screen);
	IRE_ClearKeyBuf();
//	blit(screen,swap_screen);
//	gr_keys[GR_KEY_ESC]=0;
	}


if(IRE_TestKey(IREKEY_F5)) {
	getsong();
//	blit(screen,swap_screen);
	blitToScreen(swap_screen);
//	gr_keys[GR_KEY_ESC]=0;
	IRE_ClearKeyBuf();
	}

//		maintain_monsters(l);

		if(y<0)		// Master Control Guard
			y=0;		// NeverNeverNever
		if(x>288)
			x=288;
//		if(y>164)
//			{
//			js=0;
//			y=164;	// NEVER NEVER NEVER NEVER NEVER
//			}

		tbg[0].get_sprite(x,y,swap_screen);
		sp[man_shape].cel_put_sprite(x,y,swap_screen);
		ox=x;
		oy=y;

if(IRE_TestKey(IREKEY_F1)) {
	blitFromScreen(spare_screen);

	hx=(320-HELP1.get_width())/2;
	hy=(200-HELP1.get_height())/2;
	HELP1.block_put_sprite(hx,hy,spare_screen);
	while(IRE_TestKey(IREKEY_F1)) {
		blitToScreen(spare_screen);
	}

	while(!IRE_TestKey(IREKEY_ENTER) && !IRE_TestKey(IREKEY_ESC)) {
		blitToScreen(spare_screen);
	}
	while(IRE_TestKey(IREKEY_ENTER) | IRE_TestKey(IREKEY_ESC)) {
		blitToScreen(spare_screen);
	}

	hx=(320-HELP2.get_width())/2;
	hy=(200-HELP2.get_height())/2;
	HELP2.block_put_sprite(hx,hy,spare_screen);
	for(int ctr3=0;ctr3<10;ctr3++)
		if(gotit[ctr3+10])
			fbox2(hx+5,hy+clpos[ctr3],5,5,78,spare_screen);

	while(!IRE_TestKey(IREKEY_ENTER) && !IRE_TestKey(IREKEY_ESC)) {
		blitToScreen(spare_screen);
	}

	while(IRE_TestKey(IREKEY_ESC));

}


if(lives!=-2)
	{
		blitToScreen(swap_screen); //blit(screen,swap_screen);
	}

	if(inv&&!alive)
		alive=1;
	};

//printf("INFLIV = %d\n", INFLIV);

if(LIVES>=-1&&!INFLIV)
	lives--;

	if(lives>0)
		{
		S_PlayWave(1, SND_PLAYER);
		pal_get(pog);
		pal_get(pal);
		do      {
			pal_set(pal);
			__M_delay(1);
//			delay(1);
			} while(fadeto(redpal,pal));
		memcpy(pal,pog,768);
		}

	};

//IRE_StopKeyboard();

if(lives<-1)
	{
//	fflush(stdin);
//	memset(gr_keys,0,127);
	IRE_ClearKeyBuf();
	FP_force_flic(load("finale.flx"),swap_screen);
	IRE_ClearKeyBuf();
//	memset(gr_keys,0,127);
//	getch();
//	getch();
//	memset(gr_keys,0,127);
	}

if(alive!=2)
	{
	//strcpy(string,"ded.gif");
	loadGIF("ded.gif",spare_screen,pog);
	IRE_ClearKeyBuf();
	mix(disintegrate,6,spare_screen, swap_screen);
//	memset(gr_keys,0,127);
	IRE_ClearKeyBuf();
	while(!kbhit());
//	getch();
//	getch();
//	memset(gr_keys,0,127);
	}
}

void setup_screen(int l)
{
int ctr,ctr2,c3;
clear(swap_screen);
clear(fg_screen);

//printf("Init screen %d, backing is %d\n",l,backing[l]);

if(backing[l]!=255)
	{
	itoa(backing[l],string,10);
	strcat(string,".gif");
	if(!loadmem)
		loadGIF(string,swap_screen,pal);
	else
		{
		memcpy(pal,giflib[backing[l]],768);
//		blit(swap_screen,giflib[backing[l]]+768);
		copyScreen(swap_screen, giflib[backing[l]]+768);
		}
	}
else
	{
	clear(swap_screen);
	for(ctr=0;ctr<75;ctr++)
	if(getrandom(2))
		dot(getrandom(319),getrandom(190),getrandom(15),swap_screen);
	else
		do_star(getrandom(318),getrandom(190),getrandom(7)+10);
	}

clear(bg_screen);

c3=0;

for(ctr2=0;ctr2<6;ctr2++)
	for(ctr=0;ctr<10;ctr++)
			{
			if(levels[l][c3]!=160) {
				sp[levels[l][c3]].cel_put_sprite(ctr*32,8+ctr2*32,swap_screen);
			}
			if(sts[levels[l][c3]])
				sp[levels[l][c3]].cel_put_sprite(ctr*32,8+ctr2*32,bg_screen);
			c3++;
			}

no_m=0;
for(ctr=0;ctr<8;ctr++)
	{
	if((*mons)[l][ctr].XY!=255&&!gotit[(*mons)[l][ctr].mtype])
			{
			mx[no_m]=(((*mons)[l][ctr].XY>>4)&0x0f)*32;
			my[no_m]=8+((*mons)[l][ctr].XY&0x0f)*32;
			direct[no_m]=(*mons)[l][ctr].dx;
			mdx[ctr]=direct[ctr];
			if(direct[no_m]>0)
				direct[no_m]=1;
			else
				direct[no_m]=0;

			direct2[no_m]=(*mons)[l][ctr].dy;
			mdy[ctr]=direct2[ctr];
			if(direct2[no_m]>0)
				direct2[no_m]=1;
			else
				direct2[no_m]=0;

			tfg[no_m].get_sprite(mx[no_m],my[no_m],swap_screen);
			aframe[no_m]=0;//mt[(*mons)[l][ctr].mtype].alist;
			no_m++;
			}

	}


if(lives!=0)
	{
	print(0,0,"LIVES ",swap_screen,16,0);
	//	icons[1].block_put_sprite(0,0,swap_screen);
	printn(lives,48,0);
//	printn(backing[l],160,0);
	}
//pal_set(pal);
//blit(screen,swap_screen);
xbp(NULL,swap_screen,pal);
}

int test_BG_hit(int x,int y,int s)
{
	int z=0;
	tbg[1].get_sprite(x,y,bg_screen);
	block_and(1024,sp[s].spr,tbg[1].spr);
	z=collide(1024,tbg[1].spr);
	return z;
}

int test_FG_hit(int x,int y,int s,int t,int l)
{
	int z=0;
	if(direct[t])
		sp[mt[(*mons)[l][t].mtype].alist2+aframe[t]].cel_put_sprite(mx[t],my[t],fg_screen);
	else
		sp[mt[(*mons)[l][t].mtype].alist+aframe[t]].cel_put_sprite(mx[t],my[t],fg_screen);

	tbg[2].get_sprite(x,y,fg_screen);
	block_and(1024,sp[s].spr,tbg[2].spr);
	z=collide(1024,tbg[2].spr);
	fbox2(mx[t],my[t],32,32,0,fg_screen);
	return z;
}

int test_BFG_hit(int t,int l)
{
	int z=0;
	if(direct[t])
		sp[mt[(*mons)[l][t].mtype].alist2+aframe[t]].cel_put_sprite(mx[t],my[t],fg_screen);
	else
		sp[mt[(*mons)[l][t].mtype].alist+aframe[t]].cel_put_sprite(mx[t],my[t],fg_screen);

	tbg[3].get_sprite(mx[t],my[t],bg_screen);
	if(direct[t])
		block_and(1024,sp[mt[(*mons)[l][t].mtype].alist2+aframe[t]].spr,tbg[3].spr);
	else
		block_and(1024,sp[mt[(*mons)[l][t].mtype].alist+aframe[t]].spr,tbg[3].spr);
	z=collide(1024,tbg[3].spr);
	fbox2(mx[t],my[t],32,32,0,fg_screen);
	return z;
}

void maintain_monsters(int l)
{
int ctr,t1=0;
static int cycle=0,ambctr=0;

if(backing[l]==6)
	{
	cycle++;
	if(cycle>6)
		pal_set(HPAL1);
	else
		pal_set(HPAL2);
	if(cycle>12)
		cycle=0;
	}

for(ctr=0;ctr<no_m;ctr++)
	{
	tfg[ctr].block_put_sprite(mx[ctr],my[ctr],swap_screen);
	}

for(ctr=0;ctr<no_m;ctr++)
	{
	if(!mt[(*mons)[l][ctr].mtype].spare)
		{
		mx[ctr]+=mdx[ctr];
		my[ctr]+=mdy[ctr];
		}
		if(my[ctr]<4)
			my[ctr]=0;
		if(my[ctr]>167)
			my[ctr]=167;
	if(mx[ctr]<283)
		t1=point(mx[ctr]+31,my[ctr]+31,swap_screen);

	///
	//
	//		THIS BASTARD IS THE CULPRIT
	//
	///


	if((mx[ctr]>284||mx[ctr]<0||(t1>=244&&t1<=250))&&!mt[(*mons)[l][ctr].mtype].spare)
		{
		mx[ctr]-=mdx[ctr];
		mdx[ctr]=-mdx[ctr];
		direct[ctr]=!direct[ctr];
		}
	if(mx[ctr]<283)
		if(test_BFG_hit(ctr,l))
			{
			mx[ctr]-=mdx[ctr];
			mdx[ctr]=-mdx[ctr];
			direct[ctr]=!direct[ctr];
			}



	if(my[ctr]>160||my[ctr]<4||(t1>=244&&t1<=250))
		{
		my[ctr]-=mdy[ctr];
		if(my[ctr]<4)
			my[ctr]=4;
		if(my[ctr]>167)
			my[ctr]=167;
		mdy[ctr]=-mdy[ctr];
		direct2[ctr]=!direct2[ctr];
		}
	else if(test_BFG_hit(ctr,l))
		{
		my[ctr]-=mdy[ctr];
		if(my[ctr]<4)
			my[ctr]=4;
		if(my[ctr]>167)
			my[ctr]=167;
		mdy[ctr]=-mdy[ctr];
		direct2[ctr]=!direct2[ctr];
		}


//      mox[ctr]=mx[ctr];
//      moy[ctr]=my[ctr];

	if(!framerate)
		{
		aframe[ctr]++;
		if(aframe[ctr]>=mt[(*mons)[l][ctr].mtype].alength)
			aframe[ctr]=0;
		}
	}

	if(ambctr++>50)	{
		ambctr=0;
		if(getrandom(5)&&no_m>2) {
			int wav = ambsnd[(*mons)[l][0].mtype];
			if(wav > 0) {
				S_PlayWave(wav-1, SND_AMBIENT);
			}
//			callgate(DWAV2,ambsnd[(*mons)[l][0].mtype]);
		}
	}


	if(!framerate)
		framerate=1;
	else
		framerate=0;

for(ctr=0;ctr<no_m;ctr++)
	{
	tfg[ctr].get_sprite(mx[ctr],my[ctr],swap_screen);
	}

for(ctr=0;ctr<no_m;ctr++)
	if(direct[ctr])
		sp[mt[(*mons)[l][ctr].mtype].alist2+aframe[ctr]].cel_put_sprite(mx[ctr],my[ctr],swap_screen);
	else
		sp[mt[(*mons)[l][ctr].mtype].alist+aframe[ctr]].cel_put_sprite(mx[ctr],my[ctr],swap_screen);
}

void do_star(int x,int y,int c)
{
	dot(x,y,25,swap_screen);
	dot(x+1,y,c,swap_screen);
	dot(x-1,y,c,swap_screen);
	dot(x,y+1,c,swap_screen);
	dot(x,y-1,c,swap_screen);
}

int sure()
{
int hgt,wdt,x,y,gc;

//blit(fg_screen,screen);
blitFromScreen(fg_screen);
blitFromScreen(spare_screen);

hgt=24;
wdt=128;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+8,y+3,"  Quit Game?",spare_screen,0,254);
print2(x+8,y+11," Press Y or N",spare_screen,0,254);

blitToScreen(spare_screen);

gc=0;
do
	{
	if(IRE_TestKey(IREKEY_Y)) {
		gc='Y';
	}
	if(IRE_TestKey(IREKEY_N)) {
		gc='N';
	}
	} while(gc!='N'&&gc!='Y');

//blit(screen,fg_screen);
blitToScreen(fg_screen);

if(gc=='N')
	{
	gc=NULL;
	clear(fg_screen);
	}

return(gc);
}

int load()
{
int hgt,wdt,x,y,gc,res;
FILE *fp;
char empty[11];

if(ptr>10)
	ptr=1;

hgt=96;
wdt=96;

x=160-(wdt/2);
y=100-(hgt/2);

blitFromScreen(spare_screen);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+12,y+3,"Load game:",spare_screen,0,254);

for(int ctr=0;ctr<10;ctr++)
	{
	strcpy(string,"javsave.00x");
	string[10]='0'+ctr;

	if(savegame_loadname(string)) {
		print2(x+12,y+15+(ctr*8),savegame.name,spare_screen,0,254);
		empty[ctr]=1;
		}
	else
		{
		print2(x+12,y+15+(ctr*8),"Empty Slot",spare_screen,0,254);
		empty[ctr]=0;
		}
	}

print2(x+4,y+7+(ptr*8),"~",spare_screen,0,254);

gc=0;
do
	{
//	gc=getch();
	gc=0;
	if(IRE_TestKey(IREKEY_UP))
		gc='u';
	if(IRE_TestKey(IREKEY_DOWN))
		gc='d';
	if(IRE_TestKey(IREKEY_ENTER))
		gc='y';
	if(IRE_TestKey(IREKEY_ESC))
		gc='n';

	if(gc)
		IRE_ClearKeyBuf();

	print2(x+4,y+7+(ptr*8)," ",spare_screen,254,254);
	if(gc == 'u' && ptr>1)
		ptr--;
	if(gc == 'd' && ptr<10)
		ptr++;
	if(gc == 'y' &&!empty[ptr-1])
		gc=0;
	print2(x+4,y+7+(ptr*8),"~",spare_screen,0,254);
	blitToScreen(spare_screen);
	} while(gc!='n'&&gc!='y');

if(gc=='n')
	ptr=12;

//blit(swap,backing);
return (ptr-1);
}

int save()
{
int hgt,wdt,x,y,gc,res;
FILE *fp;

blitFromScreen(spare_screen);

if(ptr>10)
	ptr=1;

hgt=96;
wdt=96;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+12,y+3,"Save game:",spare_screen,0,254);

//ptr=1;

for(int ctr=0;ctr<10;ctr++)
	{
	strcpy(string,"javsave.00x");
	string[10]='0'+ctr;

	if(savegame_loadname(string)) {
		print2(x+12,y+15+(ctr*8),savegame.name,spare_screen,0,254);
		}
	else
		{
		print2(x+12,y+15+(ctr*8),"Empty Slot",spare_screen,0,254);
		}
	}

print2(x+4,y+7+(ptr*8),"~",spare_screen,0,254);

do
	{
//	gc=getch();
	gc=0;
	if(IRE_TestKey(IREKEY_UP))
		gc='u';
	if(IRE_TestKey(IREKEY_DOWN))
		gc='d';
	if(IRE_TestKey(IREKEY_ENTER))
		gc='y';
	if(IRE_TestKey(IREKEY_ESC))
		gc='n';
	if(gc)
		IRE_ClearKeyBuf();
	
	print2(x+4,y+7+(ptr*8)," ",spare_screen,254,254);
	if(gc=='u'&&ptr>1)
		ptr--;
	if(gc=='d'&&ptr<10)
		ptr++;
	print2(x+4,y+7+(ptr*8),"~",spare_screen,0,254);
	blitToScreen(spare_screen);
	} while(gc!='y'&&gc!='n');

if(gc=='n')
	ptr=12;

	strcpy(string,"javsave.00x");
	string[10]='0'+ptr-1;
	if(!savegame_loadname(string))  {
		strcpy(savegame.name,"");
	}
//blit(swap,backing);
return (ptr-1);
}

void notify(char *one,char *two)
{
int hgt,wdt,x,y;

blitFromScreen(spare_screen);

hgt=64;
wdt=(strlen(one)*8)+16;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+8,y+3,one,spare_screen,0,254);
if(two)
	print2(x+8,y+12,two,spare_screen,0,254);

blitToScreen(spare_screen);
while(!IRE_TestKey(IREKEY_ENTER)) __M_delay(2);
}

unsigned char get_name()
{
int hgt,wdt,x,y,gc;
char buf[]="Name your save game:";
char buf_ptr=0;
char printbuf[]="x\0";

blitFromScreen(spare_screen);

hgt=(4)*8;
wdt=31*8;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+8,y+3,buf,spare_screen,0,254);

strcpy(buf,savegame.name);
buf_ptr=strlen(buf);

gc=0;
do      {
	if(gc == '\b') // trap the delete key
		{
		gc=0;
		if(buf_ptr>0)
			buf_ptr--;
		}

	if(buf_ptr>9&&gc!=13)    // stop entry getting too long
		gc=0;

	if(gc&&isprint(gc))		  // if the key pressed was printable
		buf[buf_ptr++]=gc;


fbox2(x+1,y+12,100,9,254,spare_screen);
for(int ctr=0;ctr<buf_ptr;ctr++)
	if(buf[ctr]>=' ')
		{
		printbuf[0]=buf[ctr];
		print2((ctr*8)+x+8,y+13,printbuf,spare_screen,0,254);
		}

	blitToScreen(spare_screen);
	gc=getch();
//	// FIXME
//	if(IRE_TestKey(IREKEY_ESC)) {
//		gc=27;
//	}

	} while(gc != 13 && gc != 3 && gc != 27);

buf[buf_ptr]=0;
strcpy(savegame.name,buf);
return(gc);
}

void printn(unsigned long thenum,int x,int y)
{
char number[17];
x=x/8;
y=y/8;
ltoa(thenum,number,10);
print(x,y,number,swap_screen,254,0);
}

void getsong()
{
int hgt,wdt,x,y,gc;
static int ptr=2;

hgt=176;
wdt=208;

x=160-(wdt/2);
y=100-(hgt/2);

blitFromScreen(spare_screen);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+12,y+3,"songs:",spare_screen,0,254);

for(int ctr=0;ctr<=no_of_tunes;ctr++)
//	{
	print2(x+12,y+15+(ctr*8),songname[ctr],spare_screen,0,254);
//	}

print2(x+4,y+15+(ptr*8),"~",spare_screen,0,254);

do
	{
	gc=0;
	if(IRE_TestKey(IREKEY_UP))
		gc='u';
	if(IRE_TestKey(IREKEY_DOWN))
		gc='d';
	if(IRE_TestKey(IREKEY_ENTER))
		gc='y';
	if(IRE_TestKey(IREKEY_ESC))
		gc='n';
	if(gc)
		IRE_ClearKeyBuf();

	print2(x+4,y+15+(ptr*8)," ",spare_screen,254,254);
	if(gc=='u'&&ptr>0)
		ptr--;
	if(gc=='d'&&ptr<no_of_tunes-1)
		ptr++;
	if(gc=='y')
		{
		S_SetMusicVol(0);
		S_StopSong();
		S_PlaySong(ptr);
		S_SetMusicVol(curvol);
		}

	print2(x+4,y+15+(ptr*8),"~",spare_screen,0,254);
	blitToScreen(spare_screen);
	} while(/*gc!=13&&*/gc!='n');

return;
}

void print(char x,char y,const char *ptr,unsigned char *scr,unsigned char fc,unsigned char bc)
{
char len;
int ctr,ctr2=0,ctr3=0,nx,ny;
unsigned char letter;

len=strlen(ptr);
for(ctr=0;ctr<len;ctr++)
	{
/*	if(prt[ctr]>=32)
		font[ptr[ctr]-32].block_put_sprite((x+ctr2)*8,(y+ctr3)*8,scr);*/
	nx=(x+ctr2)*8;
	ny=(y+ctr3)*8;
	if(bc) {
		fbox2(nx,ny,8,8,bc,scr);
	}
	letter = (unsigned char)ptr[ctr];
	for(int ctr4=0;ctr4<8;ctr4++)	{
		if((*bfont)[letter][ctr4]&128)
			dot(nx,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&64)
			dot(nx+1,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&32)
			dot(nx+2,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&16)
			dot(nx+3,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&8)
			dot(nx+4,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&4)
			dot(nx+5,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&2)
			dot(nx+6,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&1)
			dot(nx+7,ny+ctr4,fc,scr);

	}
	ctr2++;
	if(ctr2>39) {
		ctr2=0;
		ctr3++;
		}
	}
}

void print2(int x,int ny,const char *ptr,unsigned char *scr,unsigned char fc,unsigned char bc)
{
char len;
int ctr,nx;
unsigned char letter=0;

len=strlen(ptr);
nx=x;
for(ctr=0;ctr<len;ctr++)
	{
	if(bc) {
		fbox2(nx,ny,8,8,bc,scr);
	}
	letter = (unsigned char)ptr[ctr];
	for(int ctr4=0;ctr4<8;ctr4++) {
		if((*bfont)[letter][ctr4]&128)
			dot(nx,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&64)
			dot(nx+1,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&32)
			dot(nx+2,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&16)
			dot(nx+3,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&8)
			dot(nx+4,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&4)
			dot(nx+5,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&2)
			dot(nx+6,ny+ctr4,fc,scr);

		if((*bfont)[letter][ctr4]&1)
			dot(nx+7,ny+ctr4,fc,scr);
		}
	nx+=8;
	}
}

void soundsetup()
{
int hgt,wdt,x,y,gc,ptr,ptr1,ptr2;
unsigned char HptR[]="x\0";
unsigned char TracK[]="x\0";

hgt=56;
wdt=144;

TracK[0]=135;
HptR[0]=136;

x=160-(wdt/2);
y=100-(hgt/2);

blitFromScreen(spare_screen);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

//print2(x+20,y+3,"Sound setup:",screen,0,254);

print2(x+20,y+15,"Effects:",spare_screen,0,254);
hline(x+20,x+128,y+26,0,spare_screen);
print2(x+20,y+39,"Music:",spare_screen,0,254);
hline(x+20,x+128,y+50,0,spare_screen);

ptr1=sfxvol;
ptr2=curvol;
ptr=1;

print2(x+20+ptr1,y+23,(char *)HptR,spare_screen,0,254);
print2(x+20+ptr2,y+47,(char *)HptR,spare_screen,0,254);

do
	{

	if(ptr==1)
		{
		print2(x+12,y+15,"~",spare_screen,0,254);
		print2(x+12,y+39," ",spare_screen,0,254);
		}
	else
		{
		print2(x+12,y+15," ",spare_screen,0,254);
		print2(x+12,y+39,"~",spare_screen,0,254);
		}

	print2(x+20+ptr1,y+23,(char *)HptR,spare_screen,0,254);
	print2(x+20+ptr2,y+47,(char *)HptR,spare_screen,0,254);
	blitToScreen(spare_screen);

	gc=0;
	if(IRE_TestKey(IREKEY_UP)) {
		gc='H';
	}
	if(IRE_TestKey(IREKEY_DOWN)) {
		gc='P';
	}
	if(IRE_TestKey(IREKEY_LEFT)) {
		gc='K';
	}
	if(IRE_TestKey(IREKEY_RIGHT)) {
		gc='M';
	}
	if(IRE_TestKey(IREKEY_ESC)) {
		gc=27;
	}
	if(IRE_TestKey(IREKEY_ENTER)) {
		gc=13;
	}

	print2(x+20+ptr1,y+23,(char *)TracK,spare_screen,0,254);
	print2(x+20+ptr2,y+47,(char *)TracK,spare_screen,0,254);
	if(gc=='H')
		ptr=1;
	if(gc=='P')
		ptr=2;

	if(ptr==1)
		{
		if(gc=='K'&&ptr1>0)
			ptr1--;
		if(gc=='M'&&ptr1<100)
			ptr1++;
		}

	if(ptr==2)
		{
		if(gc=='K'&&ptr2>0)
			ptr2--;
		if(gc=='M'&&ptr2<100)
			ptr2++;
		}

	if(gc=='K'||gc=='M')
		{
		S_SetFXVol(ptr1);
		S_SetMusicVol(ptr2);
		S_PlayWave(0, SND_PLAYER);
		}

	if(gc)
		IRE_ClearKeyBuf();

	if(gc==13)
		gc=0;
	} while(gc!=13&&gc!=27);


sfxvol=ptr1;
curvol=ptr2;

//blit(swap,backing);
}

long surveygif()
{
//int handle;
FILE *fp;
long z_length;
char finish=0;

libsize=0;
z_length=0;
do {
	itoa(libsize,string,10);
	strcat(string,".gif");
/*	handle=open(string,O_RDONLY|O_BINARY);
	if(handle==-1)
		finish=1;
	else
		close(handle);*/
	fp=load(string);
	if(!fp)
		finish=1;
	else
   	fclose(fp);
	z_length+=64768L;
	libsize++;
	} while(!finish);
libsize--;
return z_length;
}

int scroll(char length,char width)
{
int hgt,wdt,x,y,ptr,sptr,gc;

if(length<20)
	hgt=(length+1)*8;
else
	hgt=21*8;

wdt=(width+2)*8;

x=160-(wdt/2);
y=100-(hgt/2);

rgb(255,13,13,13);
rgb(254,14,16,18);
rgb(253,24,24,24);

fbox2(x,y,wdt,hgt,254,swap_screen);
hline(x,x+wdt,y,253,swap_screen);
vline(x,y,y+hgt,253,swap_screen);
hline(x,x+wdt,y+hgt,255,swap_screen);
vline(x+wdt,y,y+hgt,255,swap_screen);

print2(x+12,y+3,menulist[0],swap_screen,0,254);

if(length<20)
	{
	for(int ctr=1;ctr<length;ctr++)
		print2(x+12,y+7+(ctr*8),menulist[ctr],swap_screen,0,254);

	ptr=1;


	print2(x+4,y+7+(ptr*8),"~",swap_screen,0,254);

	do
		{
//		blit(screen,swap_screen);
		blitToScreen(swap_screen);
		gc=getch();
		print2(x+4,y+7+(ptr*8)," ",swap_screen,0,254);
		if(gc=='H'&&ptr>1)
			ptr--;
		if(gc=='P'&&ptr<(length-1))
			ptr++;
		print2(x+4,y+7+(ptr*8),"~",swap_screen,0,254);
		} while(gc!=13&&gc!=27);

	if(gc==27)
		ptr=0;
	return ptr;
	}
else
	{
	for(int ctr=1;ctr<20;ctr++)
		print2(x+12,y+7+(ctr*8),menulist[ctr],swap_screen,0,254);

	ptr=1;
	sptr=1;

//	if(ptr<20)
		print2(x+4,y+7+(ptr*8),"~",swap_screen,0,254);
/*	else
		print(x+4,y+7+(19*8),"~",swap);*/

	do
		{
		blitToScreen(swap_screen);
//		blit(screen,swap_screen);
		gc=getch();
		if(sptr<20)
			print2(x+4,y+7+(sptr*8)," ",swap_screen,0,254);
//		else

		if(gc=='H'&&ptr>1)
			sptr--;

		if(gc=='P'&&ptr<(length-1))
			sptr++;

		if(ptr<19)
			ptr=sptr;

		if(sptr<1)
			{
			if(ptr>1)
			ptr--;
			sptr=1;
			}

		if(ptr<19)
			print2(x+4,y+7+(sptr*8),"~",swap_screen,0,254);
		else
			{
			fbox2(x,y,wdt,hgt,254,swap_screen);
			hline(x,x+wdt,y,253,swap_screen);
			vline(x,y,y+hgt,253,swap_screen);
			hline(x,x+wdt,y+hgt,255,swap_screen);
			vline(x+wdt,y,y+hgt,255,swap_screen);
			print2(x+12,y+3,menulist[0],swap_screen,0,254);

			if(sptr<20)
				print2(x+4,y+7+(sptr*8),"~",swap_screen,0,254);
			else
				print2(x+4,y+7+(19*8),"~",swap_screen,0,254);

			if(sptr>19&&ptr<(length-1))
				{
				ptr++;
				sptr=19;
				}

			for(int ctr=1;ctr<20;ctr++)
				print2(x+12,y+7+(ctr*8),menulist[ctr+(ptr-19)],swap_screen,0,254);
			if(sptr>=19)
				print2(x+4,y+7+(19*8),"~",swap_screen,0,254);
			}
		} while(gc!=13&&gc!=27);
		if(ptr<19)
			ptr=19;

	if(gc==27)
		{
		ptr=19;
		sptr=0;
		}
//		ptr=(length-1);

	return((ptr-19)+sptr);
	}

}


void savegame_save(const char *filename) {
	char header[5]="JAV1";
	FILE *fp = fopen(filename, "wb");
	if(!fp) {
		return;
	}

	fwrite(header,1,4,fp);
	fwrite(savegame.name,1,30,fp);
	fputc(lives,fp);
	fwrite(&savegame.l,1,2,fp);
	fputc(savegame.sjs, fp);
	fwrite(&savegame.sx,1,2,fp);
	fwrite(&savegame.sy,1,2,fp);
	fwrite(savegame.gotit,1,255,fp);
	fputc(no_of_clothes,fp);
	fwrite(&no_of_mons,1,2,fp);
	for(int ctr=0;ctr<no_of_mons;ctr++) {
		for(int ctr2=0;ctr2<12;ctr2++) {
			fputc((*mons)[ctr][ctr2].XY,fp);
			fputc((*mons)[ctr][ctr2].mtype,fp);
			fputc((*mons)[ctr][ctr2].dx,fp);
			fputc((*mons)[ctr][ctr2].dy,fp);
		}
	}

	fclose(fp);

}

int savegame_load(const char *filename) {
	char header[5]="0000";
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		return 0;
	}

	fread(header,1,4,fp);
	if(strcmp(header,"JAV1")) {
		fclose(fp);
		return savegame_loadDOS(filename);
	}

	fread(savegame.name,1,30,fp);
	lives=fgetc(fp);
	fread(&savegame.l,1,2,fp);
	savegame.sjs = fgetc(fp);
	fread(&savegame.sx,1,2,fp);
	fread(&savegame.sy,1,2,fp);
	fread(savegame.gotit,1,255,fp);
	no_of_clothes=fgetc(fp);
	fread(&no_of_mons,1,2,fp);
	for(int ctr=0;ctr<no_of_mons;ctr++) {
		for(int ctr2=0;ctr2<12;ctr2++) {
			(*mons)[ctr][ctr2].XY=(unsigned char)fgetc(fp);
			(*mons)[ctr][ctr2].mtype=fgetc(fp);
			(*mons)[ctr][ctr2].dx=fgetc(fp);
			(*mons)[ctr][ctr2].dy=fgetc(fp);
		}
	}

	fclose(fp);
	return 1;
}

int savegame_loadname(const char *filename) {
	char header[5]="0000";
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		return 0;
	}

	fread(header,1,4,fp);
	if(strcmp(header,"JAV1")) {
		fclose(fp);
		return savegame_loadnameDOS(filename);
	}

	fread(savegame.name,1,30,fp);
	fclose(fp);
	return 1;
}

int savegame_loadDOS(const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		return 0;
	}

	fgetc(fp); // empty
	fread(savegame.name,1,30,fp);
	lives=fgetc(fp);
	fread(&savegame.l,1,2,fp);
	savegame.sjs = fgetc(fp);
	fread(&savegame.sx,1,2,fp);
	fread(&savegame.sy,1,2,fp);
	fread(savegame.gotit,1,255,fp);
	no_of_clothes=fgetc(fp);
	// DOS version assumes 512 monsters
	no_of_mons=512;
	for(int ctr=0;ctr<no_of_mons;ctr++) {
		for(int ctr2=0;ctr2<12;ctr2++) {
			(*mons)[ctr][ctr2].XY=(unsigned char)fgetc(fp);
			(*mons)[ctr][ctr2].mtype=fgetc(fp);
			(*mons)[ctr][ctr2].dx=fgetc(fp);
			(*mons)[ctr][ctr2].dy=fgetc(fp);
		}
	}

	fclose(fp);
	return 1;
}

int savegame_loadnameDOS(const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		return 0;
	}

	fgetc(fp); // Empty
	fread(savegame.name,1,30,fp);
	fclose(fp);
	return 1;
}
