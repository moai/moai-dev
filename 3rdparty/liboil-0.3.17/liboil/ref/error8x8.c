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
 * oil_err_intra8x8_u8:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 *
 * Calculates the sum of squared differences from the mean over
 * @s1_8x8 and places the result in @d_1.  This result is 64 times
 * the variance of the mean of @s1_8x8.
 */
OIL_DEFINE_CLASS (err_intra8x8_u8,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1");
/**
 * oil_err_inter8x8_u8:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @ss2:
 *
 * Calculates an intermediate 8x8 block where each element is the
 * difference between @s1_8x8 and @s2_8x8.
 * The sum of squares of the difference of each element in the 
 * intermediate block and the mean of the intermediate block is
 * placed into @d_1.  This result is
 * equal to 64 times the variance of the mean of the intermediate block.
 */
OIL_DEFINE_CLASS (err_inter8x8_u8,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, int ss2");
/**
 * oil_err_inter8x8_u8_avg:
 * @d_1:
 * @s1_8x8:
 * @ss1:
 * @s2_8x8:
 * @s3_8x8:
 * @ss2:
 *
 * Calculates an intermediate 8x8 block where each element is the
 * difference between @s1_8x8 and the average of @s2_8x8 and @s3_8x8.
 * The sum of squares of the difference of each element in the 
 * intermediate block and the mean of the intermediate block is
 * placed into @d_1.
 * This result is 64 times the variance of the mean of the intermediate
 * block.
 *
 * FIXME: This function is broken, since the reference function uses
 * @ss2 as the stride for both @s2_8x8 and @s3_8x8.
 */
OIL_DEFINE_CLASS (err_inter8x8_u8_avg,
    "uint32_t *d_1, uint8_t *s1_8x8, int ss1, uint8_t *s2_8x8, uint8_t *s3_8x8, int ss2");

#define DSP_OP_AVG(a,b) ((((int)(a)) + ((int)(b)))/2)
#define DSP_OP_DIFF(a,b) (((int)(a)) - ((int)(b)))

static void
err_intra8x8_u8_ref (uint32_t *dest, uint8_t *src1, int ss1)
{
  uint32_t  i;
  uint32_t  xsum=0;
  uint32_t  xxsum=0;

  for (i=8; i; i--) {
    /* Examine alternate pixel locations. */
    xsum += src1[0];
    xxsum += src1[0]*src1[0];
    xsum += src1[1];
    xxsum += src1[1]*src1[1];
    xsum += src1[2];
    xxsum += src1[2]*src1[2];
    xsum += src1[3];
    xxsum += src1[3]*src1[3];
    xsum += src1[4];
    xxsum += src1[4]*src1[4];
    xsum += src1[5];
    xxsum += src1[5]*src1[5];
    xsum += src1[6];
    xxsum += src1[6]*src1[6];
    xsum += src1[7];
    xxsum += src1[7]*src1[7];

    /* Step to next row of block. */
    src1 += ss1;
  }
  /* Compute population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum ));
}
OIL_DEFINE_IMPL_REF (err_intra8x8_u8_ref, err_intra8x8_u8);

static void
err_inter8x8_u8_ref (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  uint32_t  i;
  uint32_t  xsum=0;
  uint32_t  xxsum=0;
  int32_t   diff;

  for (i=8; i; i--) {
    diff = DSP_OP_DIFF (src1[0], src2[0]);
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF (src1[1], src2[1]);
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF (src1[2], src2[2]);
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF (src1[3], src2[3]);
    xsum += diff;
    xxsum += diff*diff;
        
    diff = DSP_OP_DIFF (src1[4], src2[4]);
    xsum += diff;
    xxsum += diff*diff;
        
    diff = DSP_OP_DIFF (src1[5], src2[5]);
    xsum += diff;
    xxsum += diff*diff;
        
    diff = DSP_OP_DIFF (src1[6], src2[6]);
    xsum += diff;
    xxsum += diff*diff;
        
    diff = DSP_OP_DIFF (src1[7], src2[7]);
    xsum += diff;
    xxsum += diff*diff;
        
    /* Step to next row of block. */
    src1 += ss1;
    src2 += ss2;
  }

  /* Compute and return population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum));
}
OIL_DEFINE_IMPL_REF (err_inter8x8_u8_ref, err_inter8x8_u8);

static void
err_inter8x8_u8_avg_ref (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, uint8_t *src3, int ss2)
{
  uint32_t  i;
  uint32_t  xsum=0;
  uint32_t  xxsum=0;
  int32_t   diff;

  for (i=8; i; i--) {
    diff = DSP_OP_DIFF(src1[0], DSP_OP_AVG (src2[0], src3[0]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[1], DSP_OP_AVG (src2[1], src3[1]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[2], DSP_OP_AVG (src2[2], src3[2]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[3], DSP_OP_AVG (src2[3], src3[3]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[4], DSP_OP_AVG (src2[4], src3[4]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[5], DSP_OP_AVG (src2[5], src3[5]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[6], DSP_OP_AVG (src2[6], src3[6]));
    xsum += diff;
    xxsum += diff*diff;

    diff = DSP_OP_DIFF(src1[7], DSP_OP_AVG (src2[7], src3[7]));
    xsum += diff;
    xxsum += diff*diff;

    /* Step to next row of block. */
    src1 += ss1;
    src2 += ss2;
    src3 += ss2;
  }

  /* Compute and return population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum));
}

OIL_DEFINE_IMPL_REF (err_inter8x8_u8_avg_ref, err_inter8x8_u8_avg);
