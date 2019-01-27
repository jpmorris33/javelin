//
//  DOS compatibility layer
//

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
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

void putw16(unsigned short s, FILE *fp) {
fwrite(&s,1,2,fp);
}


void copyScreen(unsigned char *dest, unsigned char *src) {
	memcpy(dest,src,64000);
}


static int IsItADirectory(struct dirent *e)
{
if(e->d_type == DT_DIR)
	 return 1;
return 0;
}


char **igetdir(const char *dir, const char *filetype, int *items)
{
const char *dotdot = "[..]";
int count=0;
char **out;
DIR *d;
struct dirent *e;
char path[1024];

if(!dir || !items)
	return NULL;

d=opendir(dir);

if(!d)
	return NULL;

do	{
	e=readdir(d);
	if(!e)
		break;
	
	if(IsItADirectory(e)) {
		continue;
	}
	if(!filetype || strstr(e->d_name,filetype)) {
		count++;
	}

} while(e);

if(count == 0)	{
	closedir(d);
	return NULL;
	}

out=(char **)calloc(1,sizeof(char *)*count);
if(!out)	{
	closedir(d);
	return NULL;
	}

count=0;

rewinddir(d);
for(;;)	{
	e=readdir(d);
	if(!e)
		break;

	if(IsItADirectory(e))
		 continue;  // We don't like your kind around here

	if(filetype && !strstr(e->d_name,filetype)) {
		continue; // Didn't match
	}

		
	// Okay, add it to the list
	out[count] = (char *)calloc(1,strlen(e->d_name)+1);	// length and NULL
	if(!out[count])
		break;
	strcpy(out[count],e->d_name);
	count++;
	}

closedir(d);
*items=count;
return out;
}


void ifreedir(char **list, int items)
{
int ctr;
if(!list)
	return;

for(ctr=0;ctr<items;ctr++)
	if(list[ctr])
		 free(list[ctr]);
free(list);
}

