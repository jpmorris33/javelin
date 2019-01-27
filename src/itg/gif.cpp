#include "itg.hpp"
#include <gif_lib.h>
#include <stdlib.h>
#include <string.h>

static long _pos,_len;
unsigned char *_ptr;

int readFunc(GifFileType *handle, GifByteType *buffer, int length) {
	if(_pos + length >= _len) {
		length=_len-_pos;
	}
	memcpy(buffer,_ptr,length);
	_ptr+=length;
	_pos+=length;

	return length;
}

int memGIF(unsigned char *src,unsigned char *dest,palette pal,unsigned int len)
{
	int ctr;
	unsigned char pall[768];
	GifFileType *gifhandle = NULL;
	GifRecordType record;
	GifByteType *ext;
	int extcode;
	int error=0;
	_pos = 0;
	_len = len;
	_ptr = src;

	// First rip the palette out of the file
	for (ctr = 0; ctr < 768; ctr++) {
		pall[ctr]=src[13+ctr]>>2; // convert to 6-bit RGB for VGA
	}

	gifhandle=DGifOpen(src,readFunc,&error);
	if(!gifhandle) {
		printf("Error %d opening gif\n",error);
		return 0;
	}

	if (gifhandle->SHeight != 200 || gifhandle->SWidth != 320) {
		DGifCloseFile(gifhandle,&error);
		printf("Background GIF files must be 320x200\n");
		return 0;
    	}

	if (gifhandle->Image.Interlace) {
		DGifCloseFile(gifhandle,&error);
		printf("Interlaced GIF not supported\n");
		return 0;
    	}

	do	{
		if(DGifGetRecordType(gifhandle,&record) == GIF_ERROR) {
			DGifCloseFile(gifhandle,&error);
			printf("GIF record error %d\n", gifhandle->Error);
			return 0;
		}
//		printf("Record %d\n",record);

		switch(record) {
			case IMAGE_DESC_RECORD_TYPE:
				DGifGetImageDesc(gifhandle);
				for(int h=0;h<gifhandle->Image.Height;h++) {
					DGifGetLine(gifhandle,dest,gifhandle->Image.Width);
					dest += 320;
				}
				break;

			case EXTENSION_RECORD_TYPE:
				ext = NULL;
				DGifGetExtension(gifhandle, &extcode, &ext);
				while (ext != NULL) {
					ext=NULL;
		    			DGifGetExtensionNext(gifhandle, &ext);
				}
				break;

			case TERMINATE_RECORD_TYPE:
				break;
	    	}
	} while(record != TERMINATE_RECORD_TYPE);

	DGifCloseFile(gifhandle,&error);

	if(pal==NULL)
		pal_set(pall);
	else
		memmove(pal,pall,768);

return 1;
}

