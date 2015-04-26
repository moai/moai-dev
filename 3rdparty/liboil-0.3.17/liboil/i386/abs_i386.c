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
#include <liboil/simdpack/simdpack.h>

#define ABS(x) ((x)>0 ? (x) : -(x))

#if 0
static void
abs_u16_s16_i386asm (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:  movswl	(%0), %%eax		\n"
      "    addl	$2, %0			\n"
      "    movl	%%eax, %%edx		\n"
      "    negl	%%edx			\n"
      "    cmpl	$-1, %%eax		\n"
      "    cmovle	%%edx, %%eax		\n"
      "    movw	%%ax, (%1)		\n"
      "    addl	$2, %1			\n"
      "    decl	%2			\n"
      "    testl	%2, %2			\n"
      "    jg	1b			\n":"+r" (src), "+r" (dest), "+r" (n)
      ::"eax", "edx");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_i386asm, abs_u16_s16, OIL_IMPL_FLAG_CMOV);
#endif

#if 0
/* The previous function after running through uberopt */
static void
abs_u16_s16_i386asm_uber4 (uint16_t * dest, int dstr, int16_t * src,
    int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:                               \n"
      "    movswl	(%0), %%eax	\n" /* UBER 0:     */
      "    addl	$2, %0			\n" /* UBER 1: 0   */
      "    movl	%%eax, %%edx		\n" /* UBER 2: 0   */
      "    decl	%2			\n" /* UBER 7:     */
      "    negl	%%edx			\n" /* UBER 3: 2   */
      "    cmpl	$-1, %%eax ; cmovle %%edx, %%eax \n" /* UBER 4: 3 */
      "    movw	%%ax, (%1)		\n" /* UBER 5: 4   */
      "    addl	$2, %1			\n" /* UBER 6: 5   */
      "    testl	%2, %2		\n"
      "    jg	1b			\n"
      :"+r" (src), "+r" (dest), "+r" (n)
      ::"eax", "edx");
}
OIL_DEFINE_IMPL_FULL (abs_u16_s16_i386asm_uber4, abs_u16_s16, OIL_IMPL_FLAG_CMOV);
#endif

#if 0
static void
abs_u16_s16_i386asm2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movswl	(%%edi), %%eax		\n"
      "	addl	$2, %%edi		\n"
      "	movl	%%eax, %%edx		\n"
      "	negl	%%edx			\n"
      "	cmpl	$-1, %%eax		\n"
      "	cmovle	%%edx, %%eax		\n"
      "	movw	%%ax, (%%ebp)		\n"
      "	addl	$2, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ecx", "edx");
}
OIL_DEFINE_IMPL_FULL (abs_u16_s16_i386asm2, abs_u16_s16, OIL_IMPL_FLAG_CMOV);
#endif

static void
abs_u16_s16_i386asm3 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  __asm__ __volatile__ ("\n"
      "	.p2align 4,,15			\n"
      "1:  movswl (%1), %%eax           \n"
      "    add %3, %1                   \n"
      "    mov %%eax, %%edx             \n"
      "    sar $0xf, %%ax               \n"
      "    and %%edx, %%eax             \n"
      "    add %%eax, %%eax             \n"
      "    sub %%eax, %%edx             \n"
      "    mov %%dx, (%0)               \n"
      "    add %4, %0                   \n"
      "    decl %2                      \n"
      "    jne 1b                       \n"
      : "+r" (dest), "+r" (src), "+m" (n)
      : "m" (dstr), "m" (sstr)
      : "eax", "edx");
}
OIL_DEFINE_IMPL_ASM (abs_u16_s16_i386asm3, abs_u16_s16);



static void
abs_u16_s16_mmx (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  static const int16_t p[][4] = {
    { -32768, -32768, -32768, -32768 },
    { 32767, 32767, 32767, 32767 }
  };
  int16_t tmp[4];

  while (n & 3) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 4;
  __asm__ __volatile__ ("\n"
      "	movq	(%0), %%mm2		\n"
      "	movq	8(%0), %%mm3		\n"
      :: "r" (p));
  while (n--) {
    tmp[0] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[1] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[2] = *src;
    OIL_INCREMENT (src, sstr);
    tmp[3] = *src;
    OIL_INCREMENT (src, sstr);
    __asm__ __volatile__ ("\n"
        "	movq	(%0), %%mm1		\n"
        "	movq	%%mm1, %%mm0		\n"
        "	paddsw	%%mm2, %%mm0		\n"
        "	paddsw	%%mm3, %%mm1		\n"
        "	psubsw	%%mm2, %%mm0		\n"
        "	psubsw	%%mm3, %%mm1		\n"
        "	psubw	%%mm1, %%mm0		\n"
        "	movq	%%mm0, (%0)		\n"
        : : "r" (tmp)
        : "memory" );
    *dest = tmp[0];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[1];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[2];
    OIL_INCREMENT (dest, dstr);
    *dest = tmp[3];
    OIL_INCREMENT (dest, dstr);
  }
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmx, abs_u16_s16, OIL_IMPL_FLAG_MMX);

#if 0
static void
abs_u16_s16_mmxx (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  short p[] = { -32768, -32768, -32768, -32768,
    32767, 32767, 32767, 32767
  };

  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	movq	(%3), %%mm2		\n"
      "	movq	8(%3), %%mm3		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%mm0		\n"
      "	movq	(%%edi), %%mm1		\n"
      "	paddsw	%%mm2, %%mm0		\n"
      "	paddsw	%%mm3, %%mm1		\n"
      "	psubsw	%%mm2, %%mm0		\n"
      "	psubsw	%%mm3, %%mm1		\n"
      "	psubw	%%mm1, %%mm0		\n"
      "	movq	%%mm0, (%%eax)		\n"
      "	 movq	8(%%edi), %%mm4		\n"
      "	 movq	8(%%edi), %%mm5		\n"
      "	 addl	$16, %%edi		\n"
      "	 paddsw	%%mm2, %%mm4		\n"
      "	 paddsw	%%mm3, %%mm5		\n"
      "	 psubsw	%%mm2, %%mm4		\n"
      "	 psubsw	%%mm3, %%mm5		\n"
      "	 psubw	%%mm5, %%mm4		\n"
      "	 movq	%%mm4, 8(%%eax)		\n"
      "	 addl	$16, %%eax		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n":"+D" (src), "+a" (dest), "+S" (n)
      :"c" (p));
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmxx, abs_u16_s16, OIL_IMPL_FLAG_MMX);
#endif

#ifdef ENABLE_BROKEN_IMPLS
static void
abs_u16_s16_mmx2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%mm0		\n"
      "	pxor	%%mm1, %%mm1		\n"
      "	 movq	8(%%edi), %%mm2		\n"
      "	 addl	$16, %%edi		\n"
      "	psubw	%%mm0, %%mm1		\n"
      "	 pxor	%%mm3, %%mm3		\n"
      "	pmaxsw	%%mm0, %%mm1		\n"
      "	 psubw	%%mm2, %%mm3		\n"
      "	movq	%%mm1, (%%ebp)		\n"
      "	 pmaxsw	%%mm2, %%mm3		\n"
      "	 movq	%%mm3, 8(%%ebp)		\n"
      "	 addl	$16, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ecx", "edx");
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (abs_u16_s16_mmx2, abs_u16_s16, OIL_IMPL_FLAG_MMXEXT);
#endif

#ifdef ENABLE_BROKEN_IMPLS
static void
abs_u16_s16_sse2 (uint16_t * dest, int dstr, int16_t * src, int sstr, int n)
{
  while (n & 7) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
    n--;
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	pushl	%%ebp			\n"
      "	movl	%%eax, %%ebp		\n"
      "	.p2align 4,,15			\n"
      "1:	movq	(%%edi), %%xmm0		\n"
      "	addl	$16, %%edi		\n"
      "	pxor	%%xmm1, %%xmm1		\n"
      "	psubw	%%xmm0, %%xmm1		\n"
      "	pmaxsw	%%xmm0, %%xmm1		\n"
      "	movq	%%xmm1, (%%ebp)		\n"
      "	addl	$16, %%ebp		\n"
      "	decl	%2			\n"
      "	testl	%2, %2			\n"
      "	jg	1b			\n"
      "	popl	%%ebp			\n":"+D" (src), "+a" (dest), "+S" (n)
      ::"ecx", "edx");
}
OIL_DEFINE_IMPL_FULL (abs_u16_s16_sse2, abs_u16_s16, OIL_IMPL_FLAG_SSE2);
#endif

