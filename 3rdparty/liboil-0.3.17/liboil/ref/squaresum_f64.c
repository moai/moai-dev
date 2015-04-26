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
 * oil_squaresum_f64:
 * @d:
 * @s:
 * @n:
 *
 * Sums the square of each element in @s and places the result in
 * @dest.
 */
OIL_DEFINE_CLASS (squaresum_f64, "double *d, double *s, int n");

static void
squaresum_f64_ref(double *dest, double *src, int n)
{
	double sum2 = 0;
	double errsum = 0;
	double tmp;
	double x;
	int i;

	for(i=0;i<n;i++){
		tmp = sum2;
		x = src[i]*src[i];
		sum2 += x;
		errsum += (tmp - sum2) + x;
	}

	*dest = sum2 + errsum;
}
OIL_DEFINE_IMPL_REF (squaresum_f64_ref, squaresum_f64);

/**
 * oil_squaresum_shifted_s16:
 * @d:
 * @s:
 * @n:
 *
 * Square each element in @s and divide by (1<<15), and sum the
 * results, placing the final result in @d.
 */
OIL_DEFINE_CLASS (squaresum_shifted_s16, "uint32_t *d, int16_t *s, int n");

static void
squaresum_shifted_s16_ref(uint32_t *dest, int16_t *src, int n)
{
	uint32_t sum2 = 0;
        uint32_t x;
	int i;

	for(i=0;i<n;i++){
		x = (src[i]*src[i] + (1<<14)) >> 15;
		sum2 += x;
	}

	*dest = sum2;
}
OIL_DEFINE_IMPL_REF (squaresum_shifted_s16_ref, squaresum_shifted_s16);

