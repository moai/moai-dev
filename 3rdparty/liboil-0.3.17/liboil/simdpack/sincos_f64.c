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
#include <liboil/liboilclasses.h>
#include <math.h>

/* XXX This gets more inaccurate as N increases */
static void
sincos_f64_i20_fast(double *dest_sin, double *dest_cos, int n, double *offset,
    double *interval)
{
	int i;
	double c,s,dc,ds,temp_s,temp_c;

	dest_sin[0] = s = sin(*offset);
	dest_cos[0] = c = cos(*offset);
	ds = sin(*interval);
	dc = cos(*interval);
	for(i=1;i<n;i++){
		temp_s = s*dc + c*ds;
		temp_c = c*dc - s*ds;
		dest_sin[i] = s = temp_s;
		dest_cos[i] = c = temp_c;
	}
}
OIL_DEFINE_IMPL (sincos_f64_i20_fast, sincos_f64);

#ifdef HAVE_SINCOS
static void
sincos_f64_sincos (double *dest_sin, double *dest_cos, int n, double *offset,
    double *interval)
{
	int i;

	for(i=0;i<n;i++){
          sincos (*offset + *interval * i, dest_sin + i, dest_cos + i);
	}
}
OIL_DEFINE_IMPL (sincos_f64_sincos, sincos_f64);
#endif

