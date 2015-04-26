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
static void
conv_f32_s16_3dnow (float *dst, int dst_stride, int16_t * src, int src_stride,
    int n)
{
  int i;

  for (i = 0; i < n; i++) {
    asm volatile ("  movswl 0(%0), %%eax \n"
        "  movd %%eax, %%mm0 \n"
        "  pi2fd %%mm0, %%mm0 \n" "  movd %%mm0, 0(%1) \n"
        ::"r" (src), "r" (dst)
        :"eax");

    dst = OIL_OFFSET (dst, dst_stride);
    src = OIL_OFFSET (src, src_stride);
  }
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (conv_f32_s16_3dnow, conv_f32_s16, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_3DNOW);

#ifdef ENABLE_BROKEN_IMPLS
/* suboptimal */
/* This appears to fail because of differences in rounding of half
 * integers. */
static void
conv_s32_f32_3dnow (int32_t * dst, int dst_stride, float *src, int src_stride,
    int n)
{
  int i;
  const float constants[][2] = {
    { 0.5,  0.5},
    {-1.0, -1.0}
  };

  for (i = 0; i < n; i++) {
    asm volatile (
        "  movq 0(%0), %%mm0 \n"
        "  pxor %%mm1, %%mm1 \n"
        "  pfcmpgt %%mm0, %%mm1 \n"
        "  movq %%mm0, %%mm2 \n"
        "  pfadd 0(%2), %%mm0 \n"
        "  pfcmpeq %%mm0, %%mm2 \n"
        "  pandn %%mm1, %%mm2 \n"
        "  pf2id %%mm0, %%mm0 \n"
        "  paddd %%mm2, %%mm0 \n"
        "  movd %%mm0, 0(%1) \n"
        :
        :"r" (src), "r" (dst), "r" (constants)
        );

    dst = OIL_OFFSET (dst, dst_stride);
    src = OIL_OFFSET (src, src_stride);
  }
  asm volatile ("emms");
}

OIL_DEFINE_IMPL_FULL (conv_s32_f32_3dnow, conv_s32_f32, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_3DNOW);
#endif

