#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

extern unsigned char curvol;
#include "javelin.hpp"
#include "soundsys.h"

int menu()
{
int g;
unsigned int mptr=0,messlen,c2=0;

// Now we do the scrolly

/*unsigned char message[]="JAVELIN 2.0                 Starring Arthur Barr..              \
This is a BETA TEST version: you may distribute it A BIT.    \
If you try and distribute an incomplete version (i.e. without the flics) I WILL SMASH YOUR FACE IN.\
I shall send DOUG to bomb your house.  You will die penniless and unhappy.  So remember that.       \
									  Javelin was brought to you by ַüיגה Software \
(New College Graphics Dept.)  Credits For Javelin:        \
DESIGN:              Joseph Morris of ַüיגה software, Ian Morris, Brendan Cracknell of BC Publications          \
CODING:              Joseph Morris of ַüיגה          \
MUSIC:               AudioTherapy           \
TESTING:  David Leach, Paul Brown       ADDITIONAL SUPPORT ETC:        \
Martyn Smart of ַüיגה, James Grant of ַüיגה, Ian Morris, Brendan Cracknell of BC Publications, \
Arthur Barr, Daniel Shirley, Ross Wintle, Ian Hanson (for the RoadHog) and Everyone else at New College who was impressed by it\
									 Javelin was written with the following tools:\
Borland C++ Version 4.5      Turbo Assembler Version 2.5      Borland Power Pack 1.0		 \
ITG16 - The Protected mode version of the ַüיגה graphics library       Keyboard Handler came from The Sprite Toolkit by Jari Karjala\
		 Backgrounds produced with Vista Pro 1.0            Fonts drawn by Brendan using Arthur's editor              \
									  You may have noticed that while I give credit for the Music Modules, \
you can't actually hear any at all.  This is because there is no sound code yet..             \
Still there? That's good.  Javelin was written using only 286 instructions, yet it still enters and leaves \
protected mode: proving that the ------s at Intel don't know what they're on about.  \
Javelin has been designed to harness the hidden power of the 80286 chip (well, most of it).  \
If I'd said I'd written a 286 protected mode program a couple of years ago, no-one would have believed me!  \
						 I say i wrote it: well, I gotta admit that the 16 bit DPMI code is Borland's work, and a damn fine job it is too!\
																	  \
ZARDOZ SPEAKS!   				  DOS IS GOOD.  'dos is good!'   \
WINDOWS IS EVIL.   WINDOWS BRINGS FORTH VISUAL BASIC AND CREATES DLLS TO POISON THE EARTH WITH A PLAGUE OF CRAP PROGRAMS,\
 AS SOON IT MAY BE.    WINDOWS MUST DIE, AND you MUST DESTROY IT.        GO FORTH AND KILL.           ZARDOZ HAS SPOKEN.\
																	  \
																	  \
																	";*/

palette pal;

loadGIF("titlepic.gif",swap_screen,pal);
//xbp(screen,swap_screen,pal);
xbp(NULL,swap_screen,pal); 

S_Fadeout();
S_StopSong();
S_SetMusicVol(curvol);
S_PlaySong(1);

IRE_ClearKeyBuf();
messlen=strlen(message);

do      {					// This is cack but it's the only way
	g=0;
	if(IRE_TestKey(IREKEY_1))
		g=1;
	if(IRE_TestKey(IREKEY_2))
		g=2;
	if(IRE_TestKey(IREKEY_3) || IRE_TestKey(IREKEY_ESC))
		g=3;
	if(IRE_TestKey(IREKEY_F3))
		g=4;

	blitToScreen(swap_screen);
/*
	// maintain the scrolly

	if(message[mptr]<32)
		message[mptr]=' ';
	scroll(swap_screen,sfont[message[mptr]].spr,c2);
	while(!inport(0x3da)&8);
	zmov(swap_screen,screen);
	while(!inport(0x3da)&8);
	c2++;
	c2++;
	if(c2==8||gr_keys[GR_KEY_ARROW_RIGHT])
	{
	c2=0;
	mptr++;
	if(mptr==messlen)
		mptr=0;
	}
*/
	} while(!g);

//gr_end_kbd_grab();              // give it back after

return g;
}
