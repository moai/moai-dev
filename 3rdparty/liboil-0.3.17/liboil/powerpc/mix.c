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
#include <math.h>


#ifdef ENABLE_BROKEN_IMPLS
static void mix_u8_a16_altivec(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	static const uint8_t permute[] __attribute__ ((__aligned__ (16))) = {
		0, 2, 4, 6, 8,10,12,14,16,18,20,22,24,26,28,30 };
	const uint8_t *perm = permute;
	int i;

	for(i=(n&~0xf);i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
        
	__asm__ __volatile__("\n"
		"	li	r12, 0		\n"
		"	vxor	v3, v3, v3	\n"
		"	lvx	v12, %4, r12	\n"
		"	mtctr	%5			\n"
		"1:	lvx	v0, %1, r12		\n"
		"	lvx	v1, %2, r12		\n"
		"	lvx	v2, %3, r12		\n"

		"	vmrghb	v4, v3, v0	\n"
		"	vmrghb	v5, v3, v1	\n"
		"	vmrghb	v6, v3, v2	\n"
		"	vmrghb	v7, v1, v3	\n"
		"	vsubuhs	v8, v7, v5	\n"
		"	vsubshs	v9, v4, v5	\n"
		"	vmladduhm	v10, v9, v6, v8	\n"

		"	vmrglb	v4, v3, v0	\n"
		"	vmrglb	v5, v3, v1	\n"
		"	vmrglb	v6, v3, v2	\n"
		"	vmrglb	v7, v1, v3	\n"
		"	vsubuhs	v8, v7, v5	\n"
		"	vsubshs	v9, v4, v5	\n"
		"	vmladduhm	v11, v9, v6, v8	\n"

		"	vperm	v0, v10, v11, v12	\n"
		"	stvx	v0, %0, r12		\n"
		"	addi	r12, r12, 16	\n"
		"	bdnz	1b			\n"
	: "+b" (dest), "+b" (src1), "+b" (src2), "+b" (scale), "+b" (perm)
	: "r" (n/16)
	: "12", "ctr");
}
OIL_DEFINE_IMPL_FULL (mix_u8_a16_altivec, mix_u8, OIL_IMPL_FLAG_ALTIVEC);
#endif

