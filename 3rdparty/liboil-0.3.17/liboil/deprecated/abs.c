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
#include <liboil/simdpack/simdpack.h>
#include <math.h>

OIL_DEFINE_CLASS (abs_u8_s8, "uint8_t *dest, int dstr, int8_t *src, int sstr, int n");
OIL_DEFINE_CLASS (abs_u16_s16, "uint16_t *dest, int dstr, int16_t *src, int sstr, int n");
OIL_DEFINE_CLASS (abs_u32_s32, "uint32_t *dest, int dstr, int32_t *src, int sstr, int n");
OIL_DEFINE_CLASS (abs_f32_f32, "float *dest, int dstr, float *src, int sstr, int n");
OIL_DEFINE_CLASS (abs_f64_f64, "double *dest, int dstr, double *src, int sstr, int n");

//static void abs_test (OilFunctionClass *klass, OilFunctionImpl *impl);

#define ABS(x) ((x)>0 ? (x) : -(x))


/**
 * oil_abs_u8_s8:
 * @dest: destination array
 * @dstr: stride of destination elements
 * @src: source array
 * @sstr: stride of source elements
 * @n: number of elements in arrays
 *
 * Calculates the absolute value of each element in the source array
 * and writes it into the destination array.
 */
static void
abs_u8_s8_ref (uint8_t *dest, int dstr, int8_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int8_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint8_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u8_s8_ref, abs_u8_s8);

/**
 * oil_abs_u16_s16:
 * @dest: destination array
 * @dstr: stride of destination elements
 * @src: source array
 * @sstr: stride of source elements
 * @n: number of elements in arrays
 *
 * Calculates the absolute value of each element in the source array
 * and writes it into the destination array.
 */
static void
abs_u16_s16_ref (uint16_t *dest, int dstr, int16_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int16_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint16_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u16_s16_ref, abs_u16_s16);

/**
 * oil_abs_u32_s32:
 * @dest: destination array
 * @dstr: stride of destination elements
 * @src: source array
 * @sstr: stride of source elements
 * @n: number of elements in arrays
 *
 * Calculates the absolute value of each element in the source array
 * and writes it into the destination array.
 */
static void
abs_u32_s32_ref (uint32_t *dest, int dstr, int32_t *src, int sstr, int n)
{
  int i;
  int x;

  for (i=0; i<n; i++) {
    x = OIL_GET(src, i*sstr, int32_t);
    x = ABS(x);
    OIL_GET(dest, i*dstr, uint32_t) = x;
  }
}
OIL_DEFINE_IMPL_REF (abs_u32_s32_ref, abs_u32_s32);

/**
 * oil_abs_f32_f32:
 * @dest: destination array
 * @dstr: stride of destination elements
 * @src: source array
 * @sstr: stride of source elements
 * @n: number of elements in arrays
 *
 * Calculates the absolute value of each element in the source array
 * and writes it into the destination array.
 */
static void
abs_f32_f32_ref (float *dest, int dstr, float *src, int sstr, int n)
{
  int i;

  for (i=0; i<n; i++) {
    OIL_GET(dest, i*dstr, float) = fabs(OIL_GET(src, i*sstr, float));
  }
}

OIL_DEFINE_IMPL_REF (abs_f32_f32_ref, abs_f32_f32);

/**
 * oil_abs_f64_f64:
 * @dest: destination array
 * @dstr: stride of destination elements
 * @src: source array
 * @sstr: stride of source elements
 * @n: number of elements in arrays
 *
 * Calculates the absolute value of each element in the source array
 * and writes it into the destination array.
 */
static void
abs_f64_f64_ref (double *dest, int dstr, double *src, int sstr, int n)
{
  int i;

  for (i=0; i<n; i++) {
    OIL_GET(dest, i*dstr, double) = fabs(OIL_GET(src, i*sstr, double));
  }
}

OIL_DEFINE_IMPL_REF (abs_f64_f64_ref, abs_f64_f64);
