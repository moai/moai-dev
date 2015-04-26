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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>


/**
 * oil_resample_linear_u8:
 * @d_n:
 * @s_2xn:
 * @n:
 * @i_2:
 *
 * Linearly resamples a row of pixels.  FIXME.
 */
static void
resample_linear_u8_test (OilTest *test)
{
  uint32_t *in = (uint32_t *) oil_test_get_source_data (test, OIL_ARG_INPLACE1);

  in[0] = 0;
  in[1] = 65536;
}
OIL_DEFINE_CLASS_FULL (resample_linear_u8,
    "uint8_t *d_n, uint8_t *s_2xn, int n, uint32_t *i_2",
    resample_linear_u8_test);

/**
 * oil_resample_linear_argb:
 * @d_n:
 * @s_2xn:
 * @n:
 * @i_2:
 *
 * Linearly resamples a row of pixels.  FIXME.
 */
static void
resample_linear_argb_test (OilTest *test)
{
  uint32_t *in = (uint32_t *) oil_test_get_source_data (test, OIL_ARG_INPLACE1);

  in[0] = 0;
  in[1] = 65536;
}
OIL_DEFINE_CLASS_FULL (resample_linear_argb,
    "uint32_t *d_n, uint32_t *s_2xn, int n, uint32_t *i_2",
    resample_linear_argb_test);

static void
resample_linear_u8_ref (uint8_t *dest, uint8_t *src, int n,
    uint32_t *in)
{
  int acc = in[0];
  int increment = in[1];
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    j = acc>>16;
    x = (acc&0xffff)>>8;
    dest[i] = (src[j]*(256-x) + src[j+1]*x) >> 8;

    acc += increment;
  }

  in[0] = acc;
}
OIL_DEFINE_IMPL_REF (resample_linear_u8_ref, resample_linear_u8);

static void
resample_linear_argb_ref (uint32_t *d, uint32_t *s, int n, uint32_t *in)
{
  uint8_t *src = (uint8_t *)s;
  uint8_t *dest = (uint8_t *)d;
  int acc = in[0];
  int increment = in[1];
  int i;
  int j;
  int x;

  for(i=0;i<n;i++){
    j = acc>>16;
    x = (acc&0xffff)>>8;
    dest[4*i+0] = (src[4*j+0]*(256-x) + src[4*j+4]*x) >> 8;
    dest[4*i+1] = (src[4*j+1]*(256-x) + src[4*j+5]*x) >> 8;
    dest[4*i+2] = (src[4*j+2]*(256-x) + src[4*j+6]*x) >> 8;
    dest[4*i+3] = (src[4*j+3]*(256-x) + src[4*j+7]*x) >> 8;

    acc += increment;
  }

  in[0] = acc;
}
OIL_DEFINE_IMPL_REF (resample_linear_argb_ref, resample_linear_argb);


static void
merge_linear_test (OilTest *test)
{
  uint32_t *src3 = (uint32_t *) oil_test_get_source_data (test, OIL_ARG_SRC3);

  do {
    src3[0] = oil_rand_u16() & 0x1ff;
  } while (src3[0] > 256);
}
OIL_DEFINE_CLASS_FULL (merge_linear_argb,
    "uint32_t *d_n, uint32_t *s_n, uint32_t *s2_n, uint32_t *s3_1, int n",
    merge_linear_test);
OIL_DEFINE_CLASS_FULL (merge_linear_u8,
    "uint8_t *d_n, uint8_t *s_n, uint8_t *s2_n, uint32_t *s3_1, int n",
    merge_linear_test);

/**
 * oil_merge_linear_argb:
 * @d_n:
 * @s_n:
 * @s2_n:
 * @s3_1:
 * @n:
 *
 * Linearly interpolate the @s_n and @s2_n arrays using the scale
 * factor in @s3_1.  The value @s3_1 must be in the range [0, 256]
 * A value of 0 indicates weights of 1.0 and 0.0 for
 * the s_n and s2_n arrays respectively.  A value of 256 indicates
 * weights of 0.0 and 1.0 respectively.
 *
 * This function is not intended for alpha blending; use one of the
 * compositing functions instead.
 */
static void
merge_linear_argb_ref (uint32_t *d, uint32_t *s1, uint32_t *s2,
    uint32_t *src3, int n)
{
  uint8_t *src1 = (uint8_t *)s1;
  uint8_t *src2 = (uint8_t *)s2;
  uint8_t *dest = (uint8_t *)d;
  int i;
  int x = src3[0];

  for(i=0;i<n;i++){
    dest[4*i+0] = (src1[4*i+0]*(256-x) + src2[4*i+0]*x) >> 8;
    dest[4*i+1] = (src1[4*i+1]*(256-x) + src2[4*i+1]*x) >> 8;
    dest[4*i+2] = (src1[4*i+2]*(256-x) + src2[4*i+2]*x) >> 8;
    dest[4*i+3] = (src1[4*i+3]*(256-x) + src2[4*i+3]*x) >> 8;
  }
}
OIL_DEFINE_IMPL_REF (merge_linear_argb_ref, merge_linear_argb);


/**
 * oil_merge_linear_u8:
 * @d_n:
 * @s_n:
 * @s2_n:
 * @s3_1:
 * @n:
 *
 * Linearly interpolate the @s_n and @s2_n arrays using the scale
 * factor in @s3_1.  The value @s3_1 must be in the range [0, 255].
 * The value translates into weights of 1.0-(value/256.0) and
 * (value/256.0) for the s_n and s2_n arrays respectively.
 *
 * This function is not intended for alpha blending; use one of the
 * compositing functions instead.
 */
static void
merge_linear_u8_ref (uint8_t *dest, uint8_t *src1, uint8_t *src2,
    uint32_t *src3, int n)
{
  int i;
  int x = src3[0];

  for(i=0;i<n;i++){
    dest[i] = (src1[i]*(256-x) + src2[i]*x) >> 8;
  }
}
OIL_DEFINE_IMPL_REF (merge_linear_u8_ref, merge_linear_u8);


