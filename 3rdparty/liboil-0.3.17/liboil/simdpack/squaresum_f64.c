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
squaresum_f64_i10_simple(double *dest, double *src, int n)
{
	double sum2 = 0;
	int i;

	for(i=0;i<n;i++){
		sum2 += src[i]*src[i];
	}

	*dest = sum2;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_simple, squaresum_f64);

#if 0
#include <multsum_f64.h>
static void
squaresum_f64_i10_multsum(double *dest, double *src, int n)
{
	multsum_f64(dest,src,src,n);
}
#endif

static void
squaresum_f64_i10_unroll4a(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;

	while(n&0x3){
		sum1 += *src * *src;
		src++;
		n--;
	}
	while(n>0){
		sum1 += *src * *src;
		src++;
		sum2 += *src * *src;
		src++;
		sum3 += *src * *src;
		src++;
		sum4 += *src * *src;
		src++;
		n-=4;
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll4a, squaresum_f64);

static void
squaresum_f64_i10_unroll4(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	int i;

	while(n&0x3){
		sum1 += src[0]*src[0];
		src++;
		n--;
	}
	for(i=0;i<n;i+=4){
		sum1 += src[i]*src[i];
		sum2 += src[i+1]*src[i+1];
		sum3 += src[i+2]*src[i+2];
		sum4 += src[i+3]*src[i+3];
	}

	*dest = sum1 + sum2 + sum3 + sum4;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll4, squaresum_f64);

static void
squaresum_f64_i10_unroll8(double *dest, double *src, int n)
{
	double sum1 = 0;
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	double sum5 = 0;
	double sum6 = 0;
	double sum7 = 0;
	double sum8 = 0;
	int i;

	while(n&0x7){
		sum1 += src[0]*src[0];
		src++;
		n--;
	}
	for(i=0;i<n;i+=8){
		sum1 += src[i]*src[i];
		sum2 += src[i+1]*src[i+1];
		sum3 += src[i+2]*src[i+2];
		sum4 += src[i+3]*src[i+3];
		sum5 += src[i+4]*src[i+4];
		sum6 += src[i+5]*src[i+5];
		sum7 += src[i+6]*src[i+6];
		sum8 += src[i+7]*src[i+7];
	}

	*dest = sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
}
OIL_DEFINE_IMPL (squaresum_f64_i10_unroll8, squaresum_f64);

