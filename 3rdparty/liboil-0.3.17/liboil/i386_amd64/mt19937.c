/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2007 David A. Schleef <ds@schleef.org>
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
#include <config.h>
#endif

#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


#define TWIST(next,pair) \
      "  movd 0(%1), %%mm0 \n" \
      "  movd " #next "(%1), %%mm1 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "  por %%mm1, %%mm0 \n" \
      "  pslld $31, %%mm1 \n" \
      "  psrad $31, %%mm1 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "  psrld $1, %%mm0 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "  movd %%mm0, 0(%1) \n"

#define TWIST2(next,pair) \
      "  movq 0(%1), %%mm0 \n" \
      "  movq " #next "(%1), %%mm1 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "  por %%mm1, %%mm0 \n" \
      "  pslld $31, %%mm1 \n" \
      "  psrad $31, %%mm1 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "  psrld $1, %%mm0 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "  movq %%mm0, 0(%1) \n"

#define TWIST4(next,pair) \
      "  movq 0(%1), %%mm0 \n" \
      "   movq 8(%1), %%mm2 \n" \
      "  movq " #next "(%1), %%mm1 \n" \
      "   movq (8+" #next ")(%1), %%mm3 \n" \
      "  pand %%mm7, %%mm0 \n" \
      "   pand %%mm7, %%mm2 \n" \
      "  pand %%mm6, %%mm1 \n" \
      "   pand %%mm6, %%mm3 \n" \
      "  por %%mm1, %%mm0 \n" \
      "   por %%mm3, %%mm2 \n" \
      "  pslld $31, %%mm1 \n" \
      "   pslld $31, %%mm3 \n" \
      "  psrad $31, %%mm1 \n" \
      "   psrad $31, %%mm3 \n" \
      "  pand %%mm5, %%mm1 \n" \
      "   pand %%mm5, %%mm3 \n" \
      "  psrld $1, %%mm0 \n" \
      "   psrld $1, %%mm2 \n" \
      "  pxor %%mm1, %%mm0 \n" \
      "   pxor %%mm3, %%mm2 \n" \
      "  pxor " #pair "(%1), %%mm0 \n" \
      "   pxor (8+" #pair ")(%1), %%mm2 \n" \
      "  movq %%mm0, 0(%1) \n" \
      "   movq %%mm2, 8(%1) \n"


static void
mt19937_i386_mmx (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"
      "  mov $227, %2 \n"

      "1: \n"
      TWIST(4,1588)
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      "  mov $396, %2 \n"
      "2: \n"
      TWIST(4,-908)
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  mov $624, %2 \n"
      "3:\n"
      "  movd 0(%1), %%mm0 \n"
      "  movd 0(%1), %%mm1 \n"
      "  psrld $11, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $7, %%mm1 \n"
      "  pand %%mm7, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $15, %%mm1 \n"
      "  pand %%mm6, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  psrld $18, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movd %%mm0, 0(%0) \n"
      "  add $4, %0\n"
      "  add $4, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx, mt19937, OIL_IMPL_FLAG_MMX);

static void
mt19937_i386_mmx_2 (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5 \n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"
      "  pshufw $0x44, %%mm4, %%mm4 \n"

      "  mov $113, %2 \n"
      "1: \n"
      TWIST2(4,1588)
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      TWIST(4,1588)
      "  add $4, %1\n"

      "  mov $198, %2 \n"
      "2: \n"
      TWIST2(4,-908)
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $312, %2 \n"
      "3:\n"
      "  movq 0(%1), %%mm0 \n"
      "  movq 0(%1), %%mm1 \n"
      "  psrld $11, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $7, %%mm1 \n"
      "  pand %%mm7, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  pslld $15, %%mm1 \n"
      "  pand %%mm6, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, %%mm1 \n"
      "  psrld $18, %%mm1 \n"
      "  pxor %%mm1, %%mm0 \n"
      "  movq %%mm0, 0(%0) \n"
      "  add $8, %0\n"
      "  add $8, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx_2, mt19937, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


static void
mt19937_i386_mmx_3 (uint32_t *d, uint32_t *mt)
{
  asm volatile (
      "  mov $0x80000000, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0x7fffffff, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $0x9908b0df, %2\n"
      "  movd %2, %%mm5\n"
      "  pshufw $0x44, %%mm5, %%mm5 \n"
      "  mov $0x1, %2\n"
      "  movd %2, %%mm4\n"

      "  mov $56, %2 \n"
      "1: \n"
      TWIST4(4,1588)
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 1b \n"

      TWIST2(4,1588)
      "  add $8, %1\n"

      TWIST(4,1588)
      "  add $4, %1\n"

      "  mov $99, %2 \n"
      "2: \n"
      TWIST4(4,-908)
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 2b \n"

      TWIST(-2492,-908)

      "  add $-2492, %1 \n"

      "  mov $0x9d2c5680, %2\n"
      "  movd %2, %%mm7\n"
      "  pshufw $0x44, %%mm7, %%mm7 \n"
      "  mov $0xefc60000, %2\n"
      "  movd %2, %%mm6\n"
      "  pshufw $0x44, %%mm6, %%mm6 \n"
      "  mov $156, %2 \n"
      "3:\n"
      "  movq 0(%1), %%mm0 \n"
      "   movq 8(%1), %%mm2 \n"
      "  movq 0(%1), %%mm1 \n"
      "   movq 8(%1), %%mm3 \n"
      "  psrld $11, %%mm1 \n"
      "   psrld $11, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  pslld $7, %%mm1 \n"
      "   pslld $7, %%mm3 \n"
      "  pand %%mm7, %%mm1 \n"
      "   pand %%mm7, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  pslld $15, %%mm1 \n"
      "   pslld $15, %%mm3 \n"
      "  pand %%mm6, %%mm1 \n"
      "   pand %%mm6, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, %%mm1 \n"
      "   movq %%mm2, %%mm3 \n"
      "  psrld $18, %%mm1 \n"
      "   psrld $18, %%mm3 \n"
      "  pxor %%mm1, %%mm0 \n"
      "   pxor %%mm3, %%mm2 \n"
      "  movq %%mm0, 0(%0) \n"
      "   movq %%mm2, 8(%0) \n"
      "  add $16, %0\n"
      "  add $16, %1\n"
      "  decl %2 \n"
      "  jne 3b \n"
      "  emms \n"

      : "+r" (d), "+r" (mt)
      : "r" (0)
      );

}
OIL_DEFINE_IMPL_FULL (mt19937_i386_mmx_3, mt19937, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

#undef TWIST
#undef TWIST2
#undef TWIST4

#ifdef unused
static void
mt19937x8_mmx (uint32_t *d, uint32_t *mt)
{
  long i = 0;
  asm volatile (
      "  mov $0x80000000, %0\n"
      "  movd %0, %%mm5\n"
      "  punpckldq %%mm5, %%mm5\n"
      "  mov $0x7fffffff, %0\n"
      "  movd %0, %%mm6\n"
      "  punpckldq %%mm6, %%mm6\n"
      "  mov $0x9908b0df, %0\n"
      "  movd %0, %%mm7\n"
      "  punpckldq %%mm7, %%mm7\n"
      "  mov $0, %0\n"
      "1:\n"
#define TWIST(x,y) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "  movq 32+" #x "(%2,%0,1), %%mm1\n" \
      "  pand %%mm5, %%mm0\n" \
      "  pand %%mm6, %%mm1\n" \
      "  por %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  pslld $31, %%mm1\n" \
      "  psrad $31, %%mm1\n" \
      "  pand %%mm7, %%mm1\n" \
      "  psrld $1, %%mm0\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%mm0\n" \
      "  movq %%mm0, " #x "(%2,%0,1)\n"

      TWIST(0, 12704)
      TWIST(8, 12704)
      TWIST(16, 12704)
      TWIST(24, 12704)

      "  add $32, %0\n"
      "  cmp $7264, %0\n"
      "  jl 1b\n"

      "2:\n"

      TWIST(0, -7264)
      TWIST(8, -7264)
      TWIST(16, -7264)
      TWIST(24, -7264)

      "  add $32, %0\n"
      "  cmp $19936, %0\n"
      "  jl 2b\n"

#define TWIST2(x,y) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "  movq " #x "(%2), %%mm1\n" \
      "  pand %%mm5, %%mm0\n" \
      "  pand %%mm6, %%mm1\n" \
      "  por %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  pslld $31, %%mm1\n" \
      "  psrad $31, %%mm1\n" \
      "  pand %%mm7, %%mm1\n" \
      "  psrld $1, %%mm0\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%mm0\n" \
      "  movq %%mm0, " #x "(%2,%0,1)\n"

      TWIST2(0, -7264)
      TWIST2(8, -7264)
      TWIST2(16, -7264)
      TWIST2(24, -7264)

      "  mov $0x9d2c5680, %0\n"
      "  movd %0, %%mm5\n"
      "  punpckldq %%mm5, %%mm5\n"
      "  mov $0xefc60000, %0\n"
      "  movd %0, %%mm6\n"
      "  punpckldq %%mm6, %%mm6\n"
      "  mov $0, %0\n"
      "1:\n"
#define TEMPER(x) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  psrld $11, %%mm1\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  pslld $7, %%mm1\n" \
      "  pand %%mm5, %%mm1\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  pslld $15, %%mm1\n" \
      "  pand %%mm6, %%mm1\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  psrld $18, %%mm1\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  movq %%mm0, " #x "(%1,%0,1)\n"

      TEMPER(0)
      TEMPER(8)
      TEMPER(16)
      TEMPER(24)

      "  add $32, %0\n"
      "  cmp $19968, %0\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (i), "+r" (d), "+r" (mt)
      );
 
#undef TWIST
#undef TWIST2
#undef TEMPER
}
OIL_DEFINE_IMPL_FULL (mt19937x8_mmx, mt19937x8, OIL_IMPL_FLAG_MMX);

static void
mt19937x8_mmx_2 (uint32_t *d, uint32_t *mt)
{
  long i = 0;
  asm volatile (
      "  mov $0x80000000, %0\n"
      "  movd %0, %%mm5\n"
      "  punpckldq %%mm5, %%mm5\n"
      "  mov $0x7fffffff, %0\n"
      "  movd %0, %%mm6\n"
      "  punpckldq %%mm6, %%mm6\n"
      "  mov $0x9908b0df, %0\n"
      "  movd %0, %%mm7\n"
      "  punpckldq %%mm7, %%mm7\n"
      "  mov $0, %0\n"
      "1:\n"
#define TWIST(x,y) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "  movq 32+" #x "(%2,%0,1), %%mm1\n" \
      "   movq 8+" #x "(%2,%0,1), %%mm2\n" \
      "  pand %%mm5, %%mm0\n" \
      "   movq 40+" #x "(%2,%0,1), %%mm3\n" \
      "  pand %%mm6, %%mm1\n" \
      "   pand %%mm5, %%mm2\n" \
      "  por %%mm1, %%mm0\n" \
      "   pand %%mm6, %%mm3\n" \
      "  movq %%mm0, %%mm1\n" \
      "   por %%mm3, %%mm2\n" \
      "  pslld $31, %%mm1\n" \
      "   movq %%mm2, %%mm3\n" \
      "  psrad $31, %%mm1\n" \
      "   pslld $31, %%mm3\n" \
      "  pand %%mm7, %%mm1\n" \
      "   psrad $31, %%mm3\n" \
      "  psrld $1, %%mm0\n" \
      "   pand %%mm7, %%mm3\n" \
      "  pxor %%mm1, %%mm0\n" \
      "   psrld $1, %%mm2\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%mm0\n" \
      "   pxor %%mm3, %%mm2\n" \
      "  movq %%mm0, " #x "(%2,%0,1)\n" \
      "   pxor 8+" #y "+" #x "(%2,%0,1), %%mm2\n" \
      "   movq %%mm2, 8+" #x "(%2,%0,1)\n"

      TWIST(0, 12704)
      //TWIST(8, 12704)
      TWIST(16, 12704)
      //TWIST(24, 12704)

      "  add $32, %0\n"
      "  cmp $7264, %0\n"
      "  jl 1b\n"

      "2:\n"

      TWIST(0, -7264)
      //TWIST(8, -7264)
      TWIST(16, -7264)
      //TWIST(24, -7264)

      "  add $32, %0\n"
      "  cmp $19936, %0\n"
      "  jl 2b\n"

#define TWIST2(x,y) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "  movq " #x "(%2), %%mm1\n" \
      "  pand %%mm5, %%mm0\n" \
      "  pand %%mm6, %%mm1\n" \
      "  por %%mm1, %%mm0\n" \
      "  movq %%mm0, %%mm1\n" \
      "  pslld $31, %%mm1\n" \
      "  psrad $31, %%mm1\n" \
      "  pand %%mm7, %%mm1\n" \
      "  psrld $1, %%mm0\n" \
      "  pxor %%mm1, %%mm0\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%mm0\n" \
      "  movq %%mm0, " #x "(%2,%0,1)\n"

      TWIST2(0, -7264)
      TWIST2(8, -7264)
      TWIST2(16, -7264)
      TWIST2(24, -7264)

      "  mov $0x9d2c5680, %0\n"
      "  movd %0, %%mm5\n"
      "  punpckldq %%mm5, %%mm5\n"
      "  mov $0xefc60000, %0\n"
      "  movd %0, %%mm6\n"
      "  punpckldq %%mm6, %%mm6\n"
      "  mov $0, %0\n"
      "1:\n"
#define TEMPER(x) \
      "  movq " #x "(%2,%0,1), %%mm0\n" \
      "   movq 8+" #x "(%2,%0,1), %%mm2\n" \
      "  movq %%mm0, %%mm1\n" \
      "   movq %%mm2, %%mm3\n" \
      "  psrld $11, %%mm1\n" \
      "   psrld $11, %%mm3\n" \
      "  pxor %%mm1, %%mm0\n" \
      "   pxor %%mm3, %%mm2\n" \
      "  movq %%mm0, %%mm1\n" \
      "   movq %%mm2, %%mm3\n" \
      "  pslld $7, %%mm1\n" \
      "   pslld $7, %%mm3\n" \
      "  pand %%mm5, %%mm1\n" \
      "   pand %%mm5, %%mm3\n" \
      "  pxor %%mm1, %%mm0\n" \
      "   pxor %%mm3, %%mm2\n" \
      "  movq %%mm0, %%mm1\n" \
      "   movq %%mm2, %%mm3\n" \
      "  pslld $15, %%mm1\n" \
      "   pslld $15, %%mm3\n" \
      "  pand %%mm6, %%mm1\n" \
      "   pand %%mm6, %%mm3\n" \
      "  pxor %%mm1, %%mm0\n" \
      "   pxor %%mm3, %%mm2\n" \
      "  movq %%mm0, %%mm1\n" \
      "   movq %%mm2, %%mm3\n" \
      "  psrld $18, %%mm1\n" \
      "   psrld $18, %%mm3\n" \
      "  pxor %%mm1, %%mm0\n" \
      "   pxor %%mm3, %%mm2\n" \
      "  movq %%mm0, " #x "(%1,%0,1)\n" \
      "   movq %%mm2, 8+" #x "(%1,%0,1)\n"

      TEMPER(0)
      //TEMPER(8)
      TEMPER(16)
      //TEMPER(24)

      "  add $32, %0\n"
      "  cmp $19968, %0\n"
      "  jl 1b\n"
      "  emms\n"
      : "+r" (i), "+r" (d), "+r" (mt)
      );
 
#undef TWIST
#undef TWIST2
#undef TEMPER
}
OIL_DEFINE_IMPL_FULL (mt19937x8_mmx_2, mt19937x8, OIL_IMPL_FLAG_MMX);

static void
mt19937x8_sse (uint32_t *d, uint32_t *mt)
{
  long i = 0;
  asm volatile (
      "  mov $0x80000000, %0\n"
      "  movd %0, %%xmm5\n"
      "  punpckldq %%xmm5, %%xmm5\n"
      "  punpcklqdq %%xmm5, %%xmm5\n"
      "  mov $0x7fffffff, %0\n"
      "  movd %0, %%xmm6\n"
      "  punpckldq %%xmm6, %%xmm6\n"
      "  punpcklqdq %%xmm6, %%xmm6\n"
      "  mov $0x9908b0df, %0\n"
      "  movd %0, %%xmm7\n"
      "  punpckldq %%xmm7, %%xmm7\n"
      "  punpcklqdq %%xmm7, %%xmm7\n"
      "  mov $0, %0\n"
      "1:\n"
#define TWIST(x,y) \
      "  movq " #x "(%2,%0,1), %%xmm0\n" \
      "  movq 32+" #x "(%2,%0,1), %%xmm1\n" \
      "   movq 16+" #x "(%2,%0,1), %%xmm2\n" \
      "  pand %%xmm5, %%xmm0\n" \
      "   movq 48+" #x "(%2,%0,1), %%xmm3\n" \
      "  pand %%xmm6, %%xmm1\n" \
      "   pand %%xmm5, %%xmm2\n" \
      "  por %%xmm1, %%xmm0\n" \
      "   pand %%xmm6, %%xmm3\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "   por %%xmm3, %%xmm2\n" \
      "  pslld $31, %%xmm1\n" \
      "   movq %%xmm2, %%xmm3\n" \
      "  psrad $31, %%xmm1\n" \
      "   pslld $31, %%xmm3\n" \
      "  pand %%xmm7, %%xmm1\n" \
      "   psrad $31, %%xmm3\n" \
      "  psrld $1, %%xmm0\n" \
      "   pand %%xmm7, %%xmm3\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "   psrld $1, %%xmm2\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%xmm0\n" \
      "   pxor %%xmm3, %%xmm2\n" \
      "  movq %%xmm0, " #x "(%2,%0,1)\n" \
      "   pxor 16+" #y "+" #x "(%2,%0,1), %%xmm2\n" \
      "   movq %%xmm2, 16+" #x "(%2,%0,1)\n"

      TWIST(0, 12704)
      //TWIST(8, 12704)
      //TWIST(16, 12704)
      //TWIST(24, 12704)

      "  add $32, %0\n"
      "  cmp $7264, %0\n"
      "  jl 1b\n"

      "2:\n"

      TWIST(0, -7264)
      //TWIST(8, -7264)
      //TWIST(16, -7264)
      //TWIST(24, -7264)

      "  add $32, %0\n"
      "  cmp $19936, %0\n"
      "  jl 2b\n"

#define TWIST2(x,y) \
      "  movq " #x "(%2,%0,1), %%xmm0\n" \
      "  movq " #x "(%2), %%xmm1\n" \
      "  pand %%xmm5, %%xmm0\n" \
      "  pand %%xmm6, %%xmm1\n" \
      "  por %%xmm1, %%xmm0\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "  pslld $31, %%xmm1\n" \
      "  psrad $31, %%xmm1\n" \
      "  pand %%xmm7, %%xmm1\n" \
      "  psrld $1, %%xmm0\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "  pxor " #y "+" #x "(%2,%0,1), %%xmm0\n" \
      "  movq %%xmm0, " #x "(%2,%0,1)\n"

      TWIST2(0, -7264)
      //TWIST2(8, -7264)
      TWIST2(16, -7264)
      //TWIST2(24, -7264)

      "  mov $0x9d2c5680, %0\n"
      "  movd %0, %%xmm5\n"
      "  punpckldq %%xmm5, %%xmm5\n"
      "  punpcklqdq %%xmm5, %%xmm5\n"
      "  mov $0xefc60000, %0\n"
      "  movd %0, %%xmm6\n"
      "  punpckldq %%xmm6, %%xmm6\n"
      "  punpcklqdq %%xmm6, %%xmm6\n"
      "  mov $0, %0\n"
      "1:\n"
#define TEMPER(x) \
      "  movq " #x "(%2,%0,1), %%xmm0\n" \
      "   movq 16+" #x "(%2,%0,1), %%xmm2\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "   movq %%xmm2, %%xmm3\n" \
      "  psrld $11, %%xmm1\n" \
      "   psrld $11, %%xmm3\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "   pxor %%xmm3, %%xmm2\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "   movq %%xmm2, %%xmm3\n" \
      "  pslld $7, %%xmm1\n" \
      "   pslld $7, %%xmm3\n" \
      "  pand %%xmm5, %%xmm1\n" \
      "   pand %%xmm5, %%xmm3\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "   pxor %%xmm3, %%xmm2\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "   movq %%xmm2, %%xmm3\n" \
      "  pslld $15, %%xmm1\n" \
      "   pslld $15, %%xmm3\n" \
      "  pand %%xmm6, %%xmm1\n" \
      "   pand %%xmm6, %%xmm3\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "   pxor %%xmm3, %%xmm2\n" \
      "  movq %%xmm0, %%xmm1\n" \
      "   movq %%xmm2, %%xmm3\n" \
      "  psrld $18, %%xmm1\n" \
      "   psrld $18, %%xmm3\n" \
      "  pxor %%xmm1, %%xmm0\n" \
      "   pxor %%xmm3, %%xmm2\n" \
      "  movq %%xmm0, " #x "(%1,%0,1)\n" \
      "   movq %%xmm2, 16+" #x "(%1,%0,1)\n"

      TEMPER(0)
      //TEMPER(8)
      //TEMPER(16)
      //TEMPER(24)

      "  add $32, %0\n"
      "  cmp $19968, %0\n"
      "  jl 1b\n"
      : "+R" (i), "+r" (d), "+r" (mt)
      );
 
}
OIL_DEFINE_IMPL_FULL (mt19937x8_sse, mt19937x8, OIL_IMPL_FLAG_SSE2);
#endif

