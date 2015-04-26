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
#include "jpeg.h"

#include <math.h>

/**
 * oil_conv8x8_s16_f64:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Converts elements in source array @s_8x8 to the destination
 * type, placing the results in @d_8x8.
 */
OIL_DEFINE_CLASS (conv8x8_s16_f64,
    "int16_t * d_8x8, int dstr, double *s_8x8, int sstr");
/**
 * oil_conv8x8_f64_s16:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Converts elements in source array @s_8x8 to the destination
 * type, placing the results in @d_8x8.  The conversion of source
 * values outside the destination range are undefined and
 * implementation dependent.
 */
OIL_DEFINE_CLASS (conv8x8_f64_s16,
    "double *d_8x8, int dstr, int16_t * s_8x8, int sstr");
/**
 * oil_clipconv8x8_u8_s16:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Converts elements in source array @s_8x8 to the destination
 * type, placing the results in @d_8x8.  Source values outside
 * the destination range are clipped to the destination range.
 */
OIL_DEFINE_CLASS (clipconv8x8_u8_s16,
    "uint8_t * d_8x8, int dstr, int16_t * s_8x8, int sstr");

#define BLOCK8x8_F64(ptr, stride, row, column) \
	(*((double *)((unsigned char *)ptr + stride*row) + column))

#define BLOCK8x8_PTR_F64(ptr, stride, row, column) \
	((double *)((unsigned char *)ptr + stride*row) + column)

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((unsigned char *)ptr + stride*row) + column))

#define BLOCK8x8_U8(ptr, stride, row, column) \
	(*((uint8_t *)((unsigned char *)ptr + stride*row) + column))


static void
conv8x8_s16_f64_c (int16_t * dest, int dstr, double *src, int sstr)
{
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      BLOCK8x8_S16 (dest, dstr, i, j) = floor (0.5 + BLOCK8x8_F64 (src, sstr, i, j));
    }
  }
}

OIL_DEFINE_IMPL_REF (conv8x8_s16_f64_c, conv8x8_s16_f64);

static void
conv8x8_f64_s16_c (double *dest, int dstr, int16_t * src, int sstr)
{
  int i, j;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      BLOCK8x8_F64 (dest, dstr, i, j) = BLOCK8x8_S16 (src, sstr, i, j);
    }
  }
}

OIL_DEFINE_IMPL_REF (conv8x8_f64_s16_c, conv8x8_f64_s16);

static void
clipconv8x8_u8_s16_c (uint8_t * dest, int dstr, int16_t * src, int sstr)
{
  int i, j;
  int16_t x;

  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++) {
      x = BLOCK8x8_S16 (src, sstr, i, j);
      if (x < 0)
	x = 0;
      if (x > 255)
	x = 255;
      BLOCK8x8_U8 (dest, dstr, i, j) = x;
    }
  }
}

OIL_DEFINE_IMPL_REF (clipconv8x8_u8_s16_c, clipconv8x8_u8_s16);
