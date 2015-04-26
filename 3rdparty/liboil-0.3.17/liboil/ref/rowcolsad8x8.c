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

#define ABS(x) ((x)>0 ? (x) : -(x))
#define DSP_OP_ABS_DIFF(a,b) ABS((((int)(a)) - ((int)(b))))

/**
 * oil_rowsad8x8_u8:
 * @d_1:
 * @s1_8x8:
 * @s2_8x8:
 *
 * Calculates the sum of absolute differences between @s1_8x8 and @s1_8s8
 * for the first 4 elements of the first row, and the sum of absolute
 * differences for the last 4 elements of the first row, and places the
 * maximum of those values in @dest.
 *
 * FIXME: This function is declared incorrectly.
 */
OIL_DEFINE_CLASS (rowsad8x8_u8,
    "uint32_t *d_1, uint8_t *s1_8x8, uint8_t *s2_8x8");
/**
 * oil_colsad8x8_u8:
 * @d_1:
 * @s1_8x8:
 * @s2_8x8:
 *
 * Divides the 8x8 block into 16 1x4 regions, and calculates the
 * sum of absolute differences between @s1_8x8 and @s2_8x8 for
 * each region.  The maximum of the results in each region is
 * placed in @d_1.
 */
OIL_DEFINE_CLASS (colsad8x8_u8,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2");

static void
rowsad8x8_u8_ref (uint32_t *dest, uint8_t *src1, uint8_t *src2)
{
  uint32_t SadValue;
  uint32_t SadValue1;

  SadValue    = DSP_OP_ABS_DIFF (src1[0], src2[0]) + 
	        DSP_OP_ABS_DIFF (src1[1], src2[1]) +
	        DSP_OP_ABS_DIFF (src1[2], src2[2]) +
	        DSP_OP_ABS_DIFF (src1[3], src2[3]);

  SadValue1   = DSP_OP_ABS_DIFF (src1[4], src2[4]) + 
	        DSP_OP_ABS_DIFF (src1[5], src2[5]) +
	        DSP_OP_ABS_DIFF (src1[6], src2[6]) +
	        DSP_OP_ABS_DIFF (src1[7], src2[7]);

  *dest = (SadValue > SadValue1) ? SadValue : SadValue1;
}
OIL_DEFINE_IMPL_REF (rowsad8x8_u8_ref, rowsad8x8_u8);

static void
colsad8x8_u8_ref (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  uint32_t SadValue[8] = {0,0,0,0,0,0,0,0};
  uint32_t SadValue2[8] = {0,0,0,0,0,0,0,0};
  uint32_t MaxSad = 0;
  uint32_t i;

  for ( i = 0; i < 4; i++ ){
    SadValue[0] += ABS(src1[0] - src2[0]);
    SadValue[1] += ABS(src1[1] - src2[1]);
    SadValue[2] += ABS(src1[2] - src2[2]);
    SadValue[3] += ABS(src1[3] - src2[3]);
    SadValue[4] += ABS(src1[4] - src2[4]);
    SadValue[5] += ABS(src1[5] - src2[5]);
    SadValue[6] += ABS(src1[6] - src2[6]);
    SadValue[7] += ABS(src1[7] - src2[7]);
    
    src1 += ss1;
    src2 += ss2;
  }

  for ( i = 0; i < 4; i++ ){
    SadValue2[0] += ABS(src1[0] - src2[0]);
    SadValue2[1] += ABS(src1[1] - src2[1]);
    SadValue2[2] += ABS(src1[2] - src2[2]);
    SadValue2[3] += ABS(src1[3] - src2[3]);
    SadValue2[4] += ABS(src1[4] - src2[4]);
    SadValue2[5] += ABS(src1[5] - src2[5]);
    SadValue2[6] += ABS(src1[6] - src2[6]);
    SadValue2[7] += ABS(src1[7] - src2[7]);
    
    src1 += ss1;
    src2 += ss2;
  }
    
  for ( i = 0; i < 8; i++ ){
    if ( SadValue[i] > MaxSad )
      MaxSad = SadValue[i];
    if ( SadValue2[i] > MaxSad )
      MaxSad = SadValue2[i];
  }
    
  *dest = MaxSad;
}
OIL_DEFINE_IMPL_REF (colsad8x8_u8_ref, colsad8x8_u8);

