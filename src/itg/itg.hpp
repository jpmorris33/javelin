/*	The ITHE Graphics Library - Version 3.00	*/

#ifndef __ITG16
#define __ITG16

#include <stdio.h>

typedef unsigned char palette[769];
typedef unsigned char pattern[100];

struct SPRITE
	{
	unsigned char *spr;
	unsigned int WxH;
	int allocate(unsigned char width,unsigned char height);
	void free();
	~SPRITE();
	SPRITE();

	void cel_put_sprite(int x,int y,unsigned char *dest);
	void cel_put_sprite2(int x,int y,unsigned char *dest);
	void clip_sprite(int x,int y,unsigned char *dest);
	void block_put_sprite(int x,int y,unsigned char *dest);
	void inverse_put_sprite(int x,int y,unsigned char *dest);
	void get_sprite(int x,int y,unsigned char *src);
	int get_width();
	int get_height();
	};

extern void off(void);
extern void on(void);
extern void hline(int x,int x2,int y,int c,unsigned char *scr);
extern void vline(int x,int y,int y2,int c,unsigned char *scr);
extern void fbox(int x1,int y1,int x2,int y2,int c,unsigned char *scr);
extern void fbox2(int x,int y,int w,int h,int c,unsigned char *scr);
extern void box(int x1,int y1,int x2,int y2,int c,unsigned char *scr);
extern void filled_box(int x1,int y1,int x2,int y2,int c,unsigned char *dest);
extern void dot(int x,int y,int c,unsigned char *scr);
extern int point(int x,int y,unsigned char *scr);
//extern "C" void blit(unsigned char *to,unsigned char *from);
void blitToScreen(unsigned char *screen);
void blitFromScreen(unsigned char *screen);

extern void put1(int x,int y,int s,unsigned char *spr,unsigned char *scr);
extern void cel1(int x,int y,int s,unsigned char *spr,unsigned char *scr);
extern void cel2(int x,int y,int s,unsigned char *spr,unsigned char *scr);

extern void ccel1(int x,int y,int s,unsigned char *spr,unsigned char *scr,int skipw);
extern void ccel2(int x,int y,int s,unsigned char *spr,unsigned char *scr,int skipw);

extern void inv1(int x,int y,int s,unsigned char *spr,unsigned char *scr);
extern void get1(int x,int y,int s,unsigned char *spr,unsigned char *scr);
extern void rgb(unsigned char col,int r,int g,int b);
extern void pal_set(palette pal);
extern void pal_get(palette pal);
extern void clear(unsigned char *dest);
extern void partial_blit(int x,int y,int w,int h,unsigned char *swapscr);
extern void putblit(int x,int y,int s,unsigned char *to,unsigned char *from);
extern void box_putblit(int x,int y,int w,int h,unsigned char *to,unsigned char *from);
extern int PCX(char pcxname[127],unsigned char *dest,palette pal);
extern int GIF(char *filename,unsigned char *dest,palette pal);
extern int load_RSD(SPRITE RSD[10000],char filename[127]);
extern int save_RSD(SPRITE RSD[10000],int quantity,char filename[127]);
extern int load_CEL(SPRITE *CEL,char filename[127]);
extern int save_CEL(SPRITE *CEL,char filename[127]);
extern int load_PAL(palette pal,char filename[127]);
extern int save_PAL(palette pal,char filename[127]);
extern void line(int x1,int y1,int x2,int y2,unsigned char colour,unsigned char *screen);
extern void dissad(int x,int y,unsigned char *screen);
extern void mixad(int x,int y,unsigned char *screen,unsigned char *screenk);
extern void dissolve(pattern dis_pat,int xdelay);
extern void mix(pattern dis_pat,int xdelay,unsigned char *screen, unsigned char *dest);
extern int load_FAD(pattern dis_pat,char *fname);
extern int fadeout(palette pal);
extern int fadeto(palette Sourcepal,palette Destpal);
extern void a_circle(int x,int y,int r,int c,unsigned char *dest);
extern void __M_delay(unsigned int xdelay);
extern int play_flic(char *filename,unsigned char *swap_screen);
extern int force_flic(char *filename,unsigned char *swap_screen);
extern void xbp(unsigned char *dest,unsigned char *src,palette pal);
extern char date_stamp[],time_stamp[];
extern int FP_play_flic(FILE *fp,unsigned char *swap_screen);
extern int FP_force_flic(FILE *fp,unsigned char *swap_screen);
extern int FP_GIF(FILE *fp,unsigned char *dest, palette pal);
#endif
