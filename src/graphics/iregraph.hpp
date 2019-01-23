//
//	IRE graphics abstraction layer
//
//	22/05/2011, JPM
//

#ifndef _IRE_IREGRAPH
#define _IRE_IREGRAPH

#include "irekeys.hpp"

class IRECOLOUR
	{
	public:
		IRECOLOUR();
		IRECOLOUR(int r, int g, int b);
		IRECOLOUR(unsigned int packed);
		void Set(int r, int g, int b);
		void Set(unsigned int packed);
		unsigned int packed;
		unsigned char r,g,b;
	};


class IREBITMAP
	{
	public:
		IREBITMAP(int w, int h);
		IREBITMAP(int w, int h, int bpp);
		virtual ~IREBITMAP();
		virtual void Clear(IRECOLOUR *col);
		virtual void Draw(IREBITMAP *dest, int x, int y)=0;
		virtual void DrawSolid(IREBITMAP *dest, int x, int y);
		virtual void DrawStretch(IREBITMAP *dest, int x, int y, int w, int h);
		virtual void Get(IREBITMAP *src, int x, int y);
		virtual int GetPixel(int x, int y);
		virtual int GetPixel(int x, int y, IRECOLOUR *col);
		virtual void PutPixel(int x, int y, IRECOLOUR *col);
		virtual void PutPixel(int x, int y, int r, int g, int b);
		virtual void Render();	// Render to physical screen
		virtual void ShowMouse();
		virtual void HideMouse();

		virtual unsigned char *GetFramebuffer();
		virtual void ReleaseFramebuffer(unsigned char *fbptr);
		virtual int SaveBMP(const char *filename);
		virtual int GetW();
		virtual int GetH();
		virtual int GetDepth();
	};
	
	

extern IREBITMAP *MakeIREBITMAP(int w, int h);
extern IREBITMAP *MakeIREBITMAP(int w, int h, int bpp);


// Startup functions
extern const char *IRE_InitBackend();
extern const char *IRE_Backend();
extern void *IRE_SetTimer(void (*timer)(), int clockhz);
extern void IRE_StopTimer(void *handle, void (*timer)());
extern void IRE_Shutdown();
extern int IRE_Startup(int allow_break, int videomode);

// Other utilities
extern void IRE_StartGFX(int bpp);
extern void IRE_StopGFX();
extern void IRE_WaitFor(int ms);
extern unsigned char *iload_file(const char *filename, int *len);

#define IRE_FNT1 0x31544e46
#define IRE_FNT2 0x32544e46

extern IRECOLOUR *ire_transparent;

#define M_free(a) free(a)
#define M_get(a,b) calloc(a,b)

#endif
