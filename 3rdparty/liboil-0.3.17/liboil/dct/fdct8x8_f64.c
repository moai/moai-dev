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
#include <liboil/liboilfuncs.h>
#include <liboil/dct/dct.h>
#include <math.h>


/**
 * oil_fdct8x8_f64:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a 2-D Forward Discrete Cosine Transform on @s_8x8 and places
 * the result in @d_8x8.
 */
OIL_DEFINE_CLASS(fdct8x8_f64,
    "double *d_8x8, int dstr, double *s_8x8, int sstr");


static void
fdct8x8_f64_ref(double *dest, int dstr, const double *src, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j,k,l;
	double tmp1,tmp2;

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
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<8;k++){
				tmp2 = 0;
				for(l=0;l<8;l++){
					tmp2 += fdct_coeff[l][j] *
					  OIL_GET (src, sstr*k + l * sizeof(double),
                                              double);
				}
				tmp1 += fdct_coeff[k][i] * tmp2;
			}
			OIL_GET (dest, dstr*i + j*sizeof(double), double) =
                          tmp1;
		}
	}
}

OIL_DEFINE_IMPL_REF (fdct8x8_f64_ref, fdct8x8_f64);

static void
fdct8x8_f64_ref2(double *dest, int dstr, const double *src, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j,k;
	double x;
	double tmp[64];

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
		for(j=0;j<8;j++){
			x = 0;
			for(k=0;k<8;k++){
				x += fdct_coeff[k][j] *
				  OIL_GET (src, sstr*i + k * sizeof(double), double);
			}
			tmp[8*i+j] = x;
		}
	}

	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			x = 0;
			for(k=0;k<8;k++){
				x += fdct_coeff[k][i] * tmp[8*k + j];
			}
			OIL_GET (dest,dstr*i+j*sizeof(double), double) = x;
		}
	}
}

OIL_DEFINE_IMPL (fdct8x8_f64_ref2, fdct8x8_f64);

#ifdef oil_fdct8_f64
static void
fdct8x8_f64_1d (double *dest, int dstr, const double *src, int sstr)
{
	int i;
	double tmp[64];

	for(i=0;i<8;i++){
		oil_fdct8_f64(tmp + i*8, OIL_OFFSET(src,sstr*i),
                    sizeof (double), sizeof(double));
	}

	for(i=0;i<8;i++){
		oil_fdct8_f64(dest + i, tmp + i,
                    dstr, 8*sizeof(double));
	}
}
OIL_DEFINE_IMPL (fdct8x8_f64_1d, fdct8x8_f64);
#endif


