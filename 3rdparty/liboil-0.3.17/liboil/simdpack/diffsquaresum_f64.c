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
diffsquaresum_f64_i10_simple(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum = 0;
	double x;
	int i;

	for(i=0;i<n;i++){
		x = OIL_GET(src1, i*sstr1, double) -
                  OIL_GET(src2, i*sstr2, double);
		x = x*x;
		sum += x;
	}

	*dest = sum;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_simple, diffsquaresum_f64);

static void
diffsquaresum_f64_i10_fast(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum0 = 0;
	double x;

	while(n>0){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n--;
	}

	*dest = sum0;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_fast, diffsquaresum_f64);

static void
diffsquaresum_f64_i10_unroll2(double *dest, double *src1, int sstr1, double *src2,
    int sstr2, int n)
{
	double sum0 = 0;
	double sum1 = 0;
	double x;

	if(n&1){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n--;
	}
	while(n>0){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum1 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n-=2;
	}

	*dest = sum0 + sum1;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_unroll2, diffsquaresum_f64);

static void
diffsquaresum_f64_i10_unroll4(double *dest, double *src1, int sstr1,
	double *src2, int sstr2, int n)
{
	double sum0 = 0;
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double x;

	while(n&3){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n--;
	}
	while(n>0){
		x = *src1 - *src2;
		sum0 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum1 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum2 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		x = *src1 - *src2;
		sum3 += x * x;
		OIL_INCREMENT (src1, sstr1);
		OIL_INCREMENT (src2, sstr2);
		n-=4;
	}

	*dest = sum0 + sum1 + sum2 + sum3;
}
OIL_DEFINE_IMPL (diffsquaresum_f64_i10_unroll4, diffsquaresum_f64);


