//
//	Other SDL-related routines
//
#include <stdio.h>
#include "ibitmap.hpp"

static int lastz=0;

static int XlateMousebuttons(int input);

IRECOLOUR::IRECOLOUR()
{
Set(0);
}

IRECOLOUR::IRECOLOUR(int red, int green, int blue)
{
Set(red,green,blue);
}

IRECOLOUR::IRECOLOUR(unsigned int pack)
{
Set(pack);
}

void IRECOLOUR::Set(int red, int green, int blue)
{
r=red&0xff;
g=green&0xff;
b=blue&0xff;
if(IRE_pixelFormat)
	packed=SDL_MapRGB(IRE_pixelFormat,r,g,b);
else
	packed = r<<16|g<<8|b;
}

void IRECOLOUR::Set(unsigned int pack)
{
Uint8 rr,gg,bb;
packed=pack;

if(IRE_pixelFormat)	{
	SDL_GetRGB(packed,IRE_pixelFormat,&rr,&gg,&bb);
	}
else	{
	rr=((packed&0xff0000)>>16);
	gg=((packed&0xff00)>>8);
	bb=(packed&0xff);
}
r=rr;
g=gg;
b=bb;
}

void IRE_StopGFX()
{
SDL_Quit();
}


void IRE_WaitFor(int ms)
{
SDL_Delay(ms);
}


