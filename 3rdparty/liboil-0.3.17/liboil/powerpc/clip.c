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

#ifdef ENABLE_BROKEN_IMPLS
static void
clip_s16_ppcasm (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	__asm__ __volatile__(
		"	li r7, 0			\n"
		"	mtctr %4		\n"
		"1:	lhax r9,r7,%1		\n"
		"	add r10,r9,%2		\n"
		"	subfc r8,r9,%3		\n"
		"	srawi r0,r10,31		\n"
		"	srawi r11,r8,31		\n"
		"	and r0,r0,r10		\n"
		"	and r11,r11,r8		\n"
		"	subf r9,r0,r9		\n"
		"	add r9,r9,r11		\n"
		"	sthx r9,r7,%0		\n"
		"	addi r7,r7,2		\n"
		"	bdnz 1b			\n"
        :
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "7", "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm, clip_s16);
#endif

#ifdef ENABLE_BROKEN_IMPLS
static void
clip_s16_ppcasm2 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	src--;
	dest--;
	__asm__ __volatile__(
		"	mtctr %4		\n"
		"1:	lhau r9,2(%1)		\n"
		"	add r10,r9,%2		\n"
		"	subfc r8,r9,%3		\n"
		"	srawi r0,r10,31		\n"
		"	srawi r11,r8,31		\n"
		"	and r0,r0,r10		\n"
		"	and r11,r11,r8		\n"
		"	subf r9,r0,r9		\n"
		"	add r9,r9,r11		\n"
		"	sthu r9,2(%0)		\n"
		"	bdnz 1b			\n"
        :
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm2, clip_s16);
#endif

#ifdef ENABLE_BROKEN_IMPLS
/* This is just a basic weave of the previous function.  It uses
 * a lot of registers and gets pretty hairy, so it would take some
 * work to make better. */
static void
clip_s16_ppcasm3 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	src--;
	dest--;
	n/=2;
	__asm__ __volatile__("\n"
		"	mtctr %4		\n"
		"1:	lhau r9,2(%1)		\n"
		"	add r10,r9,%2		\n"
		"	 lhau r19,2(%1)		\n"
		"	subfc r8,r9,%3		\n"
		"	 add r20,r19,%2		\n"
		"	srawi r0,r10,31		\n"
		"	 subfc r18,r19,%3		\n"
		"	srawi r11,r8,31		\n"
		"	 srawi r22,r20,31		\n"
		"	and r0,r0,r10		\n"
		"	 srawi r21,r18,31		\n"
		"	and r11,r11,r8		\n"
		"	 and r22,r22,r20		\n"
		"	subf r9,r0,r9		\n"
		"	 and r21,r21,r18		\n"
		"	add r9,r9,r11		\n"
		"	 subf r19,r22,r19		\n"
		"	sthu r9,2(%0)		\n"
		"	 add r19,r19,r21		\n"
		"	 sthu r19,2(%0)		\n"
		"	bdnz 1b			\n"
	: 
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "8", "9", "10", "11", "0",
	  "18", "19", "20", "21", "22", "ctr");
}
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm3, clip_s16);
#endif


#ifdef HAVE_GCC_ASM_POWERPC_FPU
#ifdef ENABLE_BROKEN_IMPLS
static void
clip_f64_ppcasm(f64 *dest, f64 *src, f64 low, f64 hi, int n)
{
	f64 ftmp;

	dest--;
	src--;
	__asm__ __volatile__("\n"
		"1:	lfdu 0,8(%1)\n"
		"	addic. %2,%2,-1\n"
		"	fsub 1,0,%3\n"
		"	fsel 0,1,0,%3\n"
		"	fsub 1,0,%4\n"
		"	fsel 0,1,%4,0\n"
		"	stfdu 0,8(%0)\n"
		"	bge 1b\n"
	: "+b" (dest), "+b" (src), "+r" (n)
	: "f" (low), "f" (hi), "b" (&ftmp)
	: "32", "33", "11" );
}
#endif
#endif


