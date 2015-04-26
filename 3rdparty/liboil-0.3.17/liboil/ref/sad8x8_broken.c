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


/**
 * oil_sad8x8_s16:
 * @d_8x8:
 * @ds:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * FIXME: This function is broken and has been replaced by
 * @oil_sad8x8_s16_2() because the destination of this function
 * is an 8x8 block instead of a single value.
 */
OIL_DEFINE_CLASS (sad8x8_s16,
    "uint32_t *d_8x8, int ds, int16_t *s1_8x8, int ss1, int16_t *s2_8x8, int ss2");
/**
 * oil_sad8x8_f64:
 * @d_8x8:
 * @ds:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * FIXME: This function is broken and has been replaced by
 * @oil_sad8x8_f64_2() because the destination of this function
 * is an 8x8 block instead of a single value.
 */
OIL_DEFINE_CLASS (sad8x8_f64,
    "double *d_8x8, int ds, double *s1_8x8, int ss1, double *s2_8x8, int ss2");

static void
sad8x8_f64_ref(double *dest, int dstr, double *src1, int sstr1, double *src2,
    int sstr2)
{
	int i,j;
	double sum;

	sum = 0;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			sum += fabs(OIL_GET(src1,sstr1*i+j*sizeof(double), double) -
			    OIL_GET(src2,sstr2*i+j*sizeof(double), double));
		}
	}
	*dest = sum;
}

OIL_DEFINE_IMPL_REF(sad8x8_f64_ref, sad8x8_f64);

static void
sad8x8_s16_ref(uint32_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2,
    int sstr2)
{
	int i,j;
	int d;
	uint32_t sum;

	sum = 0;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			d = ((int)OIL_GET(src1,sstr1*i+j*sizeof(int16_t), int16_t)) -
				((int)OIL_GET(src2,sstr2*i+j*sizeof(int16_t), int16_t));
			sum += (d<0) ? -d : d;
		}
	}
	*dest = sum;
}

OIL_DEFINE_IMPL_REF(sad8x8_s16_ref, sad8x8_s16);

