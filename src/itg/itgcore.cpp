//
//	C conversions of the original 80286 assembler
//
//	This stuff was written for speed and has basically no safety checks
//

#include "itg.hpp"
#include "../graphics/iregraph.hpp"

unsigned int packWxH(unsigned char w, unsigned char h);
unsigned char getW(int WxH);
unsigned char getH(int WxH);


static IREBITMAP *vscreen;
static palette vpalette;
static IRECOLOUR vpal[256];
static unsigned char backupscreen[64000];

void off(void) {
	delete vscreen;
	vscreen = NULL;
	IRE_Shutdown();
}

void on(void) {
	IRE_Startup(0,-2);  // Windowed
	vscreen = MakeIREBITMAP(320,200,32);
}

void blitToScreen(unsigned char *screen) {
	if(screen != backupscreen) {
		memcpy(backupscreen, screen, 64000);
	}
	uint32_t *buffer32 = (uint32_t *)vscreen->GetFramebuffer();
	for(int y=0;y<200;y++) {
		for(int x=0;x<320;x++) {
			unsigned char pix = *screen++;
			*buffer32++ = vpal[pix].packed;
		}
	}
	vscreen->Render();
}

void blitFromScreen(unsigned char *screen) {
	// Reversing the 32bpp conversion would be messy, use a backup instead
	memcpy(screen, backupscreen, 64000); 
}

void hline(int x,int x2,int y,int c,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	int w = x2-x;
	for(int ctr=0;ctr<w;ctr++) {
		*buf++ = c&0xff;
	}
}

void vline(int x,int y,int y2,int c,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	int h = y2-y;
	for(int ctr=0;ctr<h;ctr++) {
		*buf = c&0xff;
		buf += 320; // Next line
	}
}

void fbox(int x1,int y1,int x2,int y2,int c,unsigned char *scr) {
	int w = x2-x1;
	int h = y2-y1;
	fbox2(x1,y1,w,h,c,scr);
}

void fbox2(int x,int y,int w,int h,int c,unsigned char *scr) {

	unsigned char *buf = scr + ((y*320) + x);
	for(int ctr=0;ctr<h;ctr++) {
		memset(buf,c&0xff,w);
		buf += 320; // Next line
	}
}

void dot(int x,int y,int c,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	*buf=c&0xff;
}

int point(int x,int y,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	return *buf;
}

void put1(int x,int y,int wxh,unsigned char *spr,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	int ctr;

	for(ctr=0;ctr<h;ctr++) {
		memcpy(buf,spr,w);
		buf+=320;
		spr+=w;
	}
}

void cel1(int x,int y,int wxh,unsigned char *spr,unsigned char *scr) {

	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	unsigned char p;
	int ctr,ctr2,skip=320-w;

	for(ctr=0;ctr<h;ctr++) {
		for(ctr2=0;ctr2<w;ctr2++) {
			p=*spr++;
			if(p)
				*buf=p;
			buf++;
		}
		buf+=skip;
	}
}

//
//  Cel sprite with right clipping
//
void ccel1(int x,int y,int wxh,unsigned char *spr,unsigned char *scr, int skipw) {
	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	unsigned char p;
	int ctr,ctr2,skip=320-w;

	for(ctr=0;ctr<h;ctr++) {
		for(ctr2=0;ctr2<w;ctr2++) {
			p=*spr++;
			if(p)
				*buf=p;
			buf++;
		}
		spr+=skipw; // truncate the right side of the sprite
		buf+=skip;
	}
}

//
//	Left clipping
//

void ccel2(int x,int y,int wxh,unsigned char *spr,unsigned char *scr, int skipw) {
	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	unsigned char p;
	int ctr,ctr2,skip=320-w;

	spr+=skipw; // pre-skip part of the sprite data to eat into the left

	for(ctr=0;ctr<h;ctr++) {
		for(ctr2=0;ctr2<w;ctr2++) {
			p=*spr++;
			if(p)
				*buf=p;
			buf++;
		}
		spr+=skipw; // skip part of the sprite data
		buf+=skip;
	}
}



void inv1(int x,int y,int wxh,unsigned char *spr,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	int ctr,ctr2,skip=320-w;

	for(ctr=0;ctr<h;ctr++) {
		for(ctr2=0;ctr2<w;ctr2++) {
			*spr++ = (*buf++) ^ 0xff;
		}
		buf+=skip;
	}
}


void get1(int x,int y,int wxh,unsigned char *spr,unsigned char *scr) {
	unsigned char *buf = scr + ((y*320) + x);
	unsigned char w = getW(wxh);
	unsigned char h = getH(wxh);
	int ctr;

	for(ctr=0;ctr<h;ctr++) {
		memcpy(spr,buf,w);
		buf+=320;
		spr+=w;
	}
}


void rgb(unsigned char col,int r,int g,int b) {
	vpal[col].Set(r<<2,g<<2,b<<2);
	vpalette[col*3] = r;
	vpalette[(col*3)+1] = g;
	vpalette[(col*3)+2] = b;

	blitToScreen(backupscreen); // Force palette update
}


void pal_set(palette pal) {
	memcpy(vpalette, pal, sizeof(palette));
	for(int ctr=0;ctr<256;ctr++) {
		vpal[ctr].Set(pal[ctr*3]<<2, pal[(ctr*3)+1]<<2, pal[(ctr *3) + 2]<<2);
	}
	blitToScreen(backupscreen); // Force palette update
}

void pal_get(palette pal) {
	memcpy(pal, vpalette, sizeof(palette));
}

void clear(unsigned char *dest) {
	memset(dest,0,64000);
}



void line( int xa, int ya, int xb, int yb, int colour, unsigned char* bitmap) {
	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	// Find the distance to go in each plane.
	delta_x = xb - xa;
	delta_y = yb - ya;

	// Find out the direction
	if(delta_x > 0) incx = 1;
	else if(delta_x == 0) incx = 0;
	else incx = -1;

	if(delta_y > 0) incy = 1;
	else if(delta_y == 0) incy = 0;
	else incy = -1;

	// Find which way is were mostly going
	delta_x = abs(delta_x);
	delta_y = abs(delta_y);
	if(delta_x > delta_y) distance=delta_x;
	else distance=delta_y;

	// Draw the god dam line.
	for(t = 0; t <= distance + 1; t++) {
		dot(xa, ya, colour,bitmap);
		xerr += delta_x;
		yerr += delta_y;
		if(xerr > distance) {
			xerr -= distance;
			xa += incx;
		}
		if(yerr > distance) {
			yerr -= distance;
			ya += incy;
		}
	}
}

//
//  Poke holes in the screen, in a repeating 10x10 pattern
//

void dissad(int x,int y,unsigned char *screen) {
	unsigned char *buf = screen + ((y*320) + x);
	int ctr, ctr2;
	for(ctr=0;ctr<20;ctr++) {
		for(ctr2=0;ctr2<32;ctr2++) {
			*buf=0;
			buf += 10;
		}
		buf += 2880; // 320*9
	}

}

//
//	Cross-fade two screens in a repeating 10x10 pattern
//

void mixad(int x,int y,unsigned char *src,unsigned char *dest) {
	unsigned char *buf = dest + ((y*320) + x);
	unsigned char *srcbuf = src + ((y*320) + x);
	int ctr, ctr2;
	for(ctr=0;ctr<20;ctr++) {
		for(ctr2=0;ctr2<32;ctr2++) {
			*buf=*srcbuf;

			buf += 10;
			srcbuf += 10;
		}
		buf += 2880; // 320*9
		srcbuf += 2880;
	}

}



int fadeout(palette pal) {
	int changed = 0;
	for(int ctr=0;ctr<768;ctr++) {
		if(pal[ctr] > 0) {
			pal[ctr]--;
			changed++;
		}
	}
	return changed;
}

int fadeto(palette Sourcepal,palette Destpal) {
	int changed = 0;
	for(int ctr=0;ctr<768;ctr++) {
		if(Destpal[ctr] > Sourcepal[ctr]) {
			Destpal[ctr]--;
			changed++;
		}
		if(Destpal[ctr] < Sourcepal[ctr]) {
			Destpal[ctr]++;
			changed++;
		}
	}
	return changed;
}

//
//	This was designed to wait for retrace, and then update
//	both the screen and the palette in one go
//	With the screen now virtualised, things are much easier
//
void xbp(unsigned char *dest,unsigned char *src,palette pal) {
	// HACK for Javelin - assume dest is always physical screen
	memcpy(vpalette, pal, sizeof(palette));
	for(int ctr=0;ctr<256;ctr++) {
		vpal[ctr].Set(pal[ctr*3]<<2, pal[(ctr*3)+1]<<2, pal[(ctr *3) + 2]<<2);
	}
	blitToScreen(src);

}

/*
 * UnPCX - Decode the RLE data into RAW bitmap info
 */

void UnPCX(unsigned char *in,unsigned char *out,int length)
{
unsigned char ob;	// Output Byte
int ctr,ctr2;

for(ctr=0;ctr<length;ctr++) {		// Decode it all
	ob=*in++;
	if(ob > 0xbf) {			// If it's a run of colour
		ctr2=(ob & 0x3f);	// Get run length
		ob=*in++;		// Get run colour
		memset(out,ob,ctr2);	// Do the run
		out+=ctr2;		// Increment pointer
		ctr+=ctr2-1;		// Adjust RLE pointer
	} else {
		*out++=ob;		// It was a single colour
	}
}
return;                                 // Finished
}




unsigned char getW(int WxH) {
return (WxH & 0xff00) >> 8;
}

unsigned char getH(int WxH) {
return (WxH & 0xff);
}

unsigned int packWxH(unsigned char w, unsigned char h) {
return (w<<8) | h;
}
