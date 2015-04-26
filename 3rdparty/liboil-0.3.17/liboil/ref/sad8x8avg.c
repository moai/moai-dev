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
#include <math.h>

#define ABS(x) ((x)>0 ? (x) : -(x))

/**
 * oil_sad8x8_u8_avg:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @s3_8x8:
 * @ss2:
 *
 * Calculates the sum of absolute differences between @s1_8x8 and
 * the average of @s2_8x8 and @s3_8x8.
 *
 * FIXME: This function is broken because the reference function assumes
 * the stride for @s3_8x8 is @ss2.
 */
OIL_DEFINE_CLASS (sad8x8_u8_avg,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, uint8_t *s3_8x8, int ss2");

static void
sad8x8_u8_avg_ref (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, uint8_t *src3, int ss2)
{
  int i;
  uint32_t diff = 0;

  for (i=0; i<8;i++){
    diff += ABS(((int)src1[0]) - (((int)src2[0] + (int)src3[0]) / 2));
    diff += ABS(((int)src1[1]) - (((int)src2[1] + (int)src3[1]) / 2));
    diff += ABS(((int)src1[2]) - (((int)src2[2] + (int)src3[2]) / 2));
    diff += ABS(((int)src1[3]) - (((int)src2[3] + (int)src3[3]) / 2));
    diff += ABS(((int)src1[4]) - (((int)src2[4] + (int)src3[4]) / 2));
    diff += ABS(((int)src1[5]) - (((int)src2[5] + (int)src3[5]) / 2));
    diff += ABS(((int)src1[6]) - (((int)src2[6] + (int)src3[6]) / 2));
    diff += ABS(((int)src1[7]) - (((int)src2[7] + (int)src3[7]) / 2));

    /* Step to next row of block. */
    src1 += ss1;
    src2 += ss2;
    src3 += ss2;
  }
  *dest = diff;
}

OIL_DEFINE_IMPL_REF (sad8x8_u8_avg_ref, sad8x8_u8_avg);


