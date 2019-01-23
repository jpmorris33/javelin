//	ITG16 internal flic player

#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include "itg/itg.hpp"
#include "compat.h"


unsigned char *chunk_data;
unsigned char *chunk_ptr;
int ctr;
char blit_sig,pal_sig;
palette _Xpal;

unsigned long tick_count = SDL_GetTicks();

FILE *fp;

int play_flic(char *fname,unsigned char *swap_screen);
int FP_play_flic(FILE *fpx,unsigned char *swap_screen);
int force_flic(char *fname,unsigned char *swap_screen);
int FP_force_flic(FILE *fpx,unsigned char *swap_screen);
int playercore(FILE *fpx,unsigned char *swap_screen, int forceflic);


void col64();
void col256();
void LC(unsigned char *swap_screen);
void brun(unsigned char *swap_screen);
void copy(unsigned char *swap_screen);
void choose_method(unsigned int method,unsigned char *swap_screen);

static void _hl(int x,int y,int l,int c,unsigned char *scr);
static void _bhl(int x,int y,int l,unsigned char *data,unsigned char *scr);
static int Wget(unsigned char *chunk_ptr);
static void read_Fli_head(struct Fli_head *out, unsigned char *in);
static void read_Frame_header(struct Frame_header *out, unsigned char *in);
static void read_Chunk_header(struct Chunk_header *out, unsigned char *in);

struct Fli_head {
	int32_t size;
	unsigned short magic;
	unsigned short frames;
	unsigned short width;
	unsigned short height;
	unsigned short depth;
	unsigned short flags;
	unsigned short speed;
	char nothing[110];
} fli_head;	// 128 bytes

struct Frame_header
	{
	uint32_t size;	// size of chunk coming
	unsigned short F1FA;	// a file sig.	always f1fa
	unsigned short chunks;	// number of little chunks coming
	int32_t nothing;		// blank
	int32_t more_nothing;	// blank
	} frame_header; // 16 bytes

struct Chunk_header
	{
	uint32_t length;
	unsigned short type;
	} chunk_header;	// 6 bytes

long kpos;

int play_flic(char *fname,unsigned char *swap_screen)
{
fp=fopen(fname,"rb");
if(!fp)
	return 0;

return playercore(fp,swap_screen, 0);
}

int FP_play_flic(FILE *fpx,unsigned char *swap_screen) {
	return playercore(fpx,swap_screen, 0);
}

int force_flic(char *fname,unsigned char *swap_screen)
{
fp=fopen(fname,"rb");
if(!fp)
	return 0;

return playercore(fp,swap_screen, 1);
}

int FP_force_flic(FILE *fpx,unsigned char *swap_screen) {
	return playercore(fpx,swap_screen, 1);
}


int playercore(FILE *fpx,unsigned char *swap_screen, int forceflic)
{
unsigned int ctr2,zt1,zt2;
long speed;
unsigned char buf[128];

fp=fpx;

if(!fp)
	return NULL;

blit_sig=0;		pal_sig=0;
blitFromScreen(swap_screen);
fread(buf,128,1,fp);
read_Fli_head(&fli_head, buf);
speed=fli_head.speed * 10;

if(fli_head.magic !=0xaf11&&fli_head.magic !=0xb52)
	{
	fclose(fp);
	printf("Do not want\n");
	return NULL;
	}

chunk_data=(unsigned char *)malloc(65535);
if(!chunk_data)
	{
	fclose(fp);
	return NULL;
	}

fli_head.frames--;	// remove the RING frame

ctr=0;
while(ctr<fli_head.frames && (!kbhit() && !forceflic))
	{
	ctr++;
	fread(buf,16,1,fp);
	read_Frame_header(&frame_header, buf);
	if(fli_head.magic ==0xb52)
			frame_header.size^=0xb52;
	ctr2=0;
	while(ctr2<frame_header.chunks && (!kbhit() && !forceflic))
		{
		ctr2++;
		fread(buf,6,1,fp);
		read_Chunk_header(&chunk_header, buf);
		kpos = ftell(fp);
		fread(chunk_data,1,chunk_header.length-6,fp);
		choose_method(chunk_header.type,swap_screen);
		}
		if(blit_sig&&pal_sig)
			xbp(NULL,swap_screen,_Xpal);
		else
			{
			if(blit_sig)
				blitToScreen(swap_screen);
			if(pal_sig)
				pal_set(_Xpal);
			}
		blit_sig=0;		pal_sig=0;

		while (SDL_GetTicks() < tick_count) {
			SDL_Delay(0);
		}
                tick_count += speed;

	}
fclose(fp);
free(chunk_data);
return 1;
}


void choose_method(unsigned int method,unsigned char *swap_screen)
{
switch(method)
	{
	case 4:
	col256();		// type 1 palette - done
	break;

	case 11:
	col64();		// type 2 palette - done
	break;

	case 12:		// delta - done
	LC(swap_screen);
	break;

	case 13:		// it's black.	- done
	clear(swap_screen);
	break;

	case 15:		// byte Run-length - done
	brun(swap_screen);
	break;

	case 16:		// uncompressed - done
	copy(swap_screen);
	break;

//        default:
//        printf("Unknown compression method %d\n",method);
//        break;
	}
return;
}

void col256()
{
int packets,ctr,ctr2,number;
unsigned char skip_count,r,g,b;
int index=0;
pal_get(_Xpal);
chunk_ptr=chunk_data;

packets=Wget(chunk_ptr);
chunk_ptr+=2;

for(ctr=0;ctr<packets;ctr++)
	{
	skip_count=*chunk_ptr++;
	index+=skip_count;
	number=(unsigned char)*chunk_ptr++;
	if(number==0)
		number=256;
	for(ctr2=0;ctr2<number;ctr2++)
		{
		_Xpal[index*3]=(*chunk_ptr++>>2);
		_Xpal[(index*3)+1]=(*chunk_ptr++>>2);
		_Xpal[(index*3)+2]=(*chunk_ptr++>>2);
		index++;
		}
	}
pal_sig=1;
fseek(fp,chunk_header.length - 6+kpos,SEEK_SET);
}

void col64()
{
int packets,ctr,ctr2,number;
unsigned char skip_count,r,g,b;
int index=0;
pal_get(_Xpal);
chunk_ptr=chunk_data;

packets=Wget(chunk_ptr);
chunk_ptr+=2;
for(ctr=0;ctr<packets;ctr++)
	{
	skip_count=*chunk_ptr++;;
	index+=skip_count;
	number=*chunk_ptr++;;
	if(number==0)
		number=256;
	for(ctr2=0;ctr2<number;ctr2++)
		{
		_Xpal[index*3]=*chunk_ptr++;;
		_Xpal[(index*3)+1]=*chunk_ptr++;;
		_Xpal[(index*3)+2]=*chunk_ptr++;;
		index++;
		}
	}
pal_sig=1;
fseek(fp,chunk_header.length - 6+kpos,SEEK_SET);
}

void copy(unsigned char *swap_screen)	// transfer uncompressed flic data
{
fseek(fp,2L,SEEK_CUR);
blit_sig=1;
}

void brun(unsigned char *swap_screen)	// transfer compressed flic data - byte runlength
{
char packettype;
unsigned char packlen,pk;
int width=0,height=0;
unsigned char *destscreen = swap_screen;
chunk_ptr=chunk_data;

for(height=0;height<200;height++)
	{
	chunk_ptr++;
	width=0;
	while (width<320)
		{
		packettype=*chunk_ptr++;;
		if(packettype<0)
			{
			packlen=abs(packettype);
			memcpy(destscreen,chunk_ptr,packlen);
			width+=packlen;
			destscreen+=packlen;
			chunk_ptr+=packlen;
			}
		else
			{
			packlen=abs(packettype);
			pk=*chunk_ptr++;
			memset(destscreen,pk,packlen);
			width+=packlen;
			destscreen+=packlen;
			}
		}
	}
blit_sig=1;
fseek(fp,chunk_header.length - 6+kpos,SEEK_SET);
return;
}

void LC(unsigned char *swap_screen)
{
int y_off,no_of_y;
int ctr,ctr2,x,y,x_off,no_of_packs,skip,zappa;
char pk;
char packettype;
chunk_ptr=chunk_data;

y_off=Wget(chunk_ptr);
chunk_ptr+=2;
no_of_y=Wget(chunk_ptr);
chunk_ptr+=2;

y=y_off;
for(ctr=0;ctr<no_of_y;ctr++)
	{
	no_of_packs=(unsigned char)*chunk_ptr++;
		  if(no_of_packs!=0)
			{
		x_off=(unsigned char)*chunk_ptr++;;
		x=x_off;
			}
	for(ctr2=0;ctr2<no_of_packs;ctr2++)
		{
		skip=(unsigned char)*chunk_ptr++;;
		packettype=skip;
		if(packettype>0)
			{
			zappa=abs(packettype);
			_bhl(x,y,zappa,chunk_ptr,swap_screen);
			x+=zappa;
		        chunk_ptr+=zappa;

//			for(int k=0;k<zappa;k++) {
//				dot(x,y,(unsigned char)*chunk_ptr++,swap_screen);
//				x++;
//				}
			}
		else
			{
			zappa=abs(packettype);
			pk=*chunk_ptr++;;
			_hl(x,y,zappa,pk,swap_screen);
			x+=zappa;
			}

		if(no_of_packs>1 && ctr2!=no_of_packs-1)
			x+=(unsigned char)*chunk_ptr++;;
		}
	y++;
	}
	blit_sig=1;
fseek(fp,chunk_header.length - 6+kpos,SEEK_SET);
return;
}

//
//	Ports of the assembler routines
//

void _hl(int x,int y,int l,int c,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
        for(int ctr=0;ctr<l;ctr++) {
                *buf++ = c&0xff;
        }
}

void _bhl(int x,int y,int l,unsigned char *data,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	memcpy(buf,data,l);
}

int Wget(unsigned char *chunk_ptr) {
	unsigned short val;
	memcpy(&val,chunk_ptr,2);
	return val;
}

//
//	Do something more sensible than fread'ing the structures into memory
//

void read_Fli_head(struct Fli_head *out, unsigned char *in) {
	int32_t i32;
	uint16_t u16;
	memset(out,0,sizeof(Fli_head));

	memcpy(&i32,in,4);
	in+=4;
	out->size = i32;
	
	memcpy(&u16,in,2);
	in+=2;
	out->magic = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->frames = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->width = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->height = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->depth = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->flags = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->speed = u16;
}


void read_Frame_header(struct Frame_header *out, unsigned char *in) {
	uint32_t u32;
	uint16_t u16;
	memset(out,0,sizeof(Frame_header));

	memcpy(&u32,in,4);
	in+=4;
	out->size = u32;
	
	memcpy(&u16,in,2);
	in+=2;
	out->F1FA = u16;

	memcpy(&u16,in,2);
	in+=2;
	out->chunks = u16;
}


void read_Chunk_header(struct Chunk_header *out, unsigned char *in) {
	uint32_t u32;
	uint16_t u16;
	memset(out,0,sizeof(Chunk_header));

	memcpy(&u32,in,4);
	in+=4;
	out->length = u32;
	
	memcpy(&u16,in,2);
	in+=2;
	out->type = u16;
}

