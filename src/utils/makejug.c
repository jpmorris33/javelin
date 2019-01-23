#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH 32767
typedef int32_t INT32;

void F_error(char *e,char *e2);
FILE *JUG4save(char *jugfile,char *entryname, INT32 *len);
char *protectfilename(const char *filename);
INT32 getfilelength(FILE *fp);
void stringlower(char *string);
void stringupper(char *string);

char *databuf;
char *jugname;

int main(int argc,char *argv[])
{
FILE *fp,*fpI,*fpO;
char string[1024];
char string2[1024];
INT32 leng,lediv,lerem;
unsigned long lctr;
unsigned short sig;

puts("Javelin JUG compiler 1.1 (C)1995-2019 IT-HE Software");
puts("\nTakes a list file and produces a JUG file from it.");
puts("\n\nHINT:");
puts("Look at demo.lst as an example.\n\n");
if(argc<2)
	{
	puts("Syntax: makejug <listfile> [JUG file]");
	puts("If no JUG file is specified, OUTPUT.JUG is created.\n");
	return 1;
	}

if(argc>2)
	jugname=argv[2];
else
	jugname="output.jug";

databuf=(char *)malloc(HASH);
if(!databuf)
	F_error("Out of memory.",NULL);

fp=fopen(jugname,"wb");
if(!fp)
	F_error("Cannot open new JUGfile:",jugname);
sig=0x7074+0x400;	//0x747.400 jug version no
fwrite(&sig,1,2,fp);
fclose(fp);

fp=fopen(argv[1],"r");
if(!fp)
	F_error("Cannot open list file:",argv[1]);

while(fgets(string,1023,fp))
	{
	for(lctr=0;lctr<1023;lctr++)
		if(string[lctr]<33||string[lctr]>'z')
			string[lctr]=0;

	if(string[0]!='#'&&string[0]>32&&string[0]<127)	{
		fpI=fopen(string,"rb");
		if(!fpI) {
			strcpy(string2,string);
			stringlower(string2);
			fpI=fopen(string2,"rb");
			if(!fpI) {
				stringupper(string2);
				fpI=fopen(string2,"rb");
					if(!fpI) {
					F_error("Can't open entry:",string);
				}
			}
		}

		leng=getfilelength(fpI);

		lediv=leng/HASH;
		lerem=leng%HASH;

		fpO=JUG4save(jugname,protectfilename(string),&leng);
		if(!fpO)
			F_error("Cannot write to JUGfile:",jugname);

		for(lctr=0;lctr<lediv;lctr++)
			{
			fread(databuf,1,HASH,fpI);
			fwrite(databuf,1,HASH,fpO);
			}
		if(lerem)
			{
			fread(databuf,1,lerem,fpI);
			fwrite(databuf,1,lerem,fpO);
			}
		fclose(fpI);
		fclose(fpO);
		printf("%s written\n",string);
		}
	};
fclose(fp);
puts("Done.");
}

FILE *JUG4save(char *jugfile,char *entryname,INT32 *len)
{
FILE *fp;

	fp=fopen(jugfile,"ab");
	if(!fp)
		return NULL;
	fseek(fp,0L,SEEK_END);
	fwrite(entryname,1,13,fp);
	fwrite(len,1,4,fp);

	return fp;
}

void F_error(char *e,char *e2)
{
printf("%s %s\n",e,e2);
exit(1);
}

char *protectfilename(const char *filename)
{
static char buffer[1024];
static char fname[13];
const char *ext;
char *bext;

memset(buffer,0,1024);
memset(fname,0,13);
strncpy(buffer,filename,1023);
buffer[8]=0; // chop to 8 characters maximum for filename
bext=strchr(buffer,'.');
if(bext) {
	*bext=0;
}
strcat(fname,buffer);

ext = strchr(filename,'.');
if(ext) {
	strcpy(buffer,ext);
	buffer[4]=0;
	strcat(fname,buffer);
}
return fname;


/*
char drv[8];
char dir[128];
char fnm[128];
char ext[8];
static char newpath[20];
int flags;

flags=fnsplit(filename,drv,dir,fnm,ext);
strcpy(newpath,fnm);
if(flags&EXTENSION)
	strcat(newpath,ext);
return newpath;*/
}

INT32 getfilelength(FILE *fp) {
	long oldpos = ftell(fp);
	long len =0;
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	fseek(fp,oldpos,SEEK_SET);
	return (INT32)len;
}

void stringlower(char *a) {
for(;*a;a++)
        *a=tolower(*a);
}

void stringupper(char *a) {
for(;*a;a++)
        *a=toupper(*a);
}
