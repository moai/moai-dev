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

#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>

#include <stdio.h>

static void
merge_linear_u8_mmx (uint8_t *dest, uint8_t *src1, uint8_t *src2,
    uint32_t *src3, int n)
{
  unsigned int x = src3[0];
  while (n&3) {
    dest[0] = (src1[0]*(256-x) + src2[0]*x) >> 8;
    dest++;
    src1++;
    src2++;
    n--;
  }
  n >>= 2;
  if (n == 0) return;
  x |= (x<<16);
  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movd %3, %%mm6\n"
      "  pshufw $0x00, %%mm6, %%mm6\n"
      "  movl $0x01010101, %3\n"
      "  movd %3, %%mm5\n"
      "  punpcklbw %%mm7, %%mm5\n"
      "  psllw $8, %%mm5\n"
      "  psubw %%mm6, %%mm5\n"
      "1:\n"
      "  movd 0(%1), %%mm0\n"
      "  movd 0(%2), %%mm1\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pmullw %%mm5, %%mm0\n"
      "  pmullw %%mm6, %%mm1\n"
      "  paddw %%mm1, %%mm0\n"
      "  psrlw $8, %%mm0\n"
      "  packuswb %%mm0, %%mm0\n"
      "  movd %%mm0, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  add $4, %2\n"
      "  decl %4\n"
      "  jnz 1b\n"
      "  emms\n"
      : "+r" (dest), "+r" (src1), "+r" (src2), "+r" (x), "+r" (n)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (merge_linear_u8_mmx, merge_linear_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
merge_linear_u8_sse2 (uint8_t *dest, uint8_t *src1, uint8_t *src2,
    uint32_t *src3, int n)
{
  unsigned int x = src3[0];
  while (n&7) {
    dest[0] = (src1[0]*(256-x) + src2[0]*x) >> 8;
    dest++;
    src1++;
    src2++;
    n--;
  }
  n >>= 3;
  if (n == 0) return;
  x |= (x<<16);
  asm volatile ("\n"
      "  pxor %%xmm7, %%xmm7\n"
      "  movd %3, %%xmm6\n"
      "  pshufd $0x00, %%xmm6, %%xmm6\n"
      "  movl $0x01010101, %3\n"
      "  movd %3, %%xmm5\n"
      "  pshufd $0x00, %%xmm5, %%xmm5\n"
      "  punpcklbw %%xmm7, %%xmm5\n"
      "  psllw $8, %%xmm5\n"
      "  psubw %%xmm6, %%xmm5\n"
      "1:\n"
      "  movq 0(%1), %%xmm0\n"
      "  movq 0(%2), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm0\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pmullw %%xmm5, %%xmm0\n"
      "  pmullw %%xmm6, %%xmm1\n"
      "  paddw %%xmm1, %%xmm0\n"
      "  psrlw $8, %%xmm0\n"
      "  packuswb %%xmm0, %%xmm0\n"
      "  movq %%xmm0, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  add $8, %2\n"
      "  decl %4\n"
      "  jnz 1b\n"
      : "+r" (dest), "+r" (src1), "+r" (src2), "+r" (x), "+r" (n)
      :
      : "memory");
}
OIL_DEFINE_IMPL_FULL (merge_linear_u8_sse2, merge_linear_u8, OIL_IMPL_FLAG_SSE2);

