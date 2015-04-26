
#ifndef _JPEG_INTERNAL_H_
#define _JPEG_INTERNAL_H_

#include "jpeg.h"
#include "jpeg_rgb_decoder.h"
#include "bits.h"


#define JPEG_DEBUG(n, format...)	do{ \
	if((n)<=JPEG_DEBUG_LEVEL)jpeg_debug((n),format); \
}while(0)
#define JPEG_DEBUG_LEVEL 4


struct jpeg_rgb_decoder_struct {
	JpegDecoder *dec;

	unsigned char *image;
	int height, width;

	struct{
		unsigned char *image;
		int rowstride;
		int h_subsample;
		int v_subsample;
		int alloc;
	}component[3];
};

/* jpeg_rgb_decoder.c */


#endif


