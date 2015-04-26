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
 * oil_sum_f64:
 * @d_1: destination
 * @s: source array
 * @sstr: stride of source elements
 * @n: number of elements
 *
 * Sums the elements in the source array and places the result in
 * @d.
 */
OIL_DEFINE_CLASS (sum_f64, "double *d_1, double *s, int sstr, int n");
//OIL_DEFINE_CLASS (sum_f64_i10, "double *dest, double *src, int sstr, int n");

static void
sum_f64_ref (double *dest, double *src, int sstr, int n)
{
	double sum = 0;
	double errsum = 0;
	double tmp;
	int i;

	for(i=0;i<n;i++){
		tmp = sum;
		sum += OIL_GET(src, sstr*i, double);
		errsum += (tmp - sum) + OIL_GET(src, sstr*i, double);
	}

	*dest = sum + errsum;
}
OIL_DEFINE_IMPL_REF (sum_f64_ref, sum_f64);


