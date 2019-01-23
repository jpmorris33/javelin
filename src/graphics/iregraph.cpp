#include <stdio.h>
#include <stdlib.h>
#include "iregraph.hpp"

extern void IRE_SetPixelFormat(int bpp);

int _IRE_MouseActivated=0;
int _IRE_BPP=0;
IRECOLOUR *ire_transparent = NULL;

//
//  General-purpose bitmaps
//

IREBITMAP::IREBITMAP(int w, int h)
{
}

IREBITMAP::IREBITMAP(int w, int h, int bpp)
{
}

IREBITMAP::~IREBITMAP()
{
}


//  Stub functions

void IREBITMAP::Clear(IRECOLOUR *col) {}
void IREBITMAP::Draw(IREBITMAP *dest, int x, int y) {}
void IREBITMAP::DrawSolid(IREBITMAP *dest, int x, int y) {}
void IREBITMAP::DrawStretch(IREBITMAP *dest, int x, int y, int w, int h) {}
void IREBITMAP::Get(IREBITMAP *src, int x, int y) {}
int IREBITMAP::GetPixel(int x, int y) {return 0;}
int IREBITMAP::GetPixel(int x, int y, IRECOLOUR *col) {return 0;}
void IREBITMAP::PutPixel(int x, int y, IRECOLOUR *col) {}
void IREBITMAP::PutPixel(int x, int y, int r, int g, int b) {}
void IREBITMAP::Render() {}
void IREBITMAP::ShowMouse() {}
void IREBITMAP::HideMouse() {}
unsigned char *IREBITMAP::GetFramebuffer() {return NULL;}
void IREBITMAP::ReleaseFramebuffer(unsigned char *ptr) {}
int IREBITMAP::SaveBMP(const char *filename) {return 0;}
int IREBITMAP::GetW() {return 0;}
int IREBITMAP::GetH() {return 0;}
int IREBITMAP::GetDepth() {return 0;}


//
//  Other functions
//

void IRE_StartGFX(int bpp)
{
_IRE_BPP=bpp;
IRE_SetPixelFormat(bpp);
ire_transparent = new IRECOLOUR(255,0,255);
}



unsigned char *iload_file(const char *filename, int *len) {
FILE *fp;
unsigned char *buffer = NULL;
int res;
*len = 0;
fp = fopen(filename,"rb");
if(!fp) {
	return NULL;
}
fseek(fp,SEEK_END,0L);
*len = ftell(fp);
fseek(fp,SEEK_SET,0L);

buffer = (unsigned char *)calloc(1,*len);
if(!buffer) {
	*len=0;
	fclose(fp);
	return NULL;
}

res=fread(buffer,1,*len,fp);
fclose(fp);
return buffer;
}
