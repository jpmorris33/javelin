
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

unsigned char *demo747;
u_int32_t idx747,demolen;
unsigned char rx=0;
int noof747s;

extern char aframe[12];
extern int framerate;
extern char string[128];
extern char **menulist;

unsigned char get_name();
void play747(char *filename);
void rec747(char *filename);
char* get_str(char *bufx,int lenn);
extern int scroll(char length,char width);



char* get_str(char *bufx,int lenn)
{
int hgt,wdt,x,y,gc;
int buf_ptr=0;
char printbuf[]="x\0";
static char buf[30];

hgt=(4)*8;
wdt=(lenn+2)*8;

x=160-(wdt/2);
y=100-(hgt/2);

fbox2(x,y,wdt,hgt,254,spare_screen);
hline(x,x+wdt,y,253,spare_screen);
vline(x,y,y+hgt,253,spare_screen);
hline(x,x+wdt,y+hgt,255,spare_screen);
vline(x+wdt,y,y+hgt,255,spare_screen);

print2(x+8,y+3,bufx,spare_screen,0,254);

strcpy(buf,"A Demo");
buf_ptr=strlen(buf);

gc=0;
do {
	if(gc == '\b') { // trap the delete key
		gc=0;
		if(buf_ptr>0) {
			buf_ptr--;
		}
	}

	if(buf_ptr>lenn&&gc!=13) {    // stop entry getting too long
		gc=0;
	}

	if(gc&&isprint(gc)) {		  // if the key pressed was printable
		buf[buf_ptr++]=gc;
	}

	fbox2(x+1,y+12,wdt-1,9,254,spare_screen);
	for(int ctr=0;ctr<buf_ptr;ctr++) {
		if(buf[ctr]>=' ') {
			printbuf[0]=buf[ctr];
			print2((ctr*8)+x+8,y+13,printbuf,spare_screen,0,254);
		}
	}
	blitToScreen(spare_screen);
	gc=getch();
//		if(gc==3||gc==27) {
//			return(NULL);
//		}
} while(gc != 13 && gc != 3 && gc != 27);

buf[buf_ptr]=0;
return(buf);
}


unsigned char getcode747()
{
rx=!rx;
if(!rx)
	return((demo747[idx747]>>4)&15);
else
	return(demo747[idx747++]&15);
}

void putcode747(unsigned char code)
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

rx=0;

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
demo747=(unsigned char *)malloc(demolen);
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
demo747=NULL;
}

void rec747(char *title)
{
FILE *fp;

rx=0;

fp=fopen("$$demo$$.747","wb");
if(!fp)	{
	notify("Could not create demo file!",NULL);
	return;
	}

putw16(0xdeaf,fp);
demo747=(unsigned char *)malloc(MAXDEMO);
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
fwrite(title,1,25,fp);
fwrite(demo747,1,idx747,fp);
fclose(fp);
free(demo747);
demo747=NULL;
}


void demo()
{
char *demolist[256];
char *fnames[256];
char *cwd;
char fna[256];
int x;
FILE *fp;
int res;
char **filelist=NULL;
int files=0;

noof747s=0;
demolist[noof747s]=(char *)malloc(25);
strcpy(demolist[noof747s++],"Demo playback:");
demolist[noof747s]=(char *)malloc(25);
strcpy(demolist[noof747s++],"Record New Demo");

filelist=igetdir(".",".747",&files);
if(filelist) {
	for(int ctr=0;ctr<files;ctr++) {
		if(ctr >= 255) {
			break;
		}
		fp=fopen(filelist[ctr],"rb");
		fseek(fp,6L,SEEK_CUR);
		demolist[noof747s]=(char *)malloc(25);
		fnames[noof747s]=(char *)malloc(256);
		if(!demolist[noof747s]||!fnames[noof747s])
			{
			notify("Out of memory.",NULL);
			fclose(fp);
			return;
			}
		res=fread(demolist[noof747s],1,25,fp);
		strncpy(fnames[noof747s],filelist[ctr],255);
		fnames[noof747s][255]=0;
		fclose(fp);
		noof747s++;
	}
	ifreedir(filelist,files);
	filelist=NULL;
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


