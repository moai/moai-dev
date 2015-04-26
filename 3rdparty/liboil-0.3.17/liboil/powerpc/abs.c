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

#ifdef ENABLE_BROKEN_IMPLS
static void
abs_u16_s16_a16_altivec (uint16_t * dest, int dstr, int16_t * src, int sstr,
    int n)
{
  int i;

  for (i = n & ~0x7; i < n; i++) {
    *dest = ABS (*src);
    OIL_INCREMENT (dest, dstr);
    OIL_INCREMENT (src, sstr);
  }
  n /= 8;
  __asm__ __volatile__ ("\n"
      "	li r10, 0			\n"
      "	vxor v2, v2, v2		\n"
      "	mtctr %2			\n"
      "1:	lvx v0,r10,%1		\n"
      "	vsubshs v1, v2, v0	\n"
      "	vmaxsh v1, v1, v0		\n"
      "	stvx v1,r10,%0		\n"
      "	addi r10, r10, 16		\n"
      "	bdnz 1b				\n":"+b" (dest), "+b" (src), "+b" (n)
      ::"10", "ctr");
}

OIL_DEFINE_IMPL_FULL (abs_u16_s16_a16_altivec, abs_u16_s16, OIL_IMPL_FLAG_ALTIVEC);
#endif

