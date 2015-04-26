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

#define ABS(x) ((x)>0 ? (x) : -(x))

static void
abs_u16_s16_unroll2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 1) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  while (n > 0) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n -= 2;
  }
}
OIL_DEFINE_IMPL (abs_u16_s16_unroll2, abs_u16_s16);

static void
abs_u16_s16_unroll4 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 3) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  while (n > 0) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n -= 4;
  }
}
OIL_DEFINE_IMPL (abs_u16_s16_unroll4, abs_u16_s16);

static void
abs_u16_s16_fast (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  int i;
  int16_t x, y;

  for (i = 0; i < n; i++) {
    x = *src;
    y = ((x >> 15) & x);
    *dest = x - y - y;
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
  }
}

OIL_DEFINE_IMPL (abs_u16_s16_fast, abs_u16_s16);


/* Similar algorithm as clipping, but for absolute value.  Note the stunt
 * pulled to get around 2*y rolling over.  */

static void
abs_u32_s32_fast (uint32_t * dest, int dstr, int32_t * src, int sstr, int n)
{
  int i;
  int32_t x, y;

  for (i = 0; i < n; i++) {
    x = OIL_GET(src, i * sstr, int32_t);
    y = ((x >> 31) & x);
    OIL_GET(dest, i * dstr, uint32_t) = x - y - y;
  }
}

OIL_DEFINE_IMPL (abs_u32_s32_fast, abs_u32_s32);

