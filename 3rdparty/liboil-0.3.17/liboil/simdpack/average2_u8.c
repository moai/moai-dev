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

static void
average2_u8_trick (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  unsigned int x, y, d;

  while (n&3) {
    *dest = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    dest += dstr;
    n--;
  }
  while (n > 0) {
    x = (src1[0] << 24) | (src1[sstr1] << 16) | (src1[2 *
            sstr1] << 8) | (src1[3 * sstr1]);
    y = (src2[0] << 24) | (src2[sstr2] << 16) | (src2[2 *
            sstr2] << 8) | (src2[3 * sstr2]);
    d = (((x ^ y) & 0xfefefefe) >> 1) + (x & y);
    dest[0] = (d >> 24);
    dest[1*dstr] = (d >> 16);
    dest[2*dstr] = (d >> 8);
    dest[3*dstr] = (d >> 0);
    src1 += 4 * sstr1;
    src2 += 4 * sstr2;
    dest += 4 * dstr;
    n -= 4;
  }
}

OIL_DEFINE_IMPL (average2_u8_trick, average2_u8);

static void
average2_u8_fast (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  while (n > 0) {
    *dest = (*src1 + *src2) >> 1;
    src1 += sstr1;
    src2 += sstr2;
    dest += dstr;
    n--;
  }
}

OIL_DEFINE_IMPL (average2_u8_fast, average2_u8);

static void
average2_u8_unroll4 (uint8_t * dest, int dstr, uint8_t *src1, int sstr1,
    uint8_t *src2, int sstr2, int n)
{
  while (n & 0x3) {
    *dest = (*src1 + *src2) >> 1;
    dest += dstr;
    src1 += sstr1;
    src2 += sstr2;
    n--;
  }
  while (n > 0) {
    *dest = (*src1 + *src2) >> 1;
    dest += dstr;
    src1 += sstr1;
    src2 += sstr2;
    *dest = (*src1 + *src2) >> 1;
    dest += dstr;
    src1 += sstr1;
    src2 += sstr2;
    *dest = (*src1 + *src2) >> 1;
    dest += dstr;
    src1 += sstr1;
    src2 += sstr2;
    *dest = (*src1 + *src2) >> 1;
    dest += dstr;
    src1 += sstr1;
    src2 += sstr2;
    n -= 4;
  }
}

OIL_DEFINE_IMPL (average2_u8_unroll4, average2_u8);

