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
#include <liboil/liboilfuncs.h>
#include <liboil/dct/dct.h>



#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

/**
 * oil_fdct8x8s_s16:
 * @d_8x8:
 * @ds:
 * @s_8x8:
 * @ss:
 *
 * Performs a 2-D Forward Discrete Cosine Transform on @s_8x8 and places
 * the result in @d_8x8.
 *
 * This function uses an alternate scaling used by RTjpeg.
 */
OIL_DEFINE_CLASS(fdct8x8s_s16,
    "int16_t *d_8x8, int ds, int16_t *s_8x8, int ss");

#if defined(oil_fdct8x8_f64) && defined(oil_conv8x8_s16_f64)
static void
fdct8x8s_s16_ref (int16_t *dest, int dstr, const int16_t *src, int sstr)
{
	double s[64], d[64];
	double scale[8] = {
		4*C0_7071,
		4*C0_9808,
		4*C0_9239,
		4*C0_8315,
		4*C0_7071,
		4*C0_5556,
		4*C0_3827,
		4*C0_1951,
	};
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			s[8*i+j] = OIL_GET (src,sstr*i+j*sizeof(int16_t), int16_t);
		}
	}

	oil_fdct8x8_f64(d,8*sizeof(double),s,8*sizeof(double));

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			d[8*i+j] *= scale[i] * scale[j];
		}
	}

	oil_conv8x8_s16_f64(dest,dstr,d,8*sizeof(double));
}

OIL_DEFINE_IMPL_REF (fdct8x8s_s16_ref, fdct8x8s_s16);
#endif


