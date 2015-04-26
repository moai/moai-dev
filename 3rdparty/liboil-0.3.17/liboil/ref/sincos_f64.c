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
#include <math.h>

/**
 * oil_sincos_f64:
 * @dest1:
 * @dest2:
 * @n:
 * @s1_1:
 * @s2_1:
 *
 * Calculates sin(x) and cos(x) and places the results in @dest1 and
 * @dest2 respectively.  Values for x start at @s1_1 and are incremented
 * by @s2_1 for each destination element.
 */
OIL_DEFINE_CLASS (sincos_f64,
    "double *dest1, double *dest2, int n, double *s1_1, double *s2_1");

static void
sincos_f64_ref (double *dest_sin, double *dest_cos, int n, double *offset,
    double *interval)
{
	int i;
        double x;

	for(i=0;i<n;i++){
          x = *offset + *interval * i;
		dest_sin[i] = sin(x);
		dest_cos[i] = cos(x);
	}
}
OIL_DEFINE_IMPL_REF (sincos_f64_ref, sincos_f64);

