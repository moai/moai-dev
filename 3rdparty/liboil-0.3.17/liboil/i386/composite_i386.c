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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS (composite_in_argb);
OIL_DECLARE_CLASS (composite_in_argb_const_src);
OIL_DECLARE_CLASS (composite_in_argb_const_mask);
OIL_DECLARE_CLASS (composite_over_argb);
OIL_DECLARE_CLASS (composite_over_argb_const_src);
OIL_DECLARE_CLASS (composite_add_argb);
OIL_DECLARE_CLASS (composite_add_argb_const_src);
OIL_DECLARE_CLASS (composite_in_over_argb);
OIL_DECLARE_CLASS (composite_in_over_argb_const_src);
OIL_DECLARE_CLASS (composite_in_over_argb_const_mask);

#if 0
static void
composite_in_argb_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_IN(ARGB_A(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_R(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_G(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_B(src[i]), mask[i]));
  }
}
OIL_DEFINE_IMPL_FULL (composite_in_argb_mmx, composite_in_argb);
#endif

/*
 * This macro loads the constants:
 * mm7 = { 0, 0, 0, 0 }
 * mm6 = { 128, 128, 128, 128 }
 * mm5 = { 255, 255, 255, 255 }
 */
#define MMX_LOAD_CONSTANTS \
      "  pxor %%mm7, %%mm7\n" \
      "  movl $0x80808080, %%eax\n" \
      "  movd %%eax, %%mm6\n" \
      "  punpcklbw %%mm7, %%mm6\n" \
      "  movl $0xffffffff, %%eax\n" \
      "  movd %%eax, %%mm5\n" \
      "  punpcklbw %%mm7, %%mm5\n"

/*
 * a = muldiv255(a, b)
 *   a, b are unpacked
 *   destroys both registers
 *   requires mm6 set up as above
 */
#define MMX_MULDIV255(a,b) \
      "  pmullw %%" #b ", %%" #a "\n" \
      "  paddw %%mm6, %%" #a "\n" \
      "  movq %%" #a ", %%" #b "\n" \
      "  psrlw $8, %%" #b "\n" \
      "  paddw %%" #b ", %%" #a "\n" \
      "  psrlw $8, %%" #a "\n"

static void
composite_in_argb_mmx (uint32_t *dest, uint32_t *src, const uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "1:\n"
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm1\n"

      "  movd (%1), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  addl $1, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_argb_mmx, composite_in_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

/* 
 * This is a different style than the others.  Should be moved elsewhere.
 */
static void
composite_in_argb_mmx2 (uint32_t *dest, uint32_t *src, const uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "1:\n"
      "  movl (%2), %%eax\n"
      /* if alpha == 0, write a 0 */
      "  testl $0x000000ff, %%eax\n"
      "  je 2f\n"
      /* if alpha == 0xff, write src value */
      "  cmp $0xff, %%al\n"
      "  je 3f\n"

      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm1\n"

      "  movd (%1), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  jmp 4f\n"
      "2:\n"
      "  movl $0, (%0)\n"
      "  jmp 4f\n"
      "3:\n"
      "  movl (%1), %%eax\n"
      "  movl %%eax, (%0)\n"
      "4:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  addl $1, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_argb_mmx2, composite_in_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_in_argb_const_src_mmx (uint32_t *dest, uint32_t *src, const uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "  movd (%1), %%mm3\n"
      "  punpcklbw %%mm7, %%mm3\n"
      "1:\n"
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm1\n"

      "  movq %%mm3, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $1, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_argb_const_src_mmx, composite_in_argb_const_src, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_in_argb_const_mask_mmx (uint32_t *dest, uint32_t *src, const uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm3\n"
      "1:\n"
      "  movq %%mm3, %%mm1\n"
      "  movd (%1), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_argb_const_mask_mmx, composite_in_argb_const_mask, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_over_argb_mmx (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0xff, %%mm0, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx, composite_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

/* unroll 2 */
static void
composite_over_argb_mmx_2 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS

      "  testl $0x1, %2\n"
      "  jz 2f\n"

      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 1f\n"

      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0xff, %%mm0, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmullw %%mm1, %%mm2\n"
      "  paddw %%mm6, %%mm2\n"
      "  movq %%mm2, %%mm1\n"
      "  psrlw $8, %%mm1\n"
      "  paddw %%mm1, %%mm2\n"
      "  psrlw $8, %%mm2\n"

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"

      "1:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"

      "2:\n"
      "  shr $1, %2\n"
      "  jz 5f\n"
      "3:\n"
      "  movl (%1), %%eax\n"
      "  orl 4(%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 4f\n"

      "  movd (%1), %%mm0\n"
      "  movd (%0), %%mm2\n"

      "  punpcklbw %%mm7, %%mm0\n"
      "   movd 4(%1), %%mm3\n"

      "  pshufw $0xff, %%mm0, %%mm1\n"
      "  punpcklbw %%mm7, %%mm2\n"

      "  pxor %%mm5, %%mm1\n"
      "   movd 4(%0), %%mm4\n"

      "  pmullw %%mm1, %%mm2\n"
      "   punpcklbw %%mm7, %%mm3\n"

      "  paddw %%mm6, %%mm2\n"
      "   punpcklbw %%mm7, %%mm4\n"

      "  movq %%mm2, %%mm1\n"
      "   pshufw $0xff, %%mm3, %%mm7\n"

      "  psrlw $8, %%mm1\n"
      "   pxor %%mm5, %%mm7\n"

      "  paddw %%mm1, %%mm2\n"
      "   pmullw %%mm7, %%mm4\n"

      "  psrlw $8, %%mm2\n"
      "   paddw %%mm6, %%mm4\n"

      "  paddw %%mm0, %%mm2\n"
      "   movq %%mm4, %%mm7\n"

      "  packuswb %%mm2, %%mm2\n"
      "   psrlw $8, %%mm7\n"

      "  movd %%mm2, (%0)\n"
      "   paddw %%mm7, %%mm4\n"

      "   psrlw $8, %%mm4\n"
      "   paddw %%mm3, %%mm4\n"
      "   packuswb %%mm4, %%mm4\n"
      "   movd %%mm4, 4(%0)\n"

      "  pxor %%mm7, %%mm7\n"
      "4:\n"
      "  addl $8, %0\n"
      "  addl $8, %1\n"
      "  decl %2\n"
      "  jnz 3b\n"
      "5:\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx_2, composite_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

/* replace pshufw with punpck */
static void
composite_over_argb_mmx_3 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  movq %%mm0, %%mm1\n"
      "  punpckhwd %%mm1, %%mm1\n"
      "  punpckhdq %%mm1, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmullw %%mm1, %%mm2\n"
      "  paddw %%mm6, %%mm2\n"
      "  movq %%mm2, %%mm1\n"
      "  psrlw $8, %%mm1\n"
      "  paddw %%mm1, %%mm2\n"
      "  psrlw $8, %%mm2\n"

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"
      "  movd %%mm2, (%0)\n"

      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx_3, composite_over_argb, OIL_IMPL_FLAG_MMX);

/* written for gromit */
static void
composite_over_argb_mmx_4 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%mm7, %%mm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%mm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%mm7, %%mm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%mm5\n"
      "  punpcklbw %%mm7, %%mm5\n"
      "  movl $0x02020202, %%eax\n"
      "  movd %%eax, %%mm4\n"
      "  punpcklbw %%mm7, %%mm4\n"
      "  paddw %%mm5, %%mm4\n" // mm5 = { 257, 257, 257, 257 }
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0xff, %%mm0, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmullw %%mm1, %%mm2\n"
      "  paddw %%mm6, %%mm2\n"
      "  pmulhuw %%mm4, %%mm2\n"

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  subl $1, %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx_4, composite_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_over_argb_mmx_5 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%mm7, %%mm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%mm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%mm7, %%mm6\n"
#if 0
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%mm5\n"
      "  punpcklbw %%mm7, %%mm5\n"
#else
      "  pcmpeqw %%mm5, %%mm5\n"
      "  psrlw $8, %%mm5\n" // mm5 = { 255, 255, 255, 255 }
#endif
      "  movl $0x02020202, %%eax\n"
      "  movd %%eax, %%mm4\n"
      "  punpcklbw %%mm7, %%mm4\n"
      "  paddw %%mm5, %%mm4\n" // mm5 = { 257, 257, 257, 257 }
      "1:\n"
      "  movd (%1), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  xor %%eax, %%eax\n"
      "  pextrw $3, %%mm0, %%eax\n"
      "  test %%eax, %%eax\n"
      "  jz 2f\n"

      "  pshufw $0xff, %%mm0, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  pmullw %%mm1, %%mm2\n"
      "  paddw %%mm6, %%mm2\n"
      "  pmulhuw %%mm4, %%mm2\n"

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  subl $1, %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx_5, composite_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_over_argb_sse2 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%xmm7, %%xmm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%xmm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%xmm7, %%xmm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%xmm5\n"
      "  punpcklbw %%xmm7, %%xmm5\n"
      "  movl $0x02020202, %%eax\n"
      "  movd %%eax, %%xmm4\n"
      "  punpcklbw %%xmm7, %%xmm4\n"
      "  paddw %%xmm5, %%xmm4\n" // mm4 = { 255, 255, 255, 255 }
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd (%1), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movd (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"

      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movd %%xmm3, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_sse2, composite_over_argb, OIL_IMPL_FLAG_SSE2);

/* written for shaun */
static void
composite_over_argb_sse2_2 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%xmm7, %%xmm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%xmm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%xmm7, %%xmm6\n"
      "  punpcklwd %%xmm6, %%xmm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%xmm5\n"
      "  punpcklbw %%xmm7, %%xmm5\n"
      "  punpcklwd %%xmm5, %%xmm5\n"
      "  movl $0x02020202, %%eax\n"
      "  movd %%eax, %%xmm4\n"
      "  punpcklbw %%xmm7, %%xmm4\n"
      "  paddw %%xmm5, %%xmm4\n" // mm4 = { 257, 257, 257, 257 }
      "  punpcklwd %%xmm4, %%xmm4\n"
      :
      :
      :"eax");

  if (n&1) {
    __asm__ __volatile__ (
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 1f\n"

      "  movd (%1), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movd (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"

      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movd %%xmm3, (%0)\n"

      "1:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      :"+r" (dest), "+r" (src)
      :
      :"eax");
  }
  n>>=1;

  if (n>0){
    __asm__ __volatile__ ("\n"
      "3:\n"
#if 0
      "  movl (%1), %%eax\n"
      "  orl 4(%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 4f\n"
#endif

      "  movq (%1), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
      "  pshufhw $0xff, %%xmm0, %%xmm0\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movq (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"
      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movq %%xmm3, (%0)\n"
      "4:\n"
      "  addl $8, %0\n"
      "  addl $8, %1\n"
      "  subl $1, %2\n"
      "  jnz 3b\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");
  }
}
OIL_DEFINE_IMPL_FULL (composite_over_argb_sse2_2, composite_over_argb, OIL_IMPL_FLAG_SSE2);

/* written for shaun */
static void
composite_over_argb_sse2_3 (uint32_t *dest, uint32_t *src, int n)
{
  int begin;
  int middle;
  int end;

  __asm__ __volatile__ ("  pxor %%xmm7, %%xmm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%xmm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%xmm7, %%xmm6\n"
      "  punpcklwd %%xmm6, %%xmm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%xmm5\n"
      "  punpcklbw %%xmm7, %%xmm5\n"
      "  punpcklwd %%xmm5, %%xmm5\n"
      "  movl $0x02020202, %%eax\n"
      "  movd %%eax, %%xmm4\n"
      "  punpcklbw %%xmm7, %%xmm4\n"
      "  paddw %%xmm5, %%xmm4\n" // mm4 = { 257, 257, 257, 257 }
      "  punpcklwd %%xmm4, %%xmm4\n"
      :
      :
      :"eax");

  begin = 0x3 & (4 - (((unsigned long)dest & 0xf) >> 2));
  if (begin>n) {
    begin = n;
    middle = 0;
    end = 0;
  } else {
    middle = (n-begin)>>2;
    end = n - begin - middle*4;
  }

  if (begin>0) {
    __asm__ __volatile__ ("\n"
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd (%1), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movd (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"

      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movd %%xmm3, (%0)\n"

      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  subl $1, %2\n"
      "  jnz 1b\n"
      :"+r" (dest), "+r" (src), "+r" (begin)
      :
      :"eax");
  }

  if (middle>0){
    __asm__ __volatile__ ("\n"
      "1:\n"
      "  movq (%1), %%xmm1\n"
      "  movq 8(%1), %%xmm0\n"
      "  movl (%1), %%eax\n"
      "  orl 4(%1), %%eax\n"
      "  orl 8(%1), %%eax\n"
      "  orl 12(%1), %%eax\n"
      "  test $0xff000000, %%eax\n"
      "  jz 2f\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm0\n"
      "  pshuflw $0xff, %%xmm1, %%xmm1\n"
      "  pshuflw $0xff, %%xmm0, %%xmm0\n"
      "  pshufhw $0xff, %%xmm1, %%xmm1\n"
      "  pshufhw $0xff, %%xmm0, %%xmm0\n"

      "  pxor %%xmm5, %%xmm1\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movq (%0), %%xmm3\n"
      "  movq 8(%0), %%xmm2\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm2\n"

      "  pmullw %%xmm1, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm2\n"
      "  paddw %%xmm6, %%xmm2\n"
      "  pmulhuw %%xmm4, %%xmm2\n"
      "  packuswb %%xmm2, %%xmm3\n"

      "  movdqu (%1), %%xmm1\n"
      "  paddb %%xmm1, %%xmm3\n"
      "  movdqa %%xmm3, (%0)\n"
      "2:\n"
      "  addl $16, %0\n"
      "  addl $16, %1\n"
      "  subl $1, %2\n"
      "  jnz 1b\n"
      :"+r" (dest), "+r" (src), "+r" (middle)
      :
      :"eax");
  }
  if (end>0) {
    __asm__ __volatile__ ("\n"
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd (%1), %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
      "  pxor %%xmm5, %%xmm0\n"

      "  movd (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  pmulhuw %%xmm4, %%xmm3\n"

      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movd %%xmm3, (%0)\n"

      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  subl $1, %2\n"
      "  jnz 1b\n"
      :"+r" (dest), "+r" (src), "+r" (end)
      :
      :"eax");
  }

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_sse2_3, composite_over_argb, OIL_IMPL_FLAG_SSE2);


static void
composite_over_argb_const_src_mmx (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "  movl (%1), %%eax\n"
      "  movd %%eax, %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0xff, %%mm0, %%mm3\n"
      "  pxor %%mm5, %%mm3\n"
      "1:\n"
      "  movq %%mm3, %%mm1\n"
      "  movd (%0), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_const_src_mmx, composite_over_argb_const_src, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_add_argb_mmx (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      "1:\n"
      "  movd (%1), %%mm0\n"
      "  movd (%0), %%mm2\n"
      "  paddusb %%mm0, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_add_argb_mmx, composite_add_argb, OIL_IMPL_FLAG_MMX);

static void
composite_add_argb_const_src_mmx (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ (
      "  movd (%1), %%mm0\n"
      "1:\n"
      "  movd (%0), %%mm2\n"
      "  paddusb %%mm0, %%mm2\n"
      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_add_argb_const_src_mmx, composite_add_argb_const_src, OIL_IMPL_FLAG_MMX);

static void
composite_in_over_argb_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "1:\n"
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm1\n"

      "  movd (%1), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  movd (%0), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"

      "  pshufw $0xff, %%mm2, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      MMX_MULDIV255(mm0, mm1)

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  addl $1, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_mmx, composite_in_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_in_over_argb_const_src_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS

      "  movd (%1), %%mm3\n"
      "  punpcklbw %%mm7, %%mm3\n"
      "1:\n"
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm1\n"

      "  movq %%mm3, %%mm2\n"

      MMX_MULDIV255(mm2, mm1)

      "  movd (%0), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"

      "  pshufw $0xff, %%mm2, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      MMX_MULDIV255(mm0, mm1)

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $1, %2\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_const_src_mmx, composite_in_over_argb_const_src, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_in_over_argb_const_mask_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  __asm__ __volatile__ (
      MMX_LOAD_CONSTANTS
      "  movd (%2), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"
      "  pshufw $0x00, %%mm0, %%mm3\n"

      "1:\n"
      "  movd (%1), %%mm2\n"
      "  punpcklbw %%mm7, %%mm2\n"
      "  movq %%mm3, %%mm1\n"

      MMX_MULDIV255(mm2, mm1)

      "  movd (%0), %%mm0\n"
      "  punpcklbw %%mm7, %%mm0\n"

      "  pshufw $0xff, %%mm2, %%mm1\n"
      "  pxor %%mm5, %%mm1\n"

      MMX_MULDIV255(mm0, mm1)

      "  paddw %%mm0, %%mm2\n"
      "  packuswb %%mm2, %%mm2\n"

      "  movd %%mm2, (%0)\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %3\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (mask), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_const_mask_mmx, composite_in_over_argb_const_mask, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

