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
 * oil_mult8x8_s16:
 * @d_8x8:
 * @s1_8x8:
 * @s2_8x8:
 * @ds:
 * @ss1:
 * @ss2:
 *
 * Multiplies each element in @s1_8x8 and @s2_8x8 and places the result
 * in @d_8x8.
 */
OIL_DEFINE_CLASS (mult8x8_s16, 
    "int16_t *d_8x8, int16_t *s1_8x8, int16_t *s2_8x8, int ds, int ss1, int ss2");

static void
mult8x8_s16_ref(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1,
    int sstr2)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			OIL_GET(dest,dstr*i+2*j, int16_t) =
				OIL_GET(src1,sstr1*i+2*j, int16_t) *
				OIL_GET(src2,sstr2*i+2*j, int16_t);
		}
	}
}

OIL_DEFINE_IMPL_REF (mult8x8_s16_ref, mult8x8_s16);

