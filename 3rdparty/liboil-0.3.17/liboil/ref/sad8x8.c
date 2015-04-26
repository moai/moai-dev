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
 * oil_sad8x8_u8:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * Calculates the sum of absolute differences between elements in @s1_8x8
 * and @s2_8x8, and places the result in @d_1.
 */
OIL_DEFINE_CLASS (sad8x8_u8,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2");
/**
 * oil_sad8x8_s16_2:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * Calculates the sum of absolute differences between elements in @s1_8x8
 * and @s2_8x8, and places the result in @d_1.
 */
OIL_DEFINE_CLASS (sad8x8_s16_2,
    "uint32_t *d_1, int16_t *s1_8x8, int ss1, int16_t *s2_8x8, int ss2");
/**
 * oil_sad8x8_f64_2:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * Calculates the sum of absolute differences between elements in @s1_8x8
 * and @s2_8x8, and places the result in @d_1.
 */
OIL_DEFINE_CLASS (sad8x8_f64_2,
    "double *d_1, double *s1_8x8, int ss1, double *s2_8x8, int ss2");
/**
 * oil_sad8x8_8xn_u8:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8xnp7:
 * @ss2:
 * @n:
 *
 * Calculates the sum of absolute differences between elements in @s1_8x8
 * and the 8x8 block of array elements starting at row i in @s2_8x8, and
 * places the result in @d_n.
 */
OIL_DEFINE_CLASS (sad8x8_8xn_u8,
    "uint32_t *d_n, uint8_t *s1_8x8, int ss1, uint8_t *s2_8xnp7, int ss2, int n");

/**
 * oil_sad12x12_u8:
 * @d_1:
 * @s1_12x12:
 * @ss1:
 * @s2_12x12:
 * @ss2:
 *
 * Calculates the sum of absolute differences between elements in @s1_12x12
 * and @s2_12x12, and places the result in @d_1.
 */
OIL_DEFINE_CLASS (sad12x12_u8,
    "uint32_t *d_1, uint8_t *s1_12x12, int ss1, uint8_t *s2_12x12, int ss2");
/**
 * oil_sad12x12_12xn_u8:
 * @d_1:
 * @s1_12x12:
 * @ss1:
 * @s2_12xnp11:
 * @ss2:
 * @n:
 *
 * Calculates the sum of absolute differences between elements in @s1_12x12
 * and the 12x12 block of array elements starting at row i in @s2_12x12, and
 * places the result in @d_n.
 */
OIL_DEFINE_CLASS (sad12x12_12xn_u8,
    "uint32_t *d_n, uint8_t *s1_12x12, int ss1, uint8_t *s2_12xnp11, int ss2, int n");
/**
 * oil_sad16x16_u8:
 * @d_1:
 * @s1_16x16:
 * @ss1:
 * @s2_16x16:
 * @ss2:
 *
 * Calculates the sum of absolute differences between elements in @s1_16x16
 * and @s2_16x16, and places the result in @d_1.
 */
OIL_DEFINE_CLASS (sad16x16_u8,
    "uint32_t *d_1, uint8_t *s1_16x16, int ss1, uint8_t *s2_16x16, int ss2");
/**
 * oil_sad16x16_16xn_u8:
 * @d_1:
 * @s1_16x16:
 * @ss1:
 * @s2_16xnp15:
 * @ss2:
 * @n:
 *
 * Calculates the sum of absolute differences between elements in @s1_16x16
 * and the 16x16 block of array elements starting at row i in @s2_16x16, and
 * places the result in @d_n.
 */
OIL_DEFINE_CLASS (sad16x16_16xn_u8,
    "uint32_t *d_n, uint8_t *s1_16x16, int ss1, uint8_t *s2_16xnp15, int ss2, int n");


static void
sad8x8_f64_2_ref (double *dest, double *src1, int sstr1, double *src2,
    int sstr2)
{
  int i, j;
  double sum;

  sum = 0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      sum += fabs (OIL_GET (src1, sstr1 * i + j * sizeof (double), double) -
          OIL_GET (src2, sstr2 * i + j * sizeof (double), double));
    }
  }
  *dest = sum;
}

OIL_DEFINE_IMPL_REF (sad8x8_f64_2_ref, sad8x8_f64_2);

static void
sad8x8_s16_2_ref (uint32_t * dest, int16_t * src1, int sstr1, int16_t * src2,
    int sstr2)
{
  int i, j;
  int d;
  uint32_t sum;

  sum = 0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      d = ((int) OIL_GET (src1, sstr1 * i + j * sizeof (int16_t), int16_t)) -
          ((int) OIL_GET (src2, sstr2 * i + j * sizeof (int16_t), int16_t));
      sum += (d < 0) ? -d : d;
    }
  }
  *dest = sum;
}
OIL_DEFINE_IMPL_REF (sad8x8_s16_2_ref, sad8x8_s16_2);

static void
sad8x8_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  int i, j;
  int d;
  uint32_t sum;

  sum = 0;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      d = ((int) OIL_GET (src1, sstr1 * i + j * sizeof (uint8_t), uint8_t)) -
          ((int) OIL_GET (src2, sstr2 * i + j * sizeof (uint8_t), uint8_t));
      sum += (d < 0) ? -d : d;
    }
  }
  *dest = sum;
}
OIL_DEFINE_IMPL_REF (sad8x8_u8_ref, sad8x8_u8);

static void
sad8x8_8xn_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2, int n)
{
  int i, j, k;
  int d;
  uint32_t sum;

  for (i=0;i<n;i++){
    sum = 0;
    for (j = 0; j < 8; j++) {
      for (k = 0; k < 8; k++) {
        d = ((int) OIL_GET (src1, sstr1 * j + k * sizeof (uint8_t), uint8_t)) -
            ((int) OIL_GET (src2, sstr2 * (i+j) + k * sizeof (uint8_t), uint8_t));
        sum += (d < 0) ? -d : d;
      }
    }
    dest[i] = sum;
  }
}
OIL_DEFINE_IMPL_REF (sad8x8_8xn_u8_ref, sad8x8_8xn_u8);


static void
sad12x12_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  int i, j;
  int d;
  uint32_t sum;

  sum = 0;
  for (i = 0; i < 12; i++) {
    for (j = 0; j < 12; j++) {
      d = ((int) OIL_GET (src1, sstr1 * i + j * sizeof (uint8_t), uint8_t)) -
          ((int) OIL_GET (src2, sstr2 * i + j * sizeof (uint8_t), uint8_t));
      sum += (d < 0) ? -d : d;
    }
  }
  *dest = sum;
}
OIL_DEFINE_IMPL_REF (sad12x12_u8_ref, sad12x12_u8);

static void
sad12x12_12xn_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2, int n)
{
  int i, j, k;
  int d;
  uint32_t sum;

  for (i=0;i<n;i++){
    sum = 0;
    for (j = 0; j < 12; j++) {
      for (k = 0; k < 12; k++) {
        d = ((int) OIL_GET (src1, sstr1 * j + k * sizeof (uint8_t), uint8_t)) -
            ((int) OIL_GET (src2, sstr2 * (i+j) + k * sizeof (uint8_t), uint8_t));
        sum += (d < 0) ? -d : d;
      }
    }
    dest[i] = sum;
  }
}
OIL_DEFINE_IMPL_REF (sad12x12_12xn_u8_ref, sad12x12_12xn_u8);


static void
sad16x16_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  int i, j;
  int d;
  uint32_t sum;

  sum = 0;
  for (i = 0; i < 16; i++) {
    for (j = 0; j < 16; j++) {
      d = ((int) OIL_GET (src1, sstr1 * i + j * sizeof (uint8_t), uint8_t)) -
          ((int) OIL_GET (src2, sstr2 * i + j * sizeof (uint8_t), uint8_t));
      sum += (d < 0) ? -d : d;
    }
  }
  *dest = sum;
}
OIL_DEFINE_IMPL_REF (sad16x16_u8_ref, sad16x16_u8);

static void
sad16x16_16xn_u8_ref (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2, int n)
{
  int i, j, k;
  int d;
  uint32_t sum;

  for (i=0;i<n;i++){
    sum = 0;
    for (j = 0; j < 16; j++) {
      for (k = 0; k < 16; k++) {
        d = ((int) OIL_GET (src1, sstr1 * j + k * sizeof (uint8_t), uint8_t)) -
            ((int) OIL_GET (src2, sstr2 * (i+j) + k * sizeof (uint8_t), uint8_t));
        sum += (d < 0) ? -d : d;
      }
    }
    dest[i] = sum;
  }
}
OIL_DEFINE_IMPL_REF (sad16x16_16xn_u8_ref, sad16x16_16xn_u8);

