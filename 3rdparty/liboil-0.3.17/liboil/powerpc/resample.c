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


OIL_DECLARE_CLASS (merge_linear_argb);

#define ALIGN_UP(ptr,boundary) ((void *)(((unsigned long)ptr + boundary-1) & (~(boundary-1))))

#if 0
static uint16_t consts[8] __attribute__ ((__aligned__ (16))) = {
  256, 256, 256, 256, 256, 256, 256, 256 };
#endif

static void
merge_linear_argb_powerpc (uint32_t *dest, uint32_t *src1, uint32_t *src2,
    uint32_t *src3, int n)
{
  uint32_t tmp[48];
  uint32_t *atmp;
  int i;

  atmp = ALIGN_UP(tmp,16);
  if (src3[0] == 0) {
    for(i=0;i<n;i++){
      dest[i] = src1[i];
    }
  } else if (src3[0] == 256) {
    for(i=0;i<n;i++){
      dest[i] = src2[i];
    }
  } else {
    for(i=0;i<n;i+=2){
      atmp[0] = src1[i];
      atmp[1] = src1[i+1];
      atmp[2] = src2[i];
      atmp[3] = src2[i+1];
      atmp[4] = 256 - src3[0];
      atmp[5] = src3[0];

      asm volatile(
          "  lvx v0, 0, %0\n"
          "  li r11, 16\n"
          "  lvx v1, r11, %0\n"
          "  vxor v2, v2, v2\n"
          "  vmrghb v3, v2, v0\n"
          "  vmrglb v4, v2, v0\n"
          "  vsplth v5, v1, 1\n"
          "  vsplth v6, v1, 3\n"
          "  vmuloub v0, v4, v6\n"
          "  vmuloub v1, v3, v5\n"
          "  vadduhs v0, v0, v1\n"
          "  vspltish v1, 8\n"
          "  vsrh v0, v0, v1\n"
          "  vpkuhus v0, v0, v0\n"
          "  stvx v0, 0, %0\n"
          :
          : "b" (atmp));

      dest[i] = atmp[0];
      if (i+1<n) dest[i+1] = atmp[1];
    }
  }
}
OIL_DEFINE_IMPL_FULL (merge_linear_argb_powerpc, merge_linear_argb, OIL_IMPL_FLAG_ALTIVEC);


