/**
 * tga_reader.h
 * 
 * Copyright (c) 2014 Kenji Sasaki
 * Released under the MIT license.
 * https://github.com/npedotnet/TGAReader/blob/master/LICENSE
 * 
 * English document
 * https://github.com/npedotnet/TGAReader/blob/master/README.md
 * 
 * Japanese document
 * http://3dtech.jp/wiki/index.php?TGAReader
 * 
 */

#ifndef __TGA_READER_H__
#define __TGA_READER_H__

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _TGA_ORDER {
	int redShift;
	int greenShift;
	int blueShift;
	int alphaShift;
} TGA_ORDER;

extern const TGA_ORDER *TGA_READER_ARGB;
extern const TGA_ORDER *TGA_READER_ABGR;

void *tgaMalloc(size_t size);
void tgaFree(void *memory);

int tgaGetWidth(const unsigned char *buffer);
int tgaGetHeight(const unsigned char *buffer);
int *tgaRead(const unsigned char *buffer, const TGA_ORDER *order);

#ifdef __cplusplus
}
#endif

#endif /* __TGA_READER_H__ */
