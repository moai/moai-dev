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

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(copy8x8_u8);

static void
copy8x8_u8_mmx (uint8_t *dest, int dstr, uint8_t *src, int sstr)
{
  __asm__ __volatile__ (
    "  lea         (%2, %2, 2), %%edi  \n\t"

    "  movq        (%1), %%mm0         \n\t"
    "  movq        (%1, %2), %%mm1     \n\t"
    "  movq        (%1, %2, 2), %%mm2  \n\t"
    "  movq        (%1, %%edi), %%mm3  \n\t"

    "  lea         (%1, %2, 4), %1     \n\t" 

    "  movq        %%mm0, (%0)         \n\t"
    "  movq        %%mm1, (%0, %2)     \n\t"
    "  movq        %%mm2, (%0, %2, 2)  \n\t"
    "  movq        %%mm3, (%0, %%edi)  \n\t"

    "  lea         (%0, %2, 4), %0     \n\t" 

    "  movq        (%1), %%mm0         \n\t"
    "  movq        (%1, %2), %%mm1     \n\t"
    "  movq        (%1, %2, 2), %%mm2  \n\t"
    "  movq        (%1, %%edi), %%mm3  \n\t"

    "  movq        %%mm0, (%0)         \n\t"
    "  movq        %%mm1, (%0, %2)     \n\t"
    "  movq        %%mm2, (%0, %2, 2)  \n\t"
    "  movq        %%mm3, (%0, %%edi)  \n\t"
    "  emms			       \n\t"
      : "+a" (dest)
      : "c" (src),
        "r" (sstr),
        "r" (dstr)
      : "memory", "edi"
  ); 
}
OIL_DEFINE_IMPL_FULL (copy8x8_u8_mmx, copy8x8_u8, OIL_IMPL_FLAG_MMX);

