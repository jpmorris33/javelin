//
//  DOS compatibility layer
//

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "graphics/irekeys.hpp"

long coreleft() {
	return 10000000; // This game is probably not going to run out of memory
}

int strcmpi(const char *a, const char *b)
{
if(!a || !b)
        return 666;

for(;;)
        {
        if((unsigned char)*a != (unsigned char)*b)
                if(tolower((unsigned char)*a) != tolower((unsigned char)*b))
                        return (int)tolower((unsigned char)*a) - (int)tolower((unsigned char)*b);
        if(!*a)
                return 0;
        a++;
        b++;
        }

//ithe_panic("Error in ithelib.a","666 in istricmp");
return 666;
}


int getch() {
	int ascii =0;
	IRE_NextKey(&ascii);
	return ascii;
}

int kbhit() {
	return IRE_KeyPressed();
}

int getrandom(long max) {
	return rand() % max;
}
void randomize() {
	srand(time(NULL));
}

char *itoa(int value, char *string, int radix)
{
switch(radix)
    {
    case 10:
    sprintf(string,"%d",value);
    break;

    case 16:
    sprintf(string,"%x",value);
    break;

    default:
    sprintf(string,"%d",radix);
//    ithe_panic("itoa() - unsupported radix",string);
    break;
    };

return string;
}

char *ltoa(long value, char *string, int radix)
{
switch(radix)
    {
    case 10:
    sprintf(string,"%ld",value);
    break;

    case 16:
    sprintf(string,"%lx",value);
    break;

    default:
    sprintf(string,"%ld",radix);
//    ithe_panic("itoa() - unsupported radix",string);
    break;
    };

return string;
}

int getw16(FILE *fp) {
unsigned short s;
int ret;
ret = fread(&s,1,2,fp);
if(ret < 1) {
	return -1;
	}
return s;
}


void copyScreen(unsigned char *dest, unsigned char *src) {
	memcpy(dest,src,64000);
}

