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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboiltest.h>


#define AYUV(a,y,u,v) (((a)<<24) | ((y)<<16) | ((u)<<8) | (v))
#define AYUV_A(color) (((color)>>24)&0xff)
#define AYUV_Y(color) (((color)>>16)&0xff)
#define AYUV_U(color) (((color)>>8)&0xff)
#define AYUV_V(color) (((color)>>0)&0xff)


/**
 * oil_yuyv2ayuv:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in YUYV format to AYUV.  Note that only approximately
 * half of the source array is used.  Alpha values are set to 255.
 */
OIL_DEFINE_CLASS (yuyv2ayuv, "uint32_t *d_n, uint32_t *s_n, int n");
/**
 * oil_yvyu2ayuv:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in YVYU format to AYUV.  Note that only approximately
 * half of the source array is used.  Alpha values are set to 255.
 */
OIL_DEFINE_CLASS (yvyu2ayuv, "uint32_t *d_n, uint32_t *s_n, int n");
/**
 * oil_uyvy2ayuv:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in UYVY format to AYUV.  Note that only approximately
 * half of the source array is used.  Alpha values are set to 255.
 */
OIL_DEFINE_CLASS (uyvy2ayuv, "uint32_t *d_n, uint32_t *s_n, int n");

/**
 * oil_ayuv2yuyv:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in AYUV format to YUYV.  Note that only approximately
 * half of the destination array is written.  Alpha values are ignored.
 */
OIL_DEFINE_CLASS (ayuv2yuyv, "uint32_t *d_n, uint32_t *s_n, int n");
/**
 * oil_ayuv2yvyu:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in AYUV format to YVYU.  Note that only approximately
 * half of the destination array is written.  Alpha values are ignored.
 */
OIL_DEFINE_CLASS (ayuv2yvyu, "uint32_t *d_n, uint32_t *s_n, int n");
/**
 * oil_ayuv2uyvy:
 * @d_n:
 * @s_n:
 * n:
 *
 * Converts pixels in AYUV format to UYVY.  Note that only approximately
 * half of the destination array is written.  Alpha values are ignored.
 */
OIL_DEFINE_CLASS (ayuv2uyvy, "uint32_t *d_n, uint32_t *s_n, int n");
/**
 * oil_packyuyv:
 * @d:
 * @s1_nx2:
 * @s2:
 * @s3:
 * n:
 *
 * Packs pixels in separate Y, U, and V arrays to YUYV.
 */
OIL_DEFINE_CLASS (packyuyv,
    "uint32_t *d, uint8_t *s1_nx2, uint8_t *s2, uint8_t *s3, int n");
/**
 * oil_unpackyuyv:
 * @d1_nx2:
 * @d2:
 * @d3:
 * @s:
 * n:
 *
 * Unpacks YUYV pixels into separate Y, U, and V arrays;
 */
OIL_DEFINE_CLASS (unpackyuyv,
    "uint8_t *d1_nx2, uint8_t *d2, uint8_t *d3, uint32_t *s int n");


static void
yuyv2ayuv_ref (uint32_t *dest, uint32_t *s, int n)
{
  uint8_t *src = (uint8_t *)s;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*2 + 0] = AYUV(0xff, src[i*4 + 0], src[i*4 + 1], src[i*4 + 3]);
    dest[i*2 + 1] = AYUV(0xff, src[i*4 + 2], src[i*4 + 1], src[i*4 + 3]);
  }
  if (n&1) {
    dest[n-1] = AYUV(0xff, src[i*4 + 0], src[i*4 + 1], src[i*4 + 3]);
  }
}
OIL_DEFINE_IMPL_REF (yuyv2ayuv_ref, yuyv2ayuv);

static void
yvyu2ayuv_ref (uint32_t *dest, uint32_t *s, int n)
{
  uint8_t *src = (uint8_t *)s;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*2 + 0] = AYUV(0xff, src[i*4 + 0], src[i*4 + 3], src[i*4 + 1]);
    dest[i*2 + 1] = AYUV(0xff, src[i*4 + 2], src[i*4 + 3], src[i*4 + 1]);
  }
  if (n&1) {
    dest[n-1] = AYUV(0xff, src[i*4 + 0], src[i*4 + 3], src[i*4 + 1]);
  }
}
OIL_DEFINE_IMPL_REF (yvyu2ayuv_ref, yvyu2ayuv);

static void
uyvy2ayuv_ref (uint32_t *dest, uint32_t *s, int n)
{
  uint8_t *src = (uint8_t *)s;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*2 + 0] = AYUV(0xff, src[i*4 + 1], src[i*4 + 0], src[i*4 + 2]);
    dest[i*2 + 1] = AYUV(0xff, src[i*4 + 3], src[i*4 + 0], src[i*4 + 2]);
  }
  if (n&1) {
    dest[n-1] = AYUV(0xff, src[i*4 + 1], src[i*4 + 0], src[i*4 + 2]);
  }
}
OIL_DEFINE_IMPL_REF (uyvy2ayuv_ref, uyvy2ayuv);



static void
ayuv2yuyv_ref (uint32_t *d, uint32_t *src, int n)
{
  uint8_t *dest = (uint8_t *)d;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*4 + 0] = AYUV_Y(src[i*2 + 0]);
    dest[i*4 + 1] = (AYUV_U(src[i*2 + 0]) + AYUV_U(src[i*2] + 1))/2;
    dest[i*4 + 2] = AYUV_Y(src[i*2 + 1]);
    dest[i*4 + 3] = (AYUV_V(src[i*2 + 0]) + AYUV_V(src[i*2] + 1))/2;
  }
  if (n&1) {
    dest[(n/2)*4 + 0] = AYUV_Y(src[n-1]);
    dest[(n/2)*4 + 1] = AYUV_U(src[n-1]);
    dest[(n/2)*4 + 2] = 0;
    dest[(n/2)*4 + 3] = AYUV_V(src[n-1]);
  }
}
OIL_DEFINE_IMPL_REF (ayuv2yuyv_ref, ayuv2yuyv);

static void
ayuv2yvyu_ref (uint32_t *d, uint32_t *src, int n)
{
  uint8_t *dest = (uint8_t *)d;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*4 + 0] = AYUV_Y(src[i*2 + 0]);
    dest[i*4 + 1] = (AYUV_V(src[i*2 + 0]) + AYUV_V(src[i*2] + 1))/2;
    dest[i*4 + 2] = AYUV_Y(src[i*2 + 1]);
    dest[i*4 + 3] = (AYUV_U(src[i*2 + 0]) + AYUV_U(src[i*2] + 1))/2;
  }
  if (n&1) {
    dest[(n/2)*4 + 0] = AYUV_Y(src[n-1]);
    dest[(n/2)*4 + 1] = AYUV_V(src[n-1]);
    dest[(n/2)*4 + 2] = 0;
    dest[(n/2)*4 + 3] = AYUV_U(src[n-1]);
  }
}
OIL_DEFINE_IMPL_REF (ayuv2yvyu_ref, ayuv2yvyu);

static void
ayuv2uyvy_ref (uint32_t *d, uint32_t *src, int n)
{
  uint8_t *dest = (uint8_t *)d;
  int i;

  for(i=0;i<n/2;i++){
    dest[i*4 + 0] = (AYUV_U(src[i*2 + 0]) + AYUV_U(src[i*2] + 1))/2;
    dest[i*4 + 1] = AYUV_Y(src[i*2 + 0]);
    dest[i*4 + 2] = (AYUV_V(src[i*2 + 0]) + AYUV_V(src[i*2] + 1))/2;
    dest[i*4 + 3] = AYUV_Y(src[i*2 + 1]);
  }
  if (n&1) {
    dest[(n/2)*4 + 0] = AYUV_U(src[n-1]);
    dest[(n/2)*4 + 1] = AYUV_Y(src[n-1]);
    dest[(n/2)*4 + 2] = AYUV_V(src[n-1]);
    dest[(n/2)*4 + 3] = 0;
  }
}
OIL_DEFINE_IMPL_REF (ayuv2uyvy_ref, ayuv2uyvy);

static void
packyuyv_ref (uint32_t *d, uint8_t *s1_nx2, uint8_t *s2, uint8_t *s3, int n)
{
  int i;
  uint8_t *dest = (uint8_t *)d;

  for(i=0;i<n;i++){
    dest[i*4 + 0] = s1_nx2[i*2+0];
    dest[i*4 + 2] = s1_nx2[i*2+1];
    dest[i*4 + 1] = s2[i];
    dest[i*4 + 3] = s3[i];
  }
}
OIL_DEFINE_IMPL_REF (packyuyv_ref, packyuyv);

static void
unpackyuyv_ref (uint8_t *d1_nx2, uint8_t *d2, uint8_t *d3, uint32_t *s, int n)
{
  int i;
  uint8_t *src = (uint8_t *)s;

  for(i=0;i<n;i++){
    d1_nx2[i*2+0] = src[i*4 + 0];
    d1_nx2[i*2+1] = src[i*4 + 2];
    d2[i] = src[i*4 + 1];
    d3[i] = src[i*4 + 3];
  }
}
OIL_DEFINE_IMPL_REF (unpackyuyv_ref, unpackyuyv);

