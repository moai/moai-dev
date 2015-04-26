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

/**
 * oil_diff8x8_s16_u8:
 * @d_8x8:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * Calculates the difference of each value in @s1_8x8 and @s2_8x8
 * and places the result in @d_8x8.  Note that the destination type
 * is larger than the source type.
 */
OIL_DEFINE_CLASS (diff8x8_s16_u8,
    "int16_t *d_8x8, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2");
/**
 * oil_diff8x8_const128_s16_u8:
 * @d_8x8:
 * @s1_8x8:
 * @ss1:
 *
 * Subtracts 128 from each value in @s1_8x8
 * and places the result in @d_8x8.  Note that the destination type
 * is larger than the source type.
 */
OIL_DEFINE_CLASS (diff8x8_const128_s16_u8,
    "int16_t *d_8x8, uint8_t *s1_8x8, int ss1");
/**
 * oil_diff8x8_average_s16_u8:
 * @d_8x8:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 * @s3_8x8:
 * @ss3:
 *
 * Calculates the difference of each value in @s1_8x8 and the
 * average of @s2_8x8 and @s3_8x8,
 * and places the result in @d_8x8.  Note that the destination type
 * is larger than the source type.
 */
OIL_DEFINE_CLASS (diff8x8_average_s16_u8,
    "int16_t *d_8x8, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2, uint8_t *s3_8x8, int ss3");

static void
diff8x8_s16_u8_ref (int16_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  int i;

  /* For each block row */
  for (i=0;i<8;i++ ){
    dest[0] = ((int16_t)src1[0]) - ((int16_t)src2[0]);
    dest[1] = ((int16_t)src1[1]) - ((int16_t)src2[1]);
    dest[2] = ((int16_t)src1[2]) - ((int16_t)src2[2]);
    dest[3] = ((int16_t)src1[3]) - ((int16_t)src2[3]);
    dest[4] = ((int16_t)src1[4]) - ((int16_t)src2[4]);
    dest[5] = ((int16_t)src1[5]) - ((int16_t)src2[5]);
    dest[6] = ((int16_t)src1[6]) - ((int16_t)src2[6]);
    dest[7] = ((int16_t)src1[7]) - ((int16_t)src2[7]);

    /* Start next row */
    src1 += ss1;
    src2 += ss2;
    dest += 8;
  }
}
OIL_DEFINE_IMPL_REF (diff8x8_s16_u8_ref, diff8x8_s16_u8);

static void
diff8x8_const128_s16_u8_ref (int16_t *dest, uint8_t *src1, int ss1)
{
  int i;

  /* For each block row */
  for (i=0;i<8;i++ ){
    dest[0] = ((int16_t)src1[0]) - 128;
    dest[1] = ((int16_t)src1[1]) - 128;
    dest[2] = ((int16_t)src1[2]) - 128;
    dest[3] = ((int16_t)src1[3]) - 128;
    dest[4] = ((int16_t)src1[4]) - 128;
    dest[5] = ((int16_t)src1[5]) - 128;
    dest[6] = ((int16_t)src1[6]) - 128;
    dest[7] = ((int16_t)src1[7]) - 128;

    /* Start next row */
    src1 += ss1;
    dest += 8;
  }
}
OIL_DEFINE_IMPL_REF (diff8x8_const128_s16_u8_ref, diff8x8_const128_s16_u8);

static void
diff8x8_average_s16_u8_ref (int16_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2, uint8_t *src3, int ss3)
{
  int i;

  /* For each block row */
  for (i=0;i<8;i++ ){
    dest[0] = ((int16_t)src1[0]) - ((((int16_t)src2[0]) + ((int16_t)src3[0])) / 2);
    dest[1] = ((int16_t)src1[1]) - ((((int16_t)src2[1]) + ((int16_t)src3[1])) / 2);
    dest[2] = ((int16_t)src1[2]) - ((((int16_t)src2[2]) + ((int16_t)src3[2])) / 2);
    dest[3] = ((int16_t)src1[3]) - ((((int16_t)src2[3]) + ((int16_t)src3[3])) / 2);
    dest[4] = ((int16_t)src1[4]) - ((((int16_t)src2[4]) + ((int16_t)src3[4])) / 2);
    dest[5] = ((int16_t)src1[5]) - ((((int16_t)src2[5]) + ((int16_t)src3[5])) / 2);
    dest[6] = ((int16_t)src1[6]) - ((((int16_t)src2[6]) + ((int16_t)src3[6])) / 2);
    dest[7] = ((int16_t)src1[7]) - ((((int16_t)src2[7]) + ((int16_t)src3[7])) / 2);

    /* Start next row */
    src1 += ss1;
    src2 += ss2;
    src3 += ss3;
    dest += 8;
  }
}
OIL_DEFINE_IMPL_REF (diff8x8_average_s16_u8_ref, diff8x8_average_s16_u8);






