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

static void
swab_u32_i386 (uint32_t *dest, uint32_t *src, int n)
{
  asm volatile ("\n"
      "1:\n"
      "  movl 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  movl %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL (swab_u32_i386, swab_u32);

static void
swab_u32_i386_unroll2 (uint32_t *dest, uint32_t *src, int n)
{
  asm volatile ("\n"
      "  test $1, %2\n"
      "  jz 1f\n"
      "  movl 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  movl %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "  jz 2f\n"
      "1:\n"
      "  movl 0(%1), %%eax\n"
      "  movl 4(%1), %%ecx\n"
      "  bswap %%eax\n"
      "  bswap %%ecx\n"
      "  movl %%eax, 0(%0)\n"
      "  movl %%ecx, 4(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  sub $2, %2\n"
      "  jnz 1b\n"
      "2:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "ecx", "memory");
}
OIL_DEFINE_IMPL (swab_u32_i386_unroll2, swab_u32);

static void
swab_u32_i386_unroll4 (uint32_t *dest, uint32_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movl 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  movl %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $3, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movl 0(%1), %%eax\n"
      "  movl 4(%1), %%ecx\n"
      "  bswap %%eax\n"
      "  bswap %%ecx\n"
      "  movl %%eax, 0(%0)\n"
      "  movl %%ecx, 4(%0)\n"
      "  movl 8(%1), %%eax\n"
      "  movl 12(%1), %%ecx\n"
      "  bswap %%eax\n"
      "  bswap %%ecx\n"
      "  movl %%eax, 8(%0)\n"
      "  movl %%ecx, 12(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  sub $4, %2\n"
      "  jnz 3b\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "ecx", "memory");
}
OIL_DEFINE_IMPL (swab_u32_i386_unroll4, swab_u32);



/*
 * This could be improved by using aligned stores
 */
static void
swab_u16_mmx (uint16_t *dest, uint16_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movw 0(%1), %%ax\n"
      "  xchg %%al, %%ah\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $3, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movq 0(%1), %%mm0\n"
      "  movq 0(%1), %%mm1\n"
      "  psllw $8, %%mm0\n"
      "  psrlw $8, %%mm1\n"
      "  por %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  sub $4, %2\n"
      "  jnz 3b\n"
      "  emms\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL_FULL (swab_u16_mmx, swab_u16, OIL_IMPL_FLAG_MMX);

static void
swab_u16_mmx_unroll2 (uint16_t *dest, uint16_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movw 0(%1), %%ax\n"
      "  xchg %%al, %%ah\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $7, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movq 0(%1), %%mm0\n"
      "  movq 0(%1), %%mm1\n"
      "   movq 8(%1), %%mm2\n"
      "  psllw $8, %%mm0\n"
      "   movq 8(%1), %%mm3\n"
      "  psrlw $8, %%mm1\n"
      "   psllw $8, %%mm2\n"
      "  por %%mm0, %%mm1\n"
      "   psrlw $8, %%mm3\n"
      "  movq %%mm1, 0(%0)\n"
      "   por %%mm2, %%mm3\n"
      "   movq %%mm3, 8(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  sub $8, %2\n"
      "  jnz 3b\n"
      "  emms\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL_FULL (swab_u16_mmx_unroll2, swab_u16, OIL_IMPL_FLAG_MMX);

/* 10 instructions to swab 2 words?  not likely */
static void
swab_u32_mmx (uint16_t *dest, uint16_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movl 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  movl %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $1, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movq 0(%1), %%mm0\n"
      "  movq 0(%1), %%mm1\n"
      "  psllw $8, %%mm0\n"
      "  psrlw $8, %%mm1\n"
      "  por %%mm0, %%mm1\n"
      "  movq %%mm1, %%mm0\n"
      "  pslld $16, %%mm0\n"
      "  psrld $16, %%mm1\n"
      "  por %%mm0, %%mm1\n"
      "  movq %%mm1, 0(%0)\n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  sub $2, %2\n"
      "  jnz 3b\n"
      "  emms\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL_FULL (swab_u32_mmx, swab_u32, OIL_IMPL_FLAG_MMX);

static void
swab_u16_sse2 (uint16_t *dest, uint16_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movw 0(%1), %%ax\n"
      "  xchg %%al, %%ah\n"
      "  movw %%ax, 0(%0)\n"
      "  add $2, %0\n"
      "  add $2, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $7, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movdqu 0(%1), %%xmm0\n"
      "  movdqu 0(%1), %%xmm1\n"
      "  psllw $8, %%xmm0\n"
      "  psrlw $8, %%xmm1\n"
      "  por %%xmm0, %%xmm1\n"
      "  movdqu %%xmm1, 0(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  sub $8, %2\n"
      "  jnz 3b\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL_FULL (swab_u16_sse2, swab_u16, OIL_IMPL_FLAG_SSE2);

static void
swab_u32_sse2 (uint16_t *dest, uint16_t *src, int n)
{
  asm volatile ("\n"
      "  jmp 2f\n"
      "1:\n"
      "  movl 0(%1), %%eax\n"
      "  bswap %%eax\n"
      "  movl %%eax, 0(%0)\n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2\n"
      "2:\n"
      "  test $3, %2\n"
      "  jnz 1b\n"
      "  test %2, %2\n"
      "  jz 4f\n"
      "3:\n"
      "  movdqu 0(%1), %%xmm0\n"
      "  movdqu 0(%1), %%xmm1\n"
      "  psllw $8, %%xmm0\n"
      "  psrlw $8, %%xmm1\n"
      "  por %%xmm0, %%xmm1\n"
      "  movdqu %%xmm1, %%xmm0\n"
      "  pslld $16, %%xmm0\n"
      "  psrld $16, %%xmm1\n"
      "  por %%xmm0, %%xmm1\n"
      "  movdqu %%xmm1, 0(%0)\n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  sub $4, %2\n"
      "  jnz 3b\n"
      "4:\n"
      : "+r" (dest), "+r" (src), "+r" (n)
      :
      : "eax", "memory");
}
OIL_DEFINE_IMPL_FULL (swab_u32_sse2, swab_u32, OIL_IMPL_FLAG_SSE2);

