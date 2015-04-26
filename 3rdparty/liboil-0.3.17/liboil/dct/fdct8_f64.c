/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003 David A. Schleef <ds@schleef.org>
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
 * oil_fdct8_f64:
 * @d_8:
 * @s_8:
 * @dstr:
 * @sstr:
 *
 * Performs a Forward Discrete Cosine Transform on @s_8 and places
 * the result in @d_8.
 */
OIL_DEFINE_CLASS (fdct8_f64, "double *d_8, double *s_8, int dstr, int sstr");

#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

static void
fdct8_f64_ref (double *dest, const double *src, int dstr, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j;
	double x;

	if(!fdct_coeff_init){
		double scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				fdct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		fdct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		x = 0;
		for(j=0;j<8;j++){
			x += fdct_coeff[j][i] * OIL_GET(src,sstr*j, double);
		}
		OIL_GET(dest,dstr*i, double) = x;
	}
}

OIL_DEFINE_IMPL_REF (fdct8_f64_ref, fdct8_f64);

/*
 * This algorithm is roughly similar to a Fast-Fourier transform,
 * taking advantage of the symmeties of the base vectors.  For
 * reference, the base vectors are (horizontally):
 *
 * 0: 1.0000  1.0000  1.0000  1.0000  1.0000  1.0000  1.0000  1.0000 
 * 1: 0.9808  0.8315  0.5556  0.1951 -0.1951 -0.5556 -0.8315 -0.9808 
 * 2: 0.9239  0.3827 -0.3827 -0.9239 -0.9239 -0.3827  0.3827  0.9239 
 * 3: 0.8315 -0.1951 -0.9808 -0.5556  0.5556  0.9808  0.1951 -0.8315 
 * 4: 0.7071 -0.7071 -0.7071  0.7071  0.7071 -0.7071 -0.7071  0.7071 
 * 5: 0.5556 -0.9808  0.1951  0.8315 -0.8315 -0.1951  0.9808 -0.5556 
 * 6: 0.3827 -0.9239  0.9239 -0.3827 -0.3827  0.9239 -0.9239  0.3827 
 * 7: 0.1951 -0.5556  0.8315 -0.9808  0.9808 -0.8315  0.5556 -0.1951 
 *
 * The symmetries of note: 
 *  - even vectors are symmetric around 4 (the middle)
 *  - odd vectors are antisymmetric around 4
 *  - 0,4 are symmertic around 2 and 6
 *  - 2,6 are antisymmetic around 2 and 6
 *
 * f0 = (x0 + x7) + (x1 + x6) + (x2 + x5) + (x3 + x4);
 * f1 = 0.9808*(x0 - x7) + 0.8315*(x1 - x6) + 0.5556*(x2 - x5) + 0.1951*(x3 - x4)
 * f2 = 0.9239*(x0 + x7) + 0.3827*(x1 + x6) - 0.3827*(x2 + x5) - 0.9239*(x3 + x4)
 * f3 = 0.8315*(x0 - x7) - 0.1951*(x1 - x6) - 0.9808*(x2 - x5) - 0.5556*(x3 - x4)
 * f4 = 0.7071*((x0 + x7) - (x1 + x6) - (x2 + x5) + (x3 + x4))
 * f5 = 0.5556*(x0 - x7) - 0.9808*(x1 - x6) + 0.1951*(x2 - x5) + 0.8315*(x3 - x4)
 * f6 = 0.3827*(x0 + x7) - 0.9239*(x1 + x6) + 0.9239*(x2 + x5) - 0.3827*(x3 + x4)
 * f7 = 0.1951*(x0 - x7) - 0.5556*(x1 - x6) + 0.8315*(x2 - x5) - 0.9808*(x3 - x4)
 *
 * The even vectors can be further simplified:
 *
 * f0 = ((x0 + x7) + (x3 + x4)) + ((x1 + x6) + (x2 + x5));
 * f2 = 0.9239*((x0 + x7) - (x3 + x4)) + 0.3827*((x1 + x6) - (x2 + x5))
 * f4 = 0.7071*((x0 + x7) + (x3 + x4)) - 0.7071*((x1 + x6) + (x2 + x5))
 * f6 = 0.3827*((x0 + x7) - (x3 + x4)) - 0.9239*((x1 + x6) - (x2 + x5))
 *
 * Some implementations move some of the normalization to a later
 * stage of processing, saving a few multiplies which get absorbed
 * into later multiplies.  However, this incurs a bit of error in
 * the integer versions of this function.  Also, if the CPU has a
 * multiply-and-add function, you don't gain anything.
 */

static void
fdct8_f64_fast(double *dest, const double *src, int dstr, int sstr)
{
	double s07, s16, s25, s34;
	double d07, d16, d25, d34;
	double ss07s34, ds07s34, ss16s25, ds16s25;

	s07 = OIL_GET(src,sstr*0,double) + OIL_GET(src,sstr*7,double);
	d07 = OIL_GET(src,sstr*0,double) - OIL_GET(src,sstr*7,double);
	s16 = OIL_GET(src,sstr*1,double) + OIL_GET(src,sstr*6,double);
	d16 = OIL_GET(src,sstr*1,double) - OIL_GET(src,sstr*6,double);
	s25 = OIL_GET(src,sstr*2,double) + OIL_GET(src,sstr*5,double);
	d25 = OIL_GET(src,sstr*2,double) - OIL_GET(src,sstr*5,double);
	s34 = OIL_GET(src,sstr*3,double) + OIL_GET(src,sstr*4,double);
	d34 = OIL_GET(src,sstr*3,double) - OIL_GET(src,sstr*4,double);

	ss07s34 = s07 + s34;
	ds07s34 = s07 - s34;
	ss16s25 = s16 + s25;
	ds16s25 = s16 - s25;

	OIL_GET(dest,dstr*0,double) = 0.5*C0_7071*(ss07s34 + ss16s25);
	OIL_GET(dest,dstr*2,double) = 0.5*(C0_9239*ds07s34 + C0_3827*ds16s25);
	OIL_GET(dest,dstr*4,double) = 0.5*C0_7071*(ss07s34 - ss16s25);
	OIL_GET(dest,dstr*6,double) = 0.5*(C0_3827*ds07s34 - C0_9239*ds16s25);

	OIL_GET(dest,dstr*1,double) = 0.5*(C0_9808*d07 + C0_8315*d16
			+ C0_5556*d25 + C0_1951*d34);
	OIL_GET(dest,dstr*3,double) = 0.5*(C0_8315*d07 - C0_1951*d16
			- C0_9808*d25 - C0_5556*d34);
	OIL_GET(dest,dstr*5,double) = 0.5*(C0_5556*d07 - C0_9808*d16
			+ C0_1951*d25 + C0_8315*d34);
	OIL_GET(dest,dstr*7,double) = 0.5*(C0_1951*d07 - C0_5556*d16
			+ C0_8315*d25 - C0_9808*d34);
 
#if 0
	z1 = (ds1  tmp12 + tmp13) * 0.707106781;
	OIL_GET(dest,dstr*2,double) = (tmp13 + z1)*(0.25*M_SQRT2)*0.765366864;
	OIL_GET(dest,dstr*6,double) = (tmp13 - z1)*(0.25*M_SQRT2)*1.847759066;

	tmp10 = tmp4 + tmp5;
	tmp11 = tmp5 + tmp6;
	tmp12 = tmp6 + tmp7;

	z5 = (tmp10 - tmp12) * 0.382683433;
	z2 = 0.541196100 * tmp10 + z5;
	z4 = 1.306562965 * tmp12 + z5;
	z3 = tmp11 * 0.707106781;

	z11 = tmp7 + z3;
	z13 = tmp7 - z3;

	OIL_GET(dest,dstr*5,double) = (z13 + z2)*(0.25*M_SQRT2)*1.2728;
	OIL_GET(dest,dstr*3,double) = (z13 - z2)*(0.25*M_SQRT2)*0.8504;
	OIL_GET(dest,dstr*1,double) = (z11 + z4)*(0.25*M_SQRT2)*0.7209;
	OIL_GET(dest,dstr*7,double) = (z11 - z4)*(0.25*M_SQRT2)*3.6245;
#endif
}
OIL_DEFINE_IMPL (fdct8_f64_fast, fdct8_f64);




