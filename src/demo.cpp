
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>

#include "compat.h"
#include "soundsys.h"

#include "itg/itg.hpp"
#include "javelin.hpp"

#define MAXDEMO 64000

//char *demo747;
//long idx747,demolen;
//char code,rx;
//int noof747s;

unsigned char get_name();
void putcode747(char code);
char getcode747();
void play747(char *filename);
void rec747(char *filename);
char* get_str(char *bufx,int lenn);



char* get_str(char *bufx,int lenn)
{
int hgt,wdt,x,y,gc;
char buf_ptr=0;
char printbuf[]="x\0";
char buf[30];

hgt=(4)*8;
wdt=(lenn+2)*8;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,screen);
hline(x,x+wdt,y,253,screen);
vline(x,y,y+hgt,253,screen);
hline(x,x+wdt,y+hgt,255,screen);
vline(x+wdt,y,y+hgt,255,screen);

print2(x+8,y+3,bufx,screen,0,254);

strcpy(buf,"A Demo");
buf_ptr=strlen(buf);

gc=0;
do      {
	if(gc == '\b') // trap the delete key
		{
		gc=0;
		if(buf_ptr>0)
			buf_ptr--;
		}

	if(buf_ptr>lenn&&gc!=13)    // stop entry getting too long
		gc=0;

	if(gc&&isprint(gc))		  // if the key pressed was printable
		buf[buf_ptr++]=gc;


fbox2(x+1,y+12,wdt-1,9,254,screen);
for(int ctr=0;ctr<buf_ptr;ctr++)
	if(buf[ctr]>=' ')
		{
		printbuf[0]=buf[ctr];
		print2((ctr*8)+x+8,y+13,printbuf,screen,0,254);
		}
	gc=getch();
	if(gc==3||gc==27)
		return(NULL);
	} while(gc != 13 && gc != 3 && gc != 27);

buf[buf_ptr]=0;
return(buf);
}


char getcode747()
{
rx=!rx;
if(!rx)
	return((demo747[idx747]>>4)&15);
if(rx)
	return(demo747[idx747++]&15);
}

void putcode747(char code)
{
if(!rx)
	demo747[idx747]=(code<<4);
if(rx)
	demo747[idx747++]+=code;
rx=!rx;
}

void play747(char *filename)
{
FILE *fp;
unsigned int gw;
int res;

fp=load(filename);
if(!fp) {
	fp=fopen(filename,"rb");
	if(!fp) {
		strcat(filename,".747");
		fp=load(filename);
		if(!fp) {
			fp=fopen(filename,"rb");
			if(!fp) {
				notify("Could not load demo file:",filename);
				return;
			}
		}
	}
}
gw=getw16(fp);
if(gw!=0xdeaf) {
	fclose(fp);
	notify("Bad demo file:",filename);
	return;
	}

res=fread(&demolen,1,4,fp);
fseek(fp,25L,SEEK_CUR);
demo747=(char *)malloc(demolen);
if(!demo747)
	{
	fclose(fp);
	notify("Not enough memory.",NULL);
	return;
	}
res=fread(demo747,1,demolen,fp);
fclose(fp);
idx747=0;
for(int ctr=0;ctr<12;ctr++) {
	aframe[ctr]=0;
}
framerate=0;
game(3);
free(demo747);
}

void rec747(char *title)
{
FILE *fp;

fp=fopen("$$demo$$.747","wb");
if(!fp)	{
	notify("Could not create demo file!",NULL);
	return;
	}

putw(0xdeaf,fp);
demo747=(char *)malloc(MAXDEMO);
if(!demo747) {
	fclose(fp);
	notify("Not enough memory.",NULL);
	return;
	}
idx747=0;
for(int ctr=0;ctr<12;ctr++) {
	aframe[ctr]=0;
}
framerate=0;
game(2);
fwrite(&idx747,1,4,fp);
fwrite(&title[0],1,25,fp);
fwrite(demo747,1,idx747,fp);
fclose(fp);
free(demo747);
}


void demo()
{
char *demolist[256];
char *fnames[256];
char *cwd;
char fna[256];
int x;
FILE *fp;
struct ffblk findblock;

noof747s=0;
demolist[noof747s]=(char *)malloc(25);
strcpy(demolist[noof747s++],"Demo playback:");
demolist[noof747s]=(char *)malloc(25);
strcpy(demolist[noof747s++],"Record New Demo");

if(findfirst("*.747",&findblock,0x2f) == 0)
	{
		do
		{
		fp=fopen(findblock.ff_name,"rb");
		fseek(fp,6L,SEEK_CUR);
		demolist[noof747s]=(char *)malloc(25);
		fnames[noof747s]=(char *)malloc(14);
		if(!demolist[noof747s]||!fnames[noof747s])
			{
			notify("Out of memory.",NULL);
			fclose(fp);
			return;
			}
		res=fread(demolist[noof747s],1,25,fp);
		strcpy(fnames[noof747s],findblock.ff_name);
		fclose(fp);
		noof747s++;
		} while (findnext(&findblock)!= -1&&noof747s<255);
	}

menulist=demolist;
//blit(fg_screen,screen);
blitFromScreen(fg_screen);
x=scroll(noof747s,26);
//blit(screen,fg_screen);
blitToScreen(fg_screen);
clear(fg_screen);
if(x>1)
	play747(fnames[x]);
if(x==1)
	{
//	blit(fg_screen,screen);
	blitFromScreen(fg_screen);
	cwd=get_str("Enter a title:",25);
	//blit(screen,fg_screen);
	blitToScreen(fg_screen);
	clear(fg_screen);
	if(cwd)
		{
		strcpy(fna,cwd);
		rec747(fna);
		//blit(fg_screen,screen);
		blitFromScreen(fg_screen);
		do{
			cwd=get_str("Demo Filename:",13);
			} while(!cwd);
		//blit(screen,fg_screen);
		blitToScreen(fg_screen);
		clear(fg_screen);

		int ctr,ctr2;
		for(ctr=0;ctr<128;ctr++)
			fna[ctr]=0;
		ctr2=0;
		x=strlen(cwd);
		ctr=-1;
		do	{
		ctr++;
			if(cwd[ctr]>'0'&&cwd[ctr]<'z')
				fna[ctr2++]=cwd[ctr];
			}while(ctr<x&&cwd[ctr]!='.'&&ctr2<8);
		strcat(fna,".747");
		if(rename("$$demo$$.747",fna))
			{
			ltoa(getrandom(0xfffffff),string,16);
			strcat(string,".747");
			rename("$$demo$$.747",string);
			}
		}
	}

for(int ctr=0;ctr<noof747s;ctr++)
	{
	free(demolist[ctr]);
	if(ctr>1)
		free(fnames[ctr]);
	}
}


