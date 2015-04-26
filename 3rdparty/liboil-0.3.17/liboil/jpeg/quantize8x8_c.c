/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include "jpeg.h"

/**
 * oil_dequantize8x8_s16:
 * @d_8x8:
 * @dstr:
 * @s1_8x8:
 * @sstr1:
 * @s2_8x8:
 *
 * Multiplies each element in @s1_8x8 by the corresponding element
 * in @s2_8x8 and places the result in @d_8x8.
 */
OIL_DEFINE_CLASS (dequantize8x8_s16, 
    "int16_t *d_8x8, int dstr, int16_t *s1_8x8, int sstr1, "
    "int16_t *s2_8x8, int sstr2");

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((unsigned char *)ptr + stride*row) + column))

static void
dequantize8x8_s16_ref (int16_t *dest, int dstr, int16_t *src, int sstr,
    int16_t *quant, int qstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			BLOCK8x8_S16(dest,dstr,i,j) =
				BLOCK8x8_S16(src,sstr,i,j) *
				BLOCK8x8_S16(quant,qstr,i,j);
		}
	}
}

OIL_DEFINE_IMPL_REF (dequantize8x8_s16_ref, dequantize8x8_s16);

