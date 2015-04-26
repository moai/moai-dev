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

#include <liboil/liboilfunction.h>
#include <math.h>

OIL_DECLARE_CLASS(trans8x8_u16);

/* this could use additional work. */
static void
trans8x8_u16_mmx (uint16_t *dest, int dstr, uint16_t *src, int sstr)
{
  asm volatile (
      "  leal (%3,%3,2),%%eax \n"         // UBER 0:
      "  movq (%1), %%mm0 \n"             // UBER 1:
      "  movq (%1,%3,2), %%mm2 \n"        // UBER 2: 
      "  movq %%mm0, %%mm4 \n"            // UBER 3: 1
      "  movq %%mm2, %%mm5 \n"            // UBER 4: 2
      "  punpcklwd (%1,%3), %%mm0 \n"     // UBER 5: 1
      "  punpcklwd (%1,%%eax), %%mm2 \n"  // UBER 6: 0 2
      "  punpckhwd (%1,%3), %%mm4 \n"     // UBER 7: 3
      "  punpckhwd (%1,%%eax), %%mm5 \n"  // UBER 8: 4
      "  movq %%mm0, %%mm1 \n"            // UBER 9: 5
      "  movq %%mm4, %%mm3 \n"            // UBER 10: 7
      "  punpckldq %%mm2, %%mm0 \n"       // UBER 11: 5 6
      "  punpckldq %%mm5, %%mm4 \n"       // UBER 12: 7 8
      "  punpckhdq %%mm2, %%mm1 \n"       // UBER 13: 6 9
      "  punpckhdq %%mm5, %%mm3 \n"       // UBER 14: 9 10
      "  leal (%2,%2,2),%%eax \n"         // UBER 15: 8
      "  movq %%mm0, 0(%0) \n"            // UBER 16: 11
      "  movq %%mm1, (%0,%2) \n"          // UBER 17: 13
      "  movq %%mm4, (%0,%2,2) \n"        // UBER 18: 12
      "  movq %%mm3, (%0,%%eax) \n"       // UBER 19: 14 15

      "  leal (%3,%3,2),%%eax \n"
      "  movq 8(%1), %%mm0 \n"
      "  movq 8(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 8(%1,%3), %%mm0 \n"
      "  punpcklwd 8(%1,%%eax), %%mm2 \n"
      "  punpckhwd 8(%1,%3), %%mm4 \n"
      "  punpckhwd 8(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  leal (%0,%2,4),%0 \n"
      "  movq %%mm0, 0(%0) \n"
      "  movq %%mm1, (%0,%2) \n"
      "  movq %%mm4, (%0,%2,2) \n"
      "  movq %%mm3, (%0,%%eax) \n"

      "  leal (%1,%3,4),%1 \n"
      "  leal (%3,%3,2),%%eax \n"
      "  movq 0(%1), %%mm0 \n"
      "  movq 0(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 0(%1,%3), %%mm0 \n"
      "  punpcklwd 0(%1,%%eax), %%mm2 \n"
      "  punpckhwd 0(%1,%3), %%mm4 \n"
      "  punpckhwd 0(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  neg %2 \n"
      "  leal (%0,%2,4),%0 \n"
      "  neg %2 \n"
      "  movq %%mm0, 8(%0) \n"
      "  movq %%mm1, 8(%0,%2) \n"
      "  movq %%mm4, 8(%0,%2,2) \n"
      "  movq %%mm3, 8(%0,%%eax) \n"

      "  leal (%3,%3,2),%%eax \n"
      "  movq 8(%1), %%mm0 \n"
      "  movq 8(%1,%3,2), %%mm2 \n"
      "  movq %%mm0, %%mm4 \n"
      "  movq %%mm2, %%mm5 \n"
      "  punpcklwd 8(%1,%3), %%mm0 \n"
      "  punpcklwd 8(%1,%%eax), %%mm2 \n"
      "  punpckhwd 8(%1,%3), %%mm4 \n"
      "  punpckhwd 8(%1,%%eax), %%mm5 \n"
      "  movq %%mm0, %%mm1 \n"
      "  movq %%mm4, %%mm3 \n"
      "  punpckldq %%mm2, %%mm0 \n"
      "  punpckldq %%mm5, %%mm4 \n"
      "  punpckhdq %%mm2, %%mm1 \n"
      "  punpckhdq %%mm5, %%mm3 \n"
      "  leal (%2,%2,2),%%eax \n"
      "  leal (%0,%2,4),%0 \n"
      "  movq %%mm0, 8(%0) \n"
      "  movq %%mm1, 8(%0,%2) \n"
      "  movq %%mm4, 8(%0,%2,2) \n"
      "  movq %%mm3, 8(%0,%%eax) \n"
      "  emms \n"
      : "+r" (dest), "+r" (src), "+r" (dstr), "+r" (sstr)
      :
      : "eax");

}
OIL_DEFINE_IMPL_FULL (trans8x8_u16_mmx, trans8x8_u16, OIL_IMPL_FLAG_MMX);

static void
trans8x8_u16_asm1 (uint16_t *dest, int dstr, uint16_t *src, int sstr)
{
  int saved_ebx = 0;
  asm (
      "  movl %%ebx, %4 \n"
      "  movl %0, %%ecx \n"
      "  movl %2, %%ebx \n"
      "  movl %1, %%edx \n"
      "  lea (%%ecx,%%edx,8), %%esi \n"
      "  sub %%edx, %%esi\n "
      "  movl $7, %%edi \n"
      "1: \n"

      "  mov (%%ebx), %%ax \n"
      "  mov %%ax,(%%ecx) \n"
      "  mov 2(%%ebx), %%ax \n"
      "  mov %%ax,(%%ecx,%%edx,1) \n"
      "  mov 4(%%ebx), %%ax \n"
      "  mov %%ax,(%%ecx,%%edx,2) \n"
      "  mov 8(%%ebx), %%ax \n"
      "  mov %%ax,(%%ecx,%%edx,4) \n"

      "  neg %%edx \n"

      "  mov 6(%%ebx), %%ax \n"
      "  mov %%ax,(%%esi,%%edx,4) \n"
      "  mov 10(%%ebx), %%ax \n"
      "  mov %%ax,(%%esi,%%edx,2) \n"
      "  mov 12(%%ebx), %%ax \n"
      "  mov %%ax,(%%esi,%%edx,1) \n"
      "  mov 14(%%ebx), %%ax \n"
      "  mov %%ax,(%%esi) \n"

      "  neg %%edx \n"
      "  add %3, %%ebx \n"
      "  add $2, %%ecx \n"
      "  add $2, %%esi \n"

      "  dec %%edi \n"
      "  jge 1b \n"
      "  movl %4, %%ebx \n"
      :
      : "m" (dest), "m" (dstr), "m" (src), "m" (sstr), "m" (saved_ebx)
      : "eax", "ecx", "edx", "esi", "edi");
}
OIL_DEFINE_IMPL (trans8x8_u16_asm1, trans8x8_u16);

static void
trans8x8_u16_asm2 (uint16_t *dest, int dstr, uint16_t *src, int sstr)
{
  int i;
  int saved_ebx = 0;
  asm (
      "  movl %%ebx, %5 \n"
      "  movl %0, %%ecx \n"
      "  movl %2, %%ebx \n"
      "  movl %1, %%edx \n"
      "  lea (%%ecx,%%edx,8), %%esi \n"
      "  sub %%edx, %%esi\n "
      "  movl $7, %4 \n"
      "  movl %%edx, %%edi \n"
      "  negl %%edi \n"
      "1: \n"

      "  movl (%%ebx), %%eax \n"
      "  mov %%ax,(%%ecx) \n"
      "  shr $16, %%eax \n"
      "  mov %%ax,(%%ecx,%%edx,1) \n"

      "  movl 4(%%ebx), %%eax \n"
      "  mov %%ax,(%%ecx,%%edx,2) \n"
      "  shr $16, %%eax \n"
      "  mov %%ax,(%%esi,%%edi,4) \n"

      "  movl 8(%%ebx), %%eax \n"
      "  mov %%ax,(%%ecx,%%edx,4) \n"
      "  shr $16, %%eax \n"
      "  mov %%ax,(%%esi,%%edi,2) \n"

      "  movl 12(%%ebx), %%eax \n"
      "  mov %%ax,(%%esi,%%edi,1) \n"
      "  shr $16, %%eax \n"
      "  mov %%ax,(%%esi) \n"

      "  add %3, %%ebx \n"
      "  add $2, %%ecx \n"
      "  add $2, %%esi \n"

      "  decl %4 \n"
      "  jge 1b \n"
      "  movl %5, %%ebx \n"
      :
      : "m" (dest), "m" (dstr), "m" (src), "m" (sstr), "m" (i), "m" (saved_ebx)
      : "eax", "ecx", "edx", "esi", "edi");
}
OIL_DEFINE_IMPL (trans8x8_u16_asm2, trans8x8_u16);

