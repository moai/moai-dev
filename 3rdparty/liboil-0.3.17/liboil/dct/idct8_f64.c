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
#include <liboil/dct/dct.h>
#include <math.h>

/**
 * oil_idct8_f64:
 * @d_8:
 * @dstr:
 * @s_8:
 * @sstr:
 *
 * Performs a Inverse Discrete Cosine Transform on @s_8 and places
 * the result in @d_8.
 */
OIL_DEFINE_CLASS (idct8_f64, "double *d_8, int dstr, double *s_8, int sstr");

#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

static void idct8_f64_ref(double *dest, int dstr, const double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j;
	double x;

	if(!idct_coeff_init){
		double scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				idct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		idct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		x = 0;
		for(j=0;j<8;j++){
			x += idct_coeff[i][j] * OIL_GET (src, sstr*j, double);
		}
		OIL_GET (dest, dstr*i, double) = x;
	}
}

OIL_DEFINE_IMPL_REF (idct8_f64_ref, idct8_f64);


static void idct8_f64_fastx(double *dest, int dstr, const double *src, int sstr)
{
	double s07, s16, s25, s34;
	double d07, d16, d25, d34;
	double ss07s34, ss16s25;
	double ds07s34, ds16s25;

	ss07s34 = C0_7071*(OIL_GET(src,sstr*0, double) + OIL_GET(src,sstr*4, double));
	ss16s25 = C0_7071*(OIL_GET(src,sstr*0, double) - OIL_GET(src,sstr*4, double));

	ds07s34 = C0_9239* OIL_GET(src,sstr*2, double) + C0_3827* OIL_GET(src,sstr*6, double);
	ds16s25 = C0_3827* OIL_GET(src,sstr*2, double) - C0_9239* OIL_GET(src,sstr*6, double);

	s07 = ss07s34 + ds07s34;
	s34 = ss07s34 - ds07s34;

	s16 = ss16s25 + ds16s25;
	s25 = ss16s25 - ds16s25;

	d07 = C0_9808* OIL_GET(src,sstr*1, double) + C0_8315* OIL_GET(src,sstr*3, double)
		+ C0_5556* OIL_GET(src,sstr*5, double) + C0_1951* OIL_GET(src,sstr*7, double);
	d16 = C0_8315* OIL_GET(src,sstr*1, double) - C0_1951* OIL_GET(src,sstr*3, double)
		- C0_9808* OIL_GET(src,sstr*5, double) - C0_5556* OIL_GET(src,sstr*7, double);
	d25 = C0_5556* OIL_GET(src,sstr*1, double) - C0_9808* OIL_GET(src,sstr*3, double)
		+ C0_1951* OIL_GET(src,sstr*5, double) + C0_8315* OIL_GET(src,sstr*7, double);
	d34 = C0_1951* OIL_GET(src,sstr*1, double) - C0_5556* OIL_GET(src,sstr*3, double)
		+ C0_8315* OIL_GET(src,sstr*5, double) - C0_9808* OIL_GET(src,sstr*7, double);

	OIL_GET(dest,dstr*0, double) = 0.5 * (s07 + d07);
	OIL_GET(dest,dstr*1, double) = 0.5 * (s16 + d16);
	OIL_GET(dest,dstr*2, double) = 0.5 * (s25 + d25);
	OIL_GET(dest,dstr*3, double) = 0.5 * (s34 + d34);
	OIL_GET(dest,dstr*4, double) = 0.5 * (s34 - d34);
	OIL_GET(dest,dstr*5, double) = 0.5 * (s25 - d25);
	OIL_GET(dest,dstr*6, double) = 0.5 * (s16 - d16);
	OIL_GET(dest,dstr*7, double) = 0.5 * (s07 - d07);

}

OIL_DEFINE_IMPL (idct8_f64_fastx, idct8_f64);


