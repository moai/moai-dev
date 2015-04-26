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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/dct/dct.h>
#include <math.h>


#ifdef ENABLE_BROKEN_IMPLS

#define CONST(x) (32768.0*(x) + 0.5)

#define C1_0000 (32767)
#define C0_9808 CONST(0.980785280)
#define C0_9239 CONST(0.923879532)
#define C0_8315 CONST(0.831469612)
#define C0_7071 CONST(0.707106781)
#define C0_5556 CONST(0.555570233)
#define C0_3827 CONST(0.382683432)
#define C0_1951 CONST(0.195090322)

#define FOUR(x) { x, x, x, x }
#define MMX_CONST(x) {32768.0*(x) + 0.5,32768.0*(x) + 0.5,32768.0*(x) + 0.5,32768.0*(x) + 0.5}

static const int16_t
dct_mmx_constants [][4] = {
  FOUR(0),
  FOUR(C0_9808),
  FOUR(C0_9239),
  FOUR(C0_8315),
  FOUR(C0_7071),
  FOUR(C0_5556),
  FOUR(C0_3827),
  FOUR(C0_1951),
  { 1, 1, -1, -1 }, // 64
  { 1, -1, 1, -1 },
  { C1_0000, C0_9239, C0_7071, C0_3827 }, // 80
  { C1_0000, C0_3827, C0_7071, C0_9239 }, // 88
  { C0_9808, C0_8315, C0_5556, C0_1951 }, // 96
  { C0_8315, C0_1951, C0_9808, C0_5556 }, // 104
  { 1, -1, -1, -1 },
  { C0_5556, C0_9808, C0_1951, C0_8315 }, // 120
  { 1, -1, 1, 1 },
  { C0_1951, C0_5556, C0_8315, C0_9808 }, // 136
  { 1, -1, 1, -1 },
  FOUR(CONST(0.5)), //152
  { C0_7071, C0_9239, C0_7071, C0_3827 }, // 160
  { C0_7071, C0_3827, C0_7071, C0_9239 }, // 168
};

static void
idct8x8_s16_mmx (int16_t *dest, int dstr, int16_t *src, int sstr)
{
  int32_t tmp[32];
  int32_t save_ebx;

  asm volatile (
      "  movl %%ebx, %6 \n"
      /* left half */
      "  movl %1, %%eax \n" // src
      "  movl %3, %%ebx \n" // sstr
      "  leal (%%eax,%%ebx,4),%%ecx \n" // src + sstr * 4
      
      "  movq (%%eax), %%mm0 \n"
      "  movq (%%eax), %%mm1 \n"
      "  paddsw (%%ecx), %%mm0 \n"  // ss07s34
      "  psubsw (%%ecx), %%mm1 \n"  // ss16s25
      "  pmulhw 32(%5), %%mm0 \n" // .7071
      "  pmulhw 32(%5), %%mm1 \n" // .7071

      "  movq (%%eax,%%ebx,2), %%mm2 \n"
      "  movq (%%eax,%%ebx,2), %%mm3 \n"
      "  movq (%%ecx,%%ebx,2), %%mm4 \n"
      "  movq (%%ecx,%%ebx,2), %%mm5 \n"
      "  pmulhw 16(%5), %%mm2 \n" // .9239
      "  pmulhw 48(%5), %%mm3 \n" // .3827
      "  pmulhw 48(%5), %%mm4 \n" // .3827
      "  pmulhw 16(%5), %%mm5 \n" // .9239 
      "  paddsw %%mm4, %%mm2 \n" // ds07s34
      "  psubsw %%mm5, %%mm3 \n" // ds16s25

      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm1, %%mm5 \n"
      "  paddsw %%mm2, %%mm0 \n" // s07
      "  psubsw %%mm2, %%mm4 \n" // s34
      "  paddsw %%mm3, %%mm1 \n" // s16
      "  psubsw %%mm3, %%mm5 \n" // s25

      "  movq %%mm0, 0(%4) \n"
      "  movq %%mm1, 8(%4) \n"
      "  movq %%mm5, 16(%4) \n"
      "  movq %%mm4, 24(%4) \n"

      "  addl %3, %%eax \n"
      "  addl %3, %%ecx \n"

      "  movq (%%eax), %%mm0 \n"
      "  pmulhw 8(%5), %%mm0 \n"
      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq (%%ecx), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq (%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n" // d07

      "  movq (%%eax), %%mm2 \n"
      "  pmulhw 24(%5), %%mm2 \n"
      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n"
      "  movq (%%ecx), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n"
      "  movq (%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n" // d16

      "  movq (%%eax), %%mm3 \n"
      "  pmulhw 40(%5), %%mm3 \n"
      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm3 \n"
      "  movq (%%ecx), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm3 \n"
      "  movq (%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm3 \n" // d25

      "  movq (%%eax), %%mm4 \n"
      "  pmulhw 56(%5), %%mm4 \n"
      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm4 \n"
      "  movq (%%ecx), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm4 \n"
      "  movq (%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm4 \n" // d34

      "  movl %0, %%eax \n" // dest
      "  movl %2, %%ebx \n" // dstr
      "  leal (%%ebx, %%ebx, 2), %%edx \n" // dstr*3

      "  movq %%mm0, %%mm1 \n"
      "  paddsw 0(%4), %%mm1 \n"
      "  movq %%mm1, (%%eax) \n"

      "  movq %%mm2, %%mm1 \n"
      "  paddsw 8(%4), %%mm1 \n"
      "  movq %%mm1, (%%eax, %%ebx, 1) \n"

      "  movq %%mm3, %%mm1 \n"
      "  paddsw 16(%4), %%mm1 \n"
      "  movq %%mm1, (%%eax, %%ebx, 2) \n" // s25 + d25

      "  movq %%mm4, %%mm1 \n"
      "  paddsw 24(%4), %%mm1 \n"
      "  movq %%mm1, (%%eax, %%edx, 1) \n"

      "  leal (%%eax, %%ebx, 4), %%eax \n"
      "  movq 24(%4), %%mm1 \n"
      "  psubsw %%mm4, %%mm1 \n"
      "  movq %%mm1, (%%eax) \n"

      "  movq 16(%4), %%mm1 \n"
      "  psubsw %%mm3, %%mm1 \n"
      "  movq %%mm1, (%%eax, %%ebx, 1) \n"

      "  movq 8(%4), %%mm1 \n"
      "  psubsw %%mm2, %%mm1 \n"
      "  movq %%mm1, (%%eax, %%ebx, 2) \n"

      "  movq 0(%4), %%mm1 \n"
      "  psubsw %%mm0, %%mm1 \n"
      "  movq %%mm1, (%%eax, %%edx, 1) \n"

      /* right half */
      "  movl %1, %%eax \n" // src
      "  movl %3, %%ebx \n" // sstr
      "  leal (%%eax,%%ebx,4),%%ecx \n" // src + sstr * 4
      
      "  movq 8(%%eax), %%mm0 \n"
      "  movq 8(%%eax), %%mm1 \n"
      "  paddsw 8(%%ecx), %%mm0 \n"  // ss07s34
      "  psubsw 8(%%ecx), %%mm1 \n"  // ss16s25
      "  pmulhw 32(%5), %%mm0 \n" // .7071
      "  pmulhw 32(%5), %%mm1 \n" // .7071

      "  movq 8(%%eax,%%ebx,2), %%mm2 \n"
      "  movq 8(%%eax,%%ebx,2), %%mm3 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm4 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm5 \n"
      "  pmulhw 16(%5), %%mm2 \n" // .9239
      "  pmulhw 48(%5), %%mm3 \n" // .3827
      "  pmulhw 48(%5), %%mm4 \n" // .3827
      "  pmulhw 16(%5), %%mm5 \n" // .9239 
      "  paddsw %%mm4, %%mm2 \n" // ds07s34
      "  psubsw %%mm5, %%mm3 \n" // ds16s25

      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm1, %%mm5 \n"
      "  paddsw %%mm2, %%mm0 \n" // s07
      "  psubsw %%mm2, %%mm4 \n" // s34
      "  paddsw %%mm3, %%mm1 \n" // s16
      "  psubsw %%mm3, %%mm5 \n" // s25

      "  movq %%mm0, 0(%4) \n"
      "  movq %%mm1, 8(%4) \n"
      "  movq %%mm5, 16(%4) \n"
      "  movq %%mm4, 24(%4) \n"

      "  addl %3, %%eax \n"
      "  addl %3, %%ecx \n"

      "  movq 8(%%eax), %%mm0 \n"
      "  pmulhw 8(%5), %%mm0 \n"
      "  movq 8(%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 8(%%ecx), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n" // d07

      "  movq 8(%%eax), %%mm2 \n"
      "  pmulhw 24(%5), %%mm2 \n"
      "  movq 8(%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n"
      "  movq 8(%%ecx), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm2 \n" // d16

      "  movq 8(%%eax), %%mm3 \n"
      "  pmulhw 40(%5), %%mm3 \n"
      "  movq 8(%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm3 \n"
      "  movq 8(%%ecx), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm3 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm3 \n" // d25

      "  movq 8(%%eax), %%mm4 \n"
      "  pmulhw 56(%5), %%mm4 \n"
      "  movq 8(%%eax,%%ebx,2), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm4 \n"
      "  movq 8(%%ecx), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm4 \n"
      "  movq 8(%%ecx,%%ebx,2), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm4 \n" // d34

      "  movl %0, %%eax \n" // dest
      "  movl %2, %%ebx \n" // dstr
      "  leal (%%ebx, %%ebx, 2), %%edx \n" // dstr*3

      "  movq %%mm0, %%mm1 \n"
      "  paddsw 0(%4), %%mm1 \n"
      "  movq %%mm1, 8(%%eax) \n"

      "  movq %%mm2, %%mm1 \n"
      "  paddsw 8(%4), %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%ebx, 1) \n"

      "  movq %%mm3, %%mm1 \n"
      "  paddsw 16(%4), %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%ebx, 2) \n" // s25 + d25

      "  movq %%mm4, %%mm1 \n"
      "  paddsw 24(%4), %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%edx, 1) \n"

      "  leal (%%eax, %%ebx, 4), %%eax \n"
      "  movq 24(%4), %%mm1 \n"
      "  psubsw %%mm4, %%mm1 \n"
      "  movq %%mm1, 8(%%eax) \n"

      "  movq 16(%4), %%mm1 \n"
      "  psubsw %%mm3, %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%ebx, 1) \n"

      "  movq 8(%4), %%mm1 \n"
      "  psubsw %%mm2, %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%ebx, 2) \n"

      "  movq 0(%4), %%mm1 \n"
      "  psubsw %%mm0, %%mm1 \n"
      "  movq %%mm1, 8(%%eax, %%edx, 1) \n"


      /* rows */
      "  movl %0, %%eax \n" /* dest */
#define LOOP \
      "  pshufw $0x88, 0(%%eax), %%mm0 \n" /* x0 x2 x0 x2 */ \
      "  pshufw $0x88, 8(%%eax), %%mm1 \n" /* x4 x6 x4 x6 */ \
      "  pmulhw 160(%5), %%mm0 \n" /* 0.707 0.9239 0.707 0.3827 */ \
      "  pmulhw 168(%5), %%mm1 \n" /* 0.707 0.3827 0.707 0.9239 */ \
      "  pmullw 64(%5), %%mm1 \n" /* 1 1 -1 -1 */ \
      "  paddsw %%mm1, %%mm0 \n" /* ss07s34 ds07s34 ss16s25 ds16s25 */ \
 \
      "  pshufw $0xa0, %%mm0, %%mm1 \n" /* ss07s34 ss07s34 ss16s25 ss16s25 */ \
      "  pshufw $0xf5, %%mm0, %%mm2 \n" /* ds07s34 ds07s34 ds16s25 ds16s25 */ \
      "  pmullw 72(%5), %%mm2 \n" /* 1 -1 1 -1 */ \
      "  paddsw %%mm2, %%mm1 \n" /* s07 s34 s16 s25 */ \
      "  pshufw $0x78, %%mm1, %%mm2 \n" /* s07 s16 s25 s34 */ \
 \
      "  pshufw $0x55, 0(%%eax), %%mm0 \n" \
      "  pmulhw 96(%5), %%mm0 \n" \
      "  pshufw $0xff, 0(%%eax), %%mm1 \n" \
      "  pmulhw 104(%5), %%mm1 \n" \
      "  pmullw 112(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm0 \n" \
      "  pshufw $0x55, 8(%%eax), %%mm1 \n" \
      "  pmulhw 120(%5), %%mm1 \n" \
      "  pmullw 128(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm0 \n" \
      "  pshufw $0xff, 8(%%eax), %%mm1 \n" \
      "  pmulhw 136(%5), %%mm1 \n" \
      "  pmullw 144(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm0 \n" \
 \
      "  movq %%mm2, %%mm1 \n" \
      "  paddsw %%mm0, %%mm1 \n" \
      "  psubsw %%mm0, %%mm2 \n" \
      "  pshufw $0x1b, %%mm2, %%mm2 \n" \
 \
      "  movq %%mm1, 0(%%eax) \n" \
      "  movq %%mm2, 8(%%eax) \n" \
      "  addl %3, %%eax \n"

      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
#undef LOOP

      "  movl %6, %%ebx \n"
      "  emms \n"
      :
      : "m" (dest), "m" (src), "m" (dstr), "m" (sstr), "r" (tmp), "r" (dct_mmx_constants), "m" (save_ebx)
      : "eax", "ecx", "edx");
}
OIL_DEFINE_IMPL_FULL (idct8x8_s16_mmx, idct8x8_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

#if 0
#define CONST(x) (32768.0*(x) + 0.5)

#define C1_0000 (32767)
#define C0_9808 CONST(0.980785280)
#define C0_9239 CONST(0.923879532)
#define C0_8315 CONST(0.831469612)
#define C0_7071 CONST(0.707106781)
#define C0_5556 CONST(0.555570233)
#define C0_3827 CONST(0.382683432)
#define C0_1951 CONST(0.195090322)

#define FOUR(x) { x, x, x, x }
#define MMX_CONST(x) {32768.0*(x) + 0.5,32768.0*(x) + 0.5,32768.0*(x) + 0.5,32768.0*(x) + 0.5}

static const int16_t
dct_mmx_constants [][4] = {
  FOUR(0),
  FOUR(C0_9808),
  FOUR(C0_9239),
  FOUR(C0_8315),
  FOUR(C0_7071),
  FOUR(C0_5556),
  FOUR(C0_3827),
  FOUR(C0_1951),
  { 1, 1, -1, -1 }, // 64
  { 1, -1, 1, -1 },
  { C1_0000, C0_9239, C0_7071, C0_3827 }, // 80
  { C1_0000, C0_3827, C0_7071, C0_9239 }, // 88
  { C0_9808, C0_8315, C0_5556, C0_1951 }, // 96
  { C0_8315, C0_1951, C0_9808, C0_5556 }, // 104
  { 1, -1, -1, -1 },
  { C0_5556, C0_9808, C0_1951, C0_8315 }, // 120
  { 1, -1, 1, 1 },
  { C0_1951, C0_5556, C0_8315, C0_9808 }, // 136
  { 1, -1, 1, -1 },
};
#endif

/* a 3dnow version can use pmulhrw instead of pmulhw for increased
 * accuracy */
static void
fdct8x8s_s16_mmx (uint16_t *dest, int dstr, uint16_t *src, int sstr)
{
  int32_t tmp[32];
  int32_t save_ebx;

  asm volatile (
      "  movl %%ebx, %6 \n"
      /* first half */
      "  movl %1, %%eax \n" // src
      "  movl %3, %%ebx \n" // sstr
      "  leal (%%ebx,%%ebx,2),%%edx \n" // sstr * 3
      "  leal (%%eax,%%ebx,4),%%ecx \n" // src + sstr * 4

      "  movq (%%eax), %%mm0 \n"
      "  movq (%%eax), %%mm1 \n"
      "  paddsw (%%ecx,%%edx,1), %%mm0 \n"  // s07
      "  psubsw (%%ecx,%%edx,1), %%mm1 \n"  // d07
      "  movq %%mm1, (%4) \n"

      "  movq (%%eax,%%ebx), %%mm2 \n"
      "  movq (%%eax,%%ebx), %%mm3 \n"
      "  paddsw (%%ecx,%%ebx,2), %%mm2 \n"  // s16
      "  psubsw (%%ecx,%%ebx,2), %%mm3 \n"  // d16
      "  movq %%mm3, 8(%4) \n"

      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  movq (%%eax,%%ebx,2), %%mm4 \n"
      "  paddsw (%%ecx,%%ebx), %%mm1 \n"  // s25
      "  psubsw (%%ecx,%%ebx), %%mm4 \n"  // d25
      "  movq %%mm4, 16(%4) \n"

      "  movq (%%eax,%%edx), %%mm3 \n"
      "  movq (%%eax,%%edx), %%mm5 \n"
      "  paddsw (%%ecx), %%mm3 \n"  // s34
      "  psubsw (%%ecx), %%mm5 \n"  // d34
      "  movq %%mm5, 24(%4) \n"

      "  movq %%mm0, %%mm4 \n"
      "  paddsw %%mm3, %%mm0 \n"  // ss07s34
      "  psubsw %%mm3, %%mm4 \n"  // ds07s34

      "  movq %%mm2, %%mm5 \n"
      "  paddsw %%mm1, %%mm2 \n"  // ss16s25
      "  psubsw %%mm1, %%mm5 \n"  // ds16s25

      "  movq %%mm0, %%mm1 \n"
      "  paddsw %%mm2, %%mm1 \n"
      "  pmulhw 32(%5), %%mm1 \n" // .7071
      "  psubsw %%mm2, %%mm0 \n"
      "  pmulhw 32(%5), %%mm0 \n" // .7071

      "  movq %%mm4, %%mm2 \n"
      "  pmulhw 16(%5), %%mm2 \n"
      "  movq %%mm5, %%mm6 \n"
      "  pmulhw 48(%5), %%mm6 \n"
      "  paddsw %%mm6, %%mm2 \n" // 

      "  pmulhw 48(%5), %%mm4 \n"
      "  pmulhw 16(%5), %%mm5 \n"
      "  psubsw %%mm5, %%mm4 \n" // 

      "  movl %0, %%eax \n" // dest
      "  movl %2, %%ebx \n" // dstr
      "  add %%ebx, %%ebx \n"
      "  leal (%%ebx,%%ebx,2),%%edx \n" // dstr * 3
      "  movq %%mm1, 0(%%eax) \n"
      "  movq %%mm2, 0(%%eax,%%ebx) \n"
      "  movq %%mm0, 0(%%eax,%%ebx,2) \n"
      "  movq %%mm4, 0(%%eax,%%edx) \n"

      "  add %2, %%eax \n"
      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 8(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 24(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%ebx) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 40(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%ebx,2) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 56(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%edx) \n"

      /* second half */

      "  movl %1, %%eax \n" // src
      "  add $8, %%eax \n"
      "  movl %3, %%ebx \n" // sstr
      "  leal (%%ebx,%%ebx,2),%%edx \n" // sstr * 3
      "  leal (%%eax,%%ebx,4),%%ecx \n" // src + sstr * 4

      "  movq (%%eax), %%mm0 \n"
      "  movq (%%eax), %%mm1 \n"
      "  paddsw (%%ecx,%%edx,1), %%mm0 \n"  // s07
      "  psubsw (%%ecx,%%edx,1), %%mm1 \n"  // d07
      "  movq %%mm1, (%4) \n"

      "  movq (%%eax,%%ebx), %%mm2 \n"
      "  movq (%%eax,%%ebx), %%mm3 \n"
      "  paddsw (%%ecx,%%ebx,2), %%mm2 \n"  // s16
      "  psubsw (%%ecx,%%ebx,2), %%mm3 \n"  // d16
      "  movq %%mm3, 8(%4) \n"

      "  movq (%%eax,%%ebx,2), %%mm1 \n"
      "  movq (%%eax,%%ebx,2), %%mm4 \n"
      "  paddsw (%%ecx,%%ebx), %%mm1 \n"  // s25
      "  psubsw (%%ecx,%%ebx), %%mm4 \n"  // d25
      "  movq %%mm4, 16(%4) \n"

      "  movq (%%eax,%%edx), %%mm3 \n"
      "  movq (%%eax,%%edx), %%mm5 \n"
      "  paddsw (%%ecx), %%mm3 \n"  // s34
      "  psubsw (%%ecx), %%mm5 \n"  // d34
      "  movq %%mm5, 24(%4) \n"

      "  movq %%mm0, %%mm4 \n"
      "  paddsw %%mm3, %%mm0 \n"  // ss07s34
      "  psubsw %%mm3, %%mm4 \n"  // ds07s34

      "  movq %%mm2, %%mm5 \n"
      "  paddsw %%mm1, %%mm2 \n"  // ss16s25
      "  psubsw %%mm1, %%mm5 \n"  // ds16s25

      "  movq %%mm0, %%mm1 \n"
      "  paddsw %%mm2, %%mm1 \n"
      "  pmulhw 32(%5), %%mm1 \n" // .7071
      "  psubsw %%mm2, %%mm0 \n"
      "  pmulhw 32(%5), %%mm0 \n" // .7071

      "  movq %%mm4, %%mm2 \n"
      "  pmulhw 16(%5), %%mm2 \n"
      "  movq %%mm5, %%mm6 \n"
      "  pmulhw 48(%5), %%mm6 \n"
      "  paddsw %%mm6, %%mm2 \n" // 

      "  pmulhw 48(%5), %%mm4 \n"
      "  pmulhw 16(%5), %%mm5 \n"
      "  psubsw %%mm5, %%mm4 \n" // 

      "  movl %0, %%eax \n" // dest
      "  add $8, %%eax \n"
      "  movl %2, %%ebx \n" // dstr
      "  add %%ebx, %%ebx \n"
      "  leal (%%ebx,%%ebx,2),%%edx \n" // dstr * 3
      "  movq %%mm1, 0(%%eax) \n"
      "  movq %%mm2, 0(%%eax,%%ebx) \n"
      "  movq %%mm0, 0(%%eax,%%ebx,2) \n"
      "  movq %%mm4, 0(%%eax,%%edx) \n"

      "  add %2, %%eax \n"
      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 8(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 24(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%ebx) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 40(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 56(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%ebx,2) \n"

      "  movq 0(%4), %%mm0 \n"
      "  pmulhw 56(%5), %%mm0 \n"
      "  movq 8(%4), %%mm1 \n"
      "  pmulhw 40(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq 16(%4), %%mm1 \n"
      "  pmulhw 24(%5), %%mm1 \n"
      "  paddsw %%mm1, %%mm0 \n"
      "  movq 24(%4), %%mm1 \n"
      "  pmulhw 8(%5), %%mm1 \n"
      "  psubsw %%mm1, %%mm0 \n"
      "  movq %%mm0, (%%eax,%%edx) \n"

      "  movl %0, %%ecx \n" // dest
      
#define LOOP \
      "  movq (%%ecx), %%mm0 \n" \
      "  pshufw $0x1b, 8(%%ecx), %%mm1 \n" \
      "  movq %%mm0, %%mm2 \n" \
      "  paddsw %%mm1, %%mm0 \n" /* s07 s16 s25 s34 */ \
      "  psubsw %%mm1, %%mm2 \n" /* d07 d16 d25 d34 */ \
 \
      "  pshufw $0xbb, %%mm0, %%mm1 \n" /* s25 s34 s25 s34 */ \
      "  pshufw $0x44, %%mm0, %%mm0 \n" /* s07 s16 s07 s16 */ \
 \
      "  pmullw 64(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm0 \n" /* ss07s34 ss16s25 ds07s34 ds16s25 */ \
 \
      "  pshufw $0x88, %%mm0, %%mm1 \n" /* ss07s34 ds07s34 ss07s34 ds07s34 */ \
      "  pshufw $0xdd, %%mm0, %%mm0 \n" /* ss16s25 ds16s25 ss16s25 ds16s25 */ \
 \
      "  pmulhw 80(%5), %%mm1 \n" \
 \
      "  pmullw 64(%5), %%mm0 \n" \
      "  pmulhw 88(%5), %%mm0 \n" \
 \
      "  paddsw %%mm1, %%mm0 \n" \
 \
      "  pshufw $0x00, %%mm2, %%mm3 \n" \
      "  pmulhw 96(%5), %%mm3 \n" \
      "  pshufw $0x55, %%mm2, %%mm1 \n" \
      "  pmulhw 104(%5), %%mm1 \n" \
      "  pmullw 112(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm3 \n" \
      "  pshufw $0xaa, %%mm2, %%mm1 \n" \
      "  pmulhw 120(%5), %%mm1 \n" \
      "  pmullw 128(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm3 \n" \
      "  pshufw $0xff, %%mm2, %%mm1 \n" \
      "  pmulhw 136(%5), %%mm1 \n" \
      "  pmullw 144(%5), %%mm1 \n" \
      "  paddsw %%mm1, %%mm3 \n" \
 \
      "  movq %%mm0, %%mm1 \n" \
      "  punpckhwd %%mm3, %%mm1 \n" \
      "  punpcklwd %%mm3, %%mm0 \n" \
 \
      "  movq %%mm0, (%%ecx) \n" \
      "  movq %%mm1, 8(%%ecx) \n" \
 \
      "  add %3, %%eax \n" \
      "  add %2, %%ecx \n"

      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP
      LOOP

      "  emms \n"
      "  movl %6, %%ebx \n"
      :
      : "m" (dest), "m" (src), "m" (dstr), "m" (sstr), "r" (tmp), "r" (dct_mmx_constants), "m" (save_ebx)
      : "eax", "ecx", "edx");

}
OIL_DEFINE_IMPL_FULL (fdct8x8s_s16_mmx, fdct8x8s_s16, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif

