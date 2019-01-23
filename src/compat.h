//
//  DOS compatibility layer
//

#ifndef __JAV_COMPAT_INC
#define __JAV_COMPAT_INC

extern long coreleft();
extern void clrscr();
extern void gotoxy(int x, int y);
extern int strcmpi(const char *a, const char *b);
extern int getch();
extern int kbhit();
extern int getrandom(long max);
extern void randomize();
extern int itoa(int value, char *string, int radix);
extern int ltoa(long value, char *string, int radix);
extern void blitToScreen(unsigned char *swapscreen);
extern void blitFromScreen(unsigned char *swapscreen);
extern void copyScreen(unsigned char *dest, unsigned char *src);
extern int getw16(FILE *fp);

#endif
