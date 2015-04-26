/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2002,2003,2004 David A. Schleef <ds@schleef.org>
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

#include <liboil/liboil.h>
#include <liboil/dct/dct.h>
#include <math.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilparameter.h>
#include <liboil/liboilrandom.h>

/**
 * SECTION:liboilfuncs-dct:
 * @title: Direct Cosine Transform
 * @short_description: DCT related functions
 */

#define BLOCK8x8_F64(ptr, stride, row, column) \
	(*((double *)((unsigned char *)ptr + stride*row) + column))

#define BLOCK8x8_PTR_F64(ptr, stride, row, column) \
	((double *)((unsigned char *)ptr + stride*row) + column)

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((unsigned char *)ptr + stride*row) + column))

static void
idct8x8_test (OilTest *test)
{
  int16_t *data = oil_test_get_source_data (test, OIL_ARG_SRC1);
  int stride = oil_test_get_value (test, OIL_ARG_SSTR1);
  int i, j;

  for(j=0;j<8;j++){
    for(i=0;i<8;i++){
      OIL_GET(data, i*2 + j*stride, int16_t) = (oil_rand_s16() & 0xfff) - 2048;
    }
  }
}

/**
 * oil_idct8x8_f64:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a 2-D Inverse Discrete Cosine Transform on @s_8x8 and places
 * the result in @d_8x8.
 */
OIL_DEFINE_CLASS (idct8x8_f64, "double *d_8x8, int dstr, double *s_8x8, int sstr");
/**
 * oil_idct8x8lim10_f64:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a 2-D Inverse Discrete Cosine Transform on @s_8x8 and places
 * the result in @d_8x8.
 */
OIL_DEFINE_CLASS (idct8x8lim10_f64, "double *d_8x8, int dstr, double *s_8x8, int sstr");
/**
 * oil_idct8x8_s16:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a limited 2-D Inverse Discrete Cosine Transform on @s_8x8
 * and places the result in @d_8x8.
 */
OIL_DEFINE_CLASS_FULL (idct8x8_s16, "int16_t *d_8x8, int dstr, int16_t *s_8x8, int sstr", idct8x8_test);
/**
 * oil_idct8x8lim10_s16:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a limited 2-D Inverse Discrete Cosine Transform on @s_8x8
 * and places the result in @d_8x8.  The source 8x8 block must be non-zero
 * only in the 10 lowest-order components.
 */
OIL_DEFINE_CLASS (idct8x8lim10_s16, "int16_t *d_8x8, int dstr, int16_t *s_8x8, int sstr");

static void
idct8x8_f64_ref (double *dest, int dstr, const double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j,k,l;
	double tmp1,tmp2;

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
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<8;k++){
				tmp2 = 0;
				for(l=0;l<8;l++){
					tmp2 += idct_coeff[j][l] *
						BLOCK8x8_F64(src,sstr,k,l);
				}
				tmp1 += idct_coeff[i][k] * tmp2;
			}
			BLOCK8x8_F64(dest,dstr,i,j) = tmp1;
		}
	}
}
OIL_DEFINE_IMPL_REF (idct8x8_f64_ref, idct8x8_f64);

static void
idct8x8lim10_f64_ref (double *dest, int dstr, const double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j,k,l;
	double tmp1,tmp2;

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
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<4;k++){
				tmp2 = 0;
				for(l=0;l<4;l++){
					tmp2 += idct_coeff[j][l] *
						BLOCK8x8_F64(src,sstr,k,l);
				}
				tmp1 += idct_coeff[i][k] * tmp2;
			}
			BLOCK8x8_F64(dest,dstr,i,j) = tmp1;
		}
	}
}
OIL_DEFINE_IMPL_REF (idct8x8lim10_f64_ref, idct8x8lim10_f64);

#if defined(oil_idct8_f64)
static void
idct8x8_f64_c (double *dest, int dstr, const double *src, int sstr)
{
	int i;
	double tmp[64];
	int tmpstr = 8*sizeof(double);

	for(i=0;i<8;i++){
		oil_idct8_f64(
			BLOCK8x8_PTR_F64(tmp,tmpstr,i,0), sizeof(double),
			BLOCK8x8_PTR_F64(src,sstr,i,0), sizeof(double));
	}
	for(i=0;i<8;i++){
		oil_idct8_f64(
			BLOCK8x8_PTR_F64(dest,dstr,0,i), dstr,
			BLOCK8x8_PTR_F64(tmp,tmpstr,0,i), tmpstr);
	}
}

OIL_DEFINE_IMPL_DEPENDS (idct8x8_f64_c, idct8x8_f64, idct8_f64);
#endif

#if defined(oil_conv8x8_f64_s16) && defined(oil_idct8x8_f64) && \
    defined(oil_conv8x8_s16_f64)
static void
idct8x8_s16_ref (int16_t *dest, int dstr, const int16_t *src, int sstr)
{
	double s[64], d[64];

	oil_conv8x8_f64_s16 (s,8*sizeof(double),src,sstr);
	oil_idct8x8_f64 (d,8*sizeof(double),s,8*sizeof(double));
	oil_conv8x8_s16_f64 (dest,dstr,d,8*sizeof(double));
}

OIL_DEFINE_IMPL_REF (idct8x8_s16_ref, idct8x8_s16);
#if 0
OIL_DEFINE_IMPL_DEPENDS (idct8x8_s16_ref, idct8x8_s16,
    conv8x8_f64_s16, idct8x8_f64, conv8x8_s16_f64);
#endif
#endif

#if defined(oil_conv8x8_f64_s16) && defined(oil_idct8x8lim10_f64) && \
    defined(oil_conv8x8_s16_f64)
static void
idct8x8lim10_s16_ref (int16_t *dest, int dstr, const int16_t *src, int sstr)
{
	double s[64], d[64];

	oil_conv8x8_f64_s16 (s,8*sizeof(double),src,sstr);
	oil_idct8x8lim10_f64 (d,8*sizeof(double),s,8*sizeof(double));
	oil_conv8x8_s16_f64 (dest,dstr,d,8*sizeof(double));
}

OIL_DEFINE_IMPL_REF (idct8x8lim10_s16_ref, idct8x8lim10_s16);
#if 0
OIL_DEFINE_IMPL_DEPENDS (idct8x8_s16_ref, idct8x8_s16,
    conv8x8_f64_s16, idct8x8_f64, conv8x8_s16_f64);
#endif
#endif

