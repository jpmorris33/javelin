/*	The ITHE Graphics Library - Version 3.00	*/

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "itg.hpp"

char time_stamp[]=__TIME__;
char date_stamp[]=__DATE__;

extern unsigned int packWxH(unsigned char w, unsigned char h);
extern unsigned char getW(int WxH);
extern unsigned char getH(int WxH);
extern void UnPCX(unsigned char *in, unsigned char *out, int len);

void box(int x1,int y1,int x2,int y2,int c,unsigned char *dest)
	{
	hline(x1,x2,y1,c,dest);
	hline(x1,x2,y2,c,dest);
	vline(x1,y1,y2,c,dest);
	vline(x2,y1,y2,c,dest);
	}

void filled_box(int x1,int y1,int x2,int y2,int c,unsigned char *dest)
	{
	int temp;

	if(x1>x2)
		{
		temp=x1;
		x1=x2;
		x2=temp;
		}

	if(y1>y2)
		{
		temp=y1;
		y1=y2;
		y2=temp;
		}
	fbox(x1,y1,x2,y2,c,dest);
	}

int SPRITE::allocate(unsigned char width,unsigned char height)
	{
	WxH = packWxH(width, height);
	spr=(unsigned char *)malloc(width*height+1);
	if(spr==NULL)
		return NULL;
	return 1;
	}

void SPRITE::free()
	{
	::free(spr);
	WxH=0;
	return;
	}

SPRITE::~SPRITE()
	{
	if(WxH)
		::free(spr);
	WxH=0;
	return;
	}

SPRITE::SPRITE()
	{
	WxH=0;
	return;
	}

void SPRITE::cel_put_sprite(int x,int y,unsigned char *dest)
	{
	cel1(x,y,WxH,spr,dest);
	return;
	}

void SPRITE::cel_put_sprite2(int x,int y,unsigned char *dest)
	{
	cel1(x,y,WxH,spr,dest); // cel2 is an optimisation we don't need
	return;
	}

void SPRITE::block_put_sprite(int x,int y,unsigned char *dest)
	{
	put1(x,y,WxH,spr,dest);
	return;
	}

void SPRITE::inverse_put_sprite(int x,int y,unsigned char *dest)
	{
	char Y2;
	Y2=y;
	inv1(x,Y2,WxH,spr,dest);
	return;
	}

void SPRITE::get_sprite(int x,int y,unsigned char *src)
	{
	get1(x,y,WxH,spr,src);

	//put1(0,0,WxH,spr,src); // HACK
	return;
	}

void SPRITE::clip_sprite(int x,int y,unsigned char *dest)
	{
	unsigned char Y2;
	unsigned char *spr2;
	int x2,w,h,y2;
	Y2=y;

	w=getW(WxH);
	h=getH(WxH);

	spr2=spr;
	x2=x+w;
	y2=y+h;

	if(y<0)
		{
		h+=y;
		Y2=0;
		spr2+=((0-y)*w);
		}
	else
	if(y2>199)
		h=200-y;
	if(y>199||y2<=0)
		return;
	if(x>319||x2<=0)
		return;

	if(x2>319)
		{
		int wxh2 = packWxH(320-x, getH(WxH));
		ccel1(x,Y2,wxh2,spr2,dest,x2-320);
		}
	else
	if(x<0)
		{
		int wxh2 = packWxH(getW(WxH)+x, getH(WxH));
		ccel2(0,Y2,wxh2,spr2,dest,0-x);
		}
	else
		{
		int wxh2 = packWxH(getW(WxH), h);
		cel1(x,Y2,wxh2,spr2,dest);
		}
	return;
	}

int SPRITE::get_width()
{
	return getW(WxH);
}

int SPRITE::get_height()
{
	return getH(WxH);
}


int PCX(char pcxname[127],unsigned char *dest,palette pal)
{
unsigned int ctr;
unsigned char *pcx;
palette pal2;
int len;

FILE *fp;
pcx=(unsigned char *)malloc(65535);
if(!pcx)
	return NULL;

fp=fopen(pcxname,"rb");
if(!fp)
	return NULL;

fseek(fp,-768L,SEEK_END);
fread(pal2,768,1,fp);
for(ctr=0;ctr<768;ctr++)
	pal2[ctr]=(unsigned char)pal2[ctr]>>2;
pal2[768]=0;
fseek(fp,128,SEEK_SET);
if(pal==NULL)
	pal_set(pal2);
else
	memmove(pal,pal2,768);

len=fread(pcx,1,65535,fp);
UnPCX(pcx,dest,len);

free(pcx);
fclose(fp);
return 1;
}

int load_CEL(SPRITE *CEL,char filename[127])
{
FILE *fp;
palette pal;
unsigned int ID;
int w,h;

fp=fopen(filename,"rb");
if(!fp)
	return NULL;

ID=getw(fp);

if(ID!=0x9119)
	return NULL;

w=getw(fp);
h=getw(fp);
fseek(fp,32L,SEEK_SET);
fread(pal,768,1,fp);
if(!CEL->allocate(w,h))
	return NULL;
fread(CEL->spr,w*h,1,fp);
fclose(fp);
return 1;
}

int save_CEL(SPRITE *CEL,char filename[127])
{
FILE *fp;
palette pal;
unsigned int ID;
int w,h;
unsigned long sizey;

fp=fopen(filename,"wb");
if(!fp)
	return NULL;
putw(0x9119,fp);
ID=CEL->WxH;
w=getW(ID);
h=getH(ID);
putw(w,fp);
putw(h,fp);
putw(0,fp);	// x coord
putw(0,fp);	// y coord
putc(8,fp);	// 8 bits/pixel
putc(0,fp);	// 0 compression
sizey=w*h;
fwrite(&sizey,4,1,fp);
fseek(fp,32L,SEEK_SET);
pal_get(pal);
fwrite(pal,768,1,fp);
fwrite(CEL->spr,w*h,1,fp);
fclose(fp);
return 1;
}

int save_RSD(SPRITE RSD[10000],int quantity,char filename[127])
{
int ctr;
unsigned int I1;
char ID[]={"SAVEtheB52s"};
FILE *fp;

fp=fopen(filename,"wb");
if(!fp)
	return NULL;
fwrite(ID,strlen(ID)+1,1,fp);
putw(quantity,fp);
for(ctr=0;ctr<quantity;ctr++)
	{
	putw(RSD[ctr].WxH,fp);
	I1=getH(RSD[ctr].WxH) * getW(RSD[ctr].WxH);
	fwrite(RSD[ctr].spr,I1,1,fp);
	}
fclose(fp);
return 1;
}

int load_RSD(SPRITE RSD[10000],char filename[127])
{
int ctr,w,h;
int quantity;
unsigned int I1;
char ID[12];
char CMP[]={"SAVEtheB52s"};
FILE *fp;

fp=fopen(filename,"rb");
if(!fp)
	return NULL;
fread(ID,strlen(CMP)+1,1,fp);
if(strcmp(ID,CMP))
	return NULL;
quantity=getw(fp);
for(ctr=0;ctr<quantity;ctr++)
	{
	I1=getw(fp);
	h=getH(I1);
	w=getW(I1);
	I1=w*h;
	if(!RSD[ctr].allocate(w,h))
		return NULL;
	fread(RSD[ctr].spr,I1,1,fp);
	}
fclose(fp);
return quantity;
}

int load_PAL(palette pal,char filename[127])
{
FILE *fp;

fp=fopen(filename,"rb");
if(!fp)
	return NULL;
fread(pal,768,1,fp);
fclose(fp);
return 1;
}

int save_PAL(palette pal,char filename[127])
{
FILE *fp;

fp=fopen(filename,"wb");
if(!fp)
	return NULL;
fwrite(pal,768,1,fp);
fclose(fp);
return 1;
}

void pal_opt(palette pal)
{
for(int ctr=0;ctr<768;ctr++)
	if(pal[ctr]>63)
  	 	pal[ctr]=pal[ctr]%64;
}

void dissolve(pattern dis_pat,int xdelay)
{
/*
char *screen=(char *)MK_FP(__SegA000,0);
for(int ctr=0;ctr<100;ctr++)
	{
	dissad((dis_pat[ctr]&240)>>4,dis_pat[ctr]&15,screen);
	__M_delay(xdelay);
	blitToScreen(screen);
	}
*/
}

void mix(pattern dis_pat,int xdelay,unsigned char *screen, unsigned char *dest)
{
for(int ctr=0;ctr<100;ctr++)
	{
	mixad((dis_pat[ctr]&240)>>4,dis_pat[ctr]&15,screen,dest);
//        __M_delay(xdelay);
	SDL_Delay(xdelay);
	blitToScreen(dest);
        }
}

int load_FAD(pattern dis_pat,char fname[127])
{
FILE *fp;
fp=fopen(fname,"rb");
if(!fp)
	return NULL;
fread(dis_pat,1,100,fp);
fclose(fp);
return 1;
}



void __M_delay(unsigned int xdelay) {
	usleep(xdelay * 1000 * 14);  // Convert to approximate 70ths of a second
}

