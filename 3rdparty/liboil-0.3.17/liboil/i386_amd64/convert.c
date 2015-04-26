/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2006 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilclasses.h>

static void
convert_u8_s16_mmx (uint8_t * dest, const int16_t * src, int n)
{
  while(n&7) {
    int x;
    x = src[0];
    if (x<0) x = 0;
    if (x>255) x = 255;
    dest[0] = x;
    src++;
    dest++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  __asm__ __volatile__ ("\n"
      "1:\n"
      "  movq 0(%1), %%mm0\n"
      "  packuswb 8(%1), %%mm0\n"
      "  movq %%mm0, 0(%0)\n"
      "  add $16, %1\n"
      "  add $8, %0\n"
      "  decl %2\n"
      "  jg 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n));
}
OIL_DEFINE_IMPL_FULL (convert_u8_s16_mmx, convert_u8_s16, OIL_IMPL_FLAG_MMX);

static void
convert_u8_s16_mmx_2 (uint8_t * dest, const int16_t * src, int n)
{
  while(n&7) {
    int x;
    x = src[0];
    if (x<0) x = 0;
    if (x>255) x = 255;
    dest[0] = x;
    src++;
    dest++;
    n--;
  }
  if (n==0) return;

  n>>=3;
  if (n&1) {
    __asm__ __volatile__ ("\n"
        "  movq 0(%1), %%mm0\n"
        "  packuswb 8(%1), %%mm0\n"
        "  movq %%mm0, 0(%0)\n"
        "  add $16, %1\n"
        "  add $8, %0\n"
        : "+r" (dest), "+r" (src), "+r" (n));
  }

  n >>= 1;
  if (n > 0) {
    __asm__ __volatile__ ("\n"
        "2:\n"
        "  movq 0(%1), %%mm0\n"
        "  packuswb 8(%1), %%mm0\n"
        "  movq %%mm0, 0(%0)\n"
        "  movq 16(%1), %%mm0\n"
        "  packuswb 24(%1), %%mm0\n"
        "  movq %%mm0, 8(%0)\n"
        "  add $32, %1\n"
        "  add $16, %0\n"
        "  decl %2\n"
        "  jg 2b\n"
        : "+r" (dest), "+r" (src), "+r" (n));
  }
  __asm__ __volatile__ ("emms\n");
}
OIL_DEFINE_IMPL_FULL (convert_u8_s16_mmx_2, convert_u8_s16, OIL_IMPL_FLAG_MMX);

static void
convert_s16_u8_mmx (int16_t * dest, const uint8_t * src, int n)
{
  while(n&7) {
    dest[0] = src[0];
    src++;
    dest++;
    n--;
  }
  if (n==0) return;

  n>>=3;

  __asm__ __volatile__ ("\n"
      "  pxor %%mm0, %%mm0\n"
      "1:\n"
      "  movd 0(%1), %%mm1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%0)\n"
      "   movd 4(%1), %%mm2\n"
      "   punpcklbw %%mm0, %%mm2\n"
      "   movq %%mm2, 8(%0)\n"
      "  add $8, %1\n"
      "  add $16, %0\n"
      "  decl %2\n"
      "  jg 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src), "+r" (n));
}
OIL_DEFINE_IMPL_FULL (convert_s16_u8_mmx, convert_s16_u8, OIL_IMPL_FLAG_MMX);

