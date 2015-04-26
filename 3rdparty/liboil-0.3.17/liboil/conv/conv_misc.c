/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2003,2004 David A. Schleef <ds@schleef.org>
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
#include "conv.h"

#include <math.h>


static void
conv_f64_s16_table(double *dest, int dest_stride, const short *src,
	int src_stride, int n)
{
	static double ints_high[256];
	static double ints_low[256];
	static int init = 0;
	int i;
	unsigned int idx;
	if(!init){
		for(i=0;i<256;i++){
			ints_high[i]=256.0*((i<128)?i:i-256);
			ints_low[i]=i;
		}
		init = 1;
	}

	if(n&1){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		n-=1;
	}
	for(i=0;i<n;i+=2){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_table, conv_f64_s16);

static void
conv_f32_s16_table(float *dest, int dest_stride, const short *src,
	int src_stride, int n)
{
	static float ints_high[256];
	static float ints_low[256];
	static int init = 0;
	int i;
	unsigned int idx;
	if(!init){
		for(i=0;i<256;i++){
			ints_high[i]=256.0*((i<128)?i:i-256);
			ints_low[i]=i;
		}
		init = 1;
	}

	if(n&1){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		n-=1;
	}
	for(i=0;i<n;i+=2){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_s16_table, conv_f32_s16);



