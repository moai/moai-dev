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

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>
#include <math.h>

static void mix_u8_fast(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast, mix_u8);

static void
mix_u8_fast2 (uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n>0){
		*dest++ = ((*src1 - *src2) * *scale + *src2 * 255)>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast2, mix_u8);

/* PowerPC likes this one */
static void
mix_u8_fast3(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = ((src1[i] - src2[i]) * scale[i] + src2[i] * 255)>>8;
	}
}
OIL_DEFINE_IMPL (mix_u8_fast3, mix_u8);

static void
mix_u8_unroll4(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	while(n&0x3){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n--;
	}
	while(n>0){
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		*dest++ = (*src1 * *scale + *src2 * (255 - *scale))>>8;
		src1++;
		src2++;
		scale++;
		n-=4;
	}
}
OIL_DEFINE_IMPL (mix_u8_unroll4, mix_u8);

