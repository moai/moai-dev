/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
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


#include <liboil/liboil.h>
#include <liboil/liboilprofile.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000

static void taylor4_f32_ref (float *dest, float *src, float *tmp, float *a, int n);
static void taylor4_f32_oil (float *dest, float *src, float *tmp, float *a, int n);

int main(int argc, char *argv[])
{
  float *dest;
  float *src;
  float *tmp;
  float a[4];
  int i;
  OilProfile prof;
  double ave, std;

  oil_init();

  src = malloc(N*sizeof(float));
  dest = malloc(N*sizeof(float));
  tmp = malloc(3*N*sizeof(float));

  for(i=0;i<N;i++){
    src[i] = i;
  }
  a[0] = 1;
  a[1] = 1;
  a[2] = 1;
  a[3] = 1;

  oil_profile_init(&prof);
  for(i=0;i<10;i++){
    oil_profile_start(&prof);
    taylor4_f32_ref (dest, src, tmp, a, N);
    oil_profile_stop(&prof);
  }
  oil_profile_get_ave_std (&prof, &ave, &std);
  printf("ref: %10.4g %10.4g\n", ave, std);
  for(i=0;i<10;i++){
    printf("%g\n", dest[i]);
  }

  oil_profile_init(&prof);
  for(i=0;i<10;i++){
    oil_profile_start(&prof);
    taylor4_f32_oil (dest, src, tmp, a, N);
    oil_profile_stop(&prof);
  }
  oil_profile_get_ave_std (&prof, &ave, &std);
  printf("oil: %10.4g %10.4g\n", ave, std);
  for(i=0;i<10;i++){
    printf("%g\n", dest[i]);
  }

  return 0;
}

static void
taylor4_f32_ref (float *dest, float *src, float *tmp, float *a, int n)
{
  int i;
  float x;
  for(i=0;i<n;i++){
    x = src[i];
    dest[i] = a[0];
    dest[i] += a[1] * x;
    dest[i] += a[2] * x * x;
    dest[i] += a[3] * x * x * x;
  }

}


static void
taylor4_f32_oil (float *dest, float *src, float *tmp, float *a, int n)
{
  float *tmp1;
  float *tmp2;
  float *tmp3;

  tmp1 = tmp;
  tmp2 = tmp+N;
  tmp3 = tmp+2*N;

  oil_scalarmultiply_f32_ns (tmp1, src, a+1, n);

  oil_scalaradd_f32_ns (tmp1, tmp1, a, n);

  oil_multiply_f32 (tmp2, src, src, n);
  oil_scalarmultiply_f32_ns (tmp3, tmp2, a+2, n);
  oil_add_f32 (tmp1, tmp1, tmp3, n);

  oil_multiply_f32 (tmp2, tmp2, src, n);
  oil_scalarmultiply_f32_ns (tmp3, tmp2, a+3, n);
  oil_add_f32 (dest, tmp1, tmp3, n);
}

