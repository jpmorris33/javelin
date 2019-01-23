#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH 32767
typedef int32_t INT32;


void F_error(char *e,char *e2);

char *databuf;
char *jugname;

int main(int argc,char *argv[])
{
FILE *fp,*fp2,*fpO;
char entry[25];
INT32 leng,lediv,lerem;
unsigned long lctr;
unsigned short sig;

puts("Javelin JUG decompiler 1.1 (C)1995-2019 IT-HE Software\n");
puts("\nTakes a JUG file, and produces list and component files");
if(argc<3)
	{
	puts("Syntax: makejug <jugfile> <output listfile>");
	return 1;
	}

jugname=argv[1];

databuf=(char *)malloc(HASH);
if(!databuf)
	F_error("Out of memory.",NULL);

fp=fopen(jugname,"rb");
if(!fp)
	F_error("Cannot open JUGfile:",jugname);
fread(&sig,1,2,fp);
if(sig!=0x7074+0x400)	//0x747.400 jug version no
	{
	puts("Not a type 4 .JUG file.");
	exit(1);
	}

fp2=fopen(argv[2],"w");
if(!fp2)
	F_error("Cannot open new list file:",argv[2]);

fprintf(fp2,"#%s\n#\n",argv[2]);

do {
	if(!fread(entry,1,13,fp))
		break;
	if(!fread(&leng,4,1,fp))
		return 0;

	fprintf(fp2,"%s\n",entry);

	lediv=leng/HASH;
	lerem=leng%HASH;
//		strcpy(string,protectfilename(string));

	fpO=fopen(entry,"wb");
	if(!fpO)
		F_error("Cannot create file:",entry);

	for(lctr=0;lctr<lediv;lctr++)
		{
		fread(databuf,1,HASH,fp);
		fwrite(databuf,1,HASH,fpO);
		}
	if(lerem)
		{
		fread(databuf,1,lerem,fp);
		fwrite(databuf,1,lerem,fpO);
		}
	fclose(fpO);
	printf("%s written\n",entry);

	} while(!feof(fp));

fclose(fp2);
fclose(fp);
puts("Done.");
}

void F_error(char *e,char *e2)
{
printf("%s %s\n",e,e2);
exit(1);
}
