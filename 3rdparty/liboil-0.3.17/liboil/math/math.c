/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
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

#include <math.h>

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


#if 0
static void
add_f32_unroll4 (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    dest[i+0] = src1[i+0] + src2[i+0];
    dest[i+1] = src1[i+1] + src2[i+1];
    dest[i+2] = src1[i+2] + src2[i+2];
    dest[i+3] = src1[i+3] + src2[i+3];
  }
  for(;i<n;i++){
    dest[i] = src1[i] + src2[i];
  }
}
OIL_DEFINE_IMPL (add_f32_unroll4, add_f32);
#endif

#if 0
static void
add_f32_unroll4b (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    *dest++ = *src1++ + *src2++;
    *dest++ = *src1++ + *src2++;
    *dest++ = *src1++ + *src2++;
    *dest++ = *src1++ + *src2++;
  }
  for(;i<n;i++){
    *dest++ = *src1++ + *src2++;
  }
}
OIL_DEFINE_IMPL (add_f32_unroll4b, add_f32);
#endif


#if 0
static void
multiply_f32_unroll4 (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    dest[i+0] = src1[i+0] * src2[i+0];
    dest[i+1] = src1[i+1] * src2[i+1];
    dest[i+2] = src1[i+2] * src2[i+2];
    dest[i+3] = src1[i+3] * src2[i+3];
  }
  for(;i<n;i++){
    dest[i] = src1[i] * src2[i];
  }
}
OIL_DEFINE_IMPL (multiply_f32_unroll4, multiply_f32);
#endif

static void
scalaradd_f32_ns_unroll4 (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    dest[i+0] = src1[i+0] + src2[0];
    dest[i+1] = src1[i+1] + src2[0];
    dest[i+2] = src1[i+2] + src2[0];
    dest[i+3] = src1[i+3] + src2[0];
  }
  for(;i<n;i++){
    dest[i] = src1[i] + src2[0];
  }
}
OIL_DEFINE_IMPL (scalaradd_f32_ns_unroll4, scalaradd_f32_ns);

static void
scalarmultiply_f32_ns_unroll4 (float *dest, float *src1, float *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    dest[i+0] = src1[i+0] * src2[0];
    dest[i+1] = src1[i+1] * src2[0];
    dest[i+2] = src1[i+2] * src2[0];
    dest[i+3] = src1[i+3] * src2[0];
  }
  for(;i<n;i++){
    dest[i] = src1[i] * src2[0];
  }
}
OIL_DEFINE_IMPL (scalarmultiply_f32_ns_unroll4, scalarmultiply_f32_ns);

static void
scalarmultiply_f64_ns_unroll4 (double *dest, double *src1, double *src2, int n)
{
  int i;

  for(i=0;i<(n&(~0x3));i+=4){
    dest[i+0] = src1[i+0] * src2[0];
    dest[i+1] = src1[i+1] * src2[0];
    dest[i+2] = src1[i+2] * src2[0];
    dest[i+3] = src1[i+3] * src2[0];
  }
  for(;i<n;i++){
    dest[i] = src1[i] * src2[0];
  }
}
OIL_DEFINE_IMPL (scalarmultiply_f64_ns_unroll4, scalarmultiply_f64_ns);

