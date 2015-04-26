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
/* This could probably be improved by unrolling */
static void multsum_f32_ppcasm(float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
	asm __volatile__("\n"
		"	lfs f0, 0(%0)\n"
		"	lfs f1, 0(%1)\n"
		"	fmuls f2, f0, f1\n"
		"	addi %2, %2, -1\n"
		"	mtctr %2\n"
		"1:	lfsu f0, 4(%0)\n"
		"	lfsu f1, 4(%1)\n"
		"	fmadds f2,f0,f1,f2\n"
		"	bdnz 1b\n"
		"	stfs f2, 0(%3)\n"
	: "+b" (src1), "+b" (src2), "+b" (n)
	: "b" (dest)
	: "32", "33", "34", "ctr");
}
OIL_DEFINE_IMPL_ASM (multsum_f32_ppcasm, multsum_f32);
#endif



