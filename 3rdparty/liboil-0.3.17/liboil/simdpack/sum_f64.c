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

static void
sum_f64_i10_simple (double *dest, double *src, int sstr, int n)
{
	double sum = 0;
	int i;

	for(i=0;i<n;i++){
		sum += OIL_GET(src, sstr*i, double);
	}

	*dest = sum;
}
OIL_DEFINE_IMPL (sum_f64_i10_simple, sum_f64);

static void
sum_f64_i10_unroll4 (double *dest, double *src, int sstr, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	int i;

	while (n&3) {
		sum1 += *src;
		OIL_INCREMENT (src, sstr);
		n--;
	}
	for(i=0;i<n;i+=4){
		sum1 += OIL_GET(src, sstr*i, double);
		sum2 += OIL_GET(src, sstr*(i+1), double);
		sum3 += OIL_GET(src, sstr*(i+2), double);
		sum4 += OIL_GET(src, sstr*(i+3), double);
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL (sum_f64_i10_unroll4, sum_f64);


