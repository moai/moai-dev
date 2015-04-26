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
#include <liboil/liboiltest.h>
#include <liboil/liboilrandom.h>
#include <liboil/dct/dct.h>
#include <math.h>

static void
idct8theora_s16_test (OilTest *test)
{
  int i;
  int stride = test->params[OIL_ARG_SSTR1].value;
  uint16_t *ptr = (uint16_t *)oil_test_get_source_data(test,OIL_ARG_SRC1);

  for(i=0;i<8;i++){
    OIL_GET(ptr, i*stride, int16_t) = oil_rand_s16() >> 3;
    //OIL_GET(ptr, i*stride, int16_t) = 0;
  }
  //OIL_GET(ptr, 0*stride, int16_t) = 100;

}

static void
idct8x8theora_s16_test (OilTest *test)
{
  int i;
  int j;
  int stride = test->params[OIL_ARG_SSTR1].value;
  uint16_t *ptr = (uint16_t *)oil_test_get_source_data(test,OIL_ARG_SRC1);

  for(i=0;i<8;i++){
    for(j=0;j<8;j++){
      OIL_GET(ptr, i*stride + j*2, int16_t) = oil_rand_s16() >> 3;
    }
  }

}

/**
 * oil_idct8theora_s16:
 * @d_8:
 * @dstr:
 * @s_8:
 * @sstr:
 *
 * Performs a Inverse Discrete Cosine Transform on @s_8 and places
 * the result in @d_8, as defined by the Theora specification.
 */
OIL_DEFINE_CLASS_FULL (idct8theora_s16, "int16_t *d_8, int dstr, int16_t *s_8, int sstr", idct8theora_s16_test);
/**
 * oil_idct8x8theora_s16:
 * @d_8x8:
 * @dstr:
 * @s_8x8:
 * @sstr:
 *
 * Performs a Inverse Discrete Cosine Transform on @s_8x8 and places
 * the result in @d_8x8, as defined by the Theora specification.
 */
OIL_DEFINE_CLASS_FULL (idct8x8theora_s16, "int16_t *d_8x8, int dstr, int16_t *s_8x8, int sstr", idct8x8theora_s16_test);



#define C1 64277
#define C2 60547
#define C3 54491
#define C4 46341
#define C5 36410
#define C6 25080
#define C7 12785

#define S7 64277
#define S6 60547
#define S5 54491
#define S4 46341
#define S3 36410
#define S2 25080
#define S1 12785

#define TRUNC(x) ((int16_t)x)
#define MULT(a,b) (((a)*(b))>>16)

static void
idct8theora_s16_ref (int16_t *dest, int dstr, const int16_t *src, int sstr)
{
  int32_t t[10];
  int32_t r;

#define Y(i) OIL_GET(src,sstr*(i),int16_t)
#define X(i) OIL_GET(dest,sstr*(i),int16_t)

  /* the ordering here corresponds closely to the theora spec */
  t[0] = MULT(C4, Y(0) + Y(4));
  t[0] = TRUNC(t[0]);
  t[1] = MULT(C4, Y(0) - Y(4));
  t[1] = TRUNC(t[1]);
  t[2] = MULT(C6, Y(2)) - MULT(S6, Y(6));
  t[3] = MULT(S6, Y(2)) + MULT(C6, Y(6));
  t[4] = MULT(C7, Y(1)) - MULT(S7, Y(7));
  t[5] = MULT(C3, Y(5)) - MULT(S3, Y(3));
  t[6] = MULT(S3, Y(5)) + MULT(C3, Y(3));
  t[7] = MULT(S7, Y(1)) + MULT(C7, Y(7));
  r = t[4] + t[5];
  t[5] = MULT(C4, t[4] - t[5]);
  t[5] = TRUNC(t[5]);
  t[4] = r;
  r = t[7] + t[6];
  t[6] = MULT(C4, t[7] - t[6]);
  t[6] = TRUNC(t[6]);
  t[7] = r;
  r = t[0] + t[3];
  t[3] = t[0] - t[3];
  t[0] = r;
  r = t[1] + t[2];
  t[2] = t[1] - t[2];
  t[1] = r;
  r = t[6] + t[5];
  t[5] = t[6] - t[5];
  t[6] = r;
  r = t[0] + t[7];
  r = TRUNC(r);
  X(0) = r;
  r = t[1] + t[6];
  r = TRUNC(r);
  X(1) = r;
  r = t[2] + t[5];
  r = TRUNC(r);
  X(2) = r;
  r = t[3] + t[4];
  r = TRUNC(r);
  X(3) = r;
  r = t[3] - t[4];
  r = TRUNC(r);
  X(4) = r;
  r = t[2] - t[5];
  r = TRUNC(r);
  X(5) = r;
  r = t[1] - t[6];
  r = TRUNC(r);
  X(6) = r;
  r = t[0] - t[7];
  r = TRUNC(r);
  X(7) = r;
}
OIL_DEFINE_IMPL_REF (idct8theora_s16_ref, idct8theora_s16);


static void
idct8x8theora_s16_ref (int16_t *dest, int dstr, const int16_t *src, int sstr)
{
  int i;
  int16_t tmp[64];

  for(i=0;i<8;i++){
    idct8theora_s16_ref(
        OIL_OFFSET(tmp, 8*sizeof(int16_t) * i), sizeof(int16_t),
        OIL_OFFSET(src, sstr * i), sizeof(int16_t));
  }
  for(i=0;i<8;i++){
    idct8theora_s16_ref(
        OIL_OFFSET(dest, sizeof(int16_t) * i), dstr,
        OIL_OFFSET(tmp, sizeof(int16_t) * i), sizeof(int16_t) * 8);
  }
}
OIL_DEFINE_IMPL_REF (idct8x8theora_s16_ref, idct8x8theora_s16);


