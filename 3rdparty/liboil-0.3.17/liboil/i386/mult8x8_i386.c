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


static void
mult8x8_s16_mmx(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1,
    int sstr2)
{

        asm volatile(
            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"

            "  add %3, %0 \n"
            "  add %4, %1 \n"
            "  add %5, %2 \n"

            "  movq 0(%1), %%mm0 \n"
            "  pmullw 0(%2), %%mm0 \n"
            "  movq %%mm0, 0(%0) \n"
            "  movq 8(%1), %%mm1 \n"
            "  pmullw 8(%2), %%mm1 \n"
            "  movq %%mm1, 8(%0) \n"
            "  emms\n "

            : "+r" (dest), "+r" (src1), "+r" (src2)
            : "m" (dstr), "m" (sstr1), "m" (sstr2));
}

OIL_DEFINE_IMPL_FULL (mult8x8_s16_mmx, mult8x8_s16, OIL_IMPL_FLAG_MMX);

