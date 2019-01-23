#ifndef __IRE_SDLBMP
#define __IRE_SDLBMP

#include <SDL2/SDL.h>
#include "../iregraph.hpp"

// JM: Eventually make this always ARGB8888 and simplify code paths
extern SDL_PixelFormat *IRE_pixelFormat;

class SDL20BMP : public IREBITMAP
	{
	public:
		SDL20BMP(int w, int h);
		SDL20BMP(int w, int h, int bpp);
		~SDL20BMP();
		void Clear(IRECOLOUR *col);
		void Draw(IREBITMAP *dest, int x, int y);
		void DrawAddition(IREBITMAP *dest, int x, int y, int level);
		void DrawAlpha(IREBITMAP *dest, int x, int y, int level);
		void DrawDissolve(IREBITMAP *dest, int x, int y, int level);
		void DrawShadow(IREBITMAP *dest, int x, int y, int level);
		void DrawSolid(IREBITMAP *dest, int x, int y);
		void DrawStretch(IREBITMAP *dest, int x, int y, int w, int h);
		void FillRect(int x, int y, int w, int h, IRECOLOUR *col);
		void FillRect(int x, int y, int w, int h, int r, int g, int b);
		void Line(int x, int y, int x2, int y2, IRECOLOUR *col);
		void Line(int x, int y, int x2, int y2, int r, int g, int b);
		void ThickLine(int x, int y, int x2, int y2, int radius, IRECOLOUR *col);
		void ThickLine(int x, int y, int x2, int y2, int radius, int r, int g, int b);
		void Polygon(int count, int *vertices, IRECOLOUR *col);
		void Polygon(int count, int *vertices, int r, int g, int b);
		void Circle(int x, int y, int radius, IRECOLOUR *col);
		void Circle(int x, int y, int radius, int r, int g, int b);
		void SetBrushmode(int mode, int alpha);
		void Get(IREBITMAP *src, int x, int y);
		int GetPixel(int x, int y);
		int GetPixel(int x, int y, IRECOLOUR *col);
		void PutPixel(int x, int y, IRECOLOUR *col);
		void PutPixel(int x, int y, int r, int g, int b);
		void HLine(int x, int y, int w, IRECOLOUR *col);
		void HLine(int x, int y, int w, int r,int g,int b);
		void Render();
		void ShowMouse();
		void HideMouse();
		unsigned char *GetFramebuffer();
		void ReleaseFramebuffer(unsigned char *ptr);
		int SaveBMP(const char *filename);
		int GetW();
		int GetH();
		int GetDepth();

	// Not protected here since the sprite and primitive classes will need them too
		SDL_Surface *img;
		SDL_Texture *texture;
		int width;
		int height;
		int bytesperpixel;
	private:
		void SetBlender();
		int BlenderMode;
		int BlenderAlpha;
		int locked;
	};

#endif
