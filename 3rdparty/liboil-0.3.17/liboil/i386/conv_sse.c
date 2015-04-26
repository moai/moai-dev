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
#include <liboil/liboilclasses.h>


/* suboptimal */
static void conv_f64_s32_sse(double *dst, int dst_stride, int32_t *src, int src_stride, int n)
{
	int i;

	for(i=0;i<n;i++){
		asm volatile(
			"	cvtsi2sd 0(%0), %%xmm0		\n"
			"	movq %%xmm0, 0(%1)		\n"
			:
			: "a" (src), "c" (dst)
			/* : "xmm0" */
		);
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_f64_s32_sse, conv_f64_s32, OIL_IMPL_FLAG_SSE2);

/* suboptimal */
static void conv_s32_f64_sse(int32_t *dst, int dst_stride, double *src, int src_stride, int n)
{
	int i;
	int eax;

	for(i=0;i<n;i++){
		asm volatile(
			"	cvtsd2si 0(%1), %0		\n"
			: "=a" (eax)
			: "c" (src)
		);
		*dst = eax;
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_s32_f64_sse, conv_s32_f64, OIL_IMPL_FLAG_SSE2);

/* suboptimal */
static void conv_s32_f32_sse(int32_t *dst, int dst_stride, double *src, int src_stride, int n)
{
	int i;

	for(i=0;i<n;i++){
		asm volatile(
                    "  movd 0(%1), %%xmm0\n"
		    "  cvtps2pi %%xmm0, %%mm0\n"
                    "  movd %%mm0, 0(%0)\n"
			:
                        : "r" (dst), "r" (src)
		);
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
        asm volatile ("emms\n");
}
OIL_DEFINE_IMPL_FULL(conv_s32_f32_sse, conv_s32_f32, OIL_IMPL_FLAG_SSE2 | OIL_IMPL_FLAG_MMX);

