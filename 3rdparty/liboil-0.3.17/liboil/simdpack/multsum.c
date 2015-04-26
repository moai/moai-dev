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


static void multsum_f32_unroll2 (float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
  int i;
  double sum1 = 0;
  double sum2 = 0;

  for(i=0;i<n-1;i+=2){
    sum1 += OIL_GET(src1,0, float) * OIL_GET(src2,0, float);
    sum2 += OIL_GET(src1,sstr1, float) * OIL_GET(src2,sstr2, float);
    OIL_INCREMENT (src1, sstr1*2);
    OIL_INCREMENT (src2, sstr2*2);
  }
  if (i<n) {
    sum1 += OIL_GET(src1,0, float) * OIL_GET(src2,0, float);
  }

  *dest = sum1 + sum2;
}
OIL_DEFINE_IMPL (multsum_f32_unroll2, multsum_f32);


static void multsum_f64_unroll8 (double *dest, double *src1, int sstr1,
    double *src2, int sstr2, int n)
{
  int i = 0;
  double sum = 0;

  while(i<n-7) {
    sum += (OIL_GET(src1,0, double) * OIL_GET(src2,0, double)) +
           (OIL_GET(src1,sstr1, double) * OIL_GET(src2,sstr2, double)) +
           (OIL_GET(src1,2*sstr1, double) * OIL_GET(src2,2*sstr2, double)) +
           (OIL_GET(src1,3*sstr1, double) * OIL_GET(src2,3*sstr2, double)) +
           (OIL_GET(src1,4*sstr1, double) * OIL_GET(src2,4*sstr2, double)) +
           (OIL_GET(src1,5*sstr1, double) * OIL_GET(src2,5*sstr2, double)) +
           (OIL_GET(src1,6*sstr1, double) * OIL_GET(src2,6*sstr2, double)) +
           (OIL_GET(src1,7*sstr1, double) * OIL_GET(src2,7*sstr2, double));
    OIL_INCREMENT (src1, sstr1*8);
    OIL_INCREMENT (src2, sstr2*8);
    i+=8;
  }
  while(i<n-3) {
    sum += (OIL_GET(src1,0, double) * OIL_GET(src2,0, double)) +
           (OIL_GET(src1,sstr1, double) * OIL_GET(src2,sstr2, double)) +
           (OIL_GET(src1,2*sstr1, double) * OIL_GET(src2,2*sstr2, double)) +
           (OIL_GET(src1,3*sstr1, double) * OIL_GET(src2,3*sstr2, double));
    OIL_INCREMENT (src1, sstr1*4);
    OIL_INCREMENT (src2, sstr2*4);
    i+=4;
  }
  while(i<n-1) {
    sum += (OIL_GET(src1,0, double) * OIL_GET(src2,0, double)) +
           (OIL_GET(src1,sstr1, double) * OIL_GET(src2,sstr2, double));
    OIL_INCREMENT (src1, sstr1*2);
    OIL_INCREMENT (src2, sstr2*2);
    i+=2;
  }
  if (i<n) {
    sum += OIL_GET(src1,0, double) * OIL_GET(src2,0, double);
  }

  *dest = sum;
}
OIL_DEFINE_IMPL (multsum_f64_unroll8, multsum_f64);

