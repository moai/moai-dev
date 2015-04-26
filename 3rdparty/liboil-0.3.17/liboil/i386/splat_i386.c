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
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(splat_u32_ns);

static void
splat_u32_ns_i386_mmx (uint32_t *dest, uint32_t *src, int n)
{
  while(n&0x7) {
    *dest++ = *src;
    n--;
  }
  if (n) asm volatile (
      "  mov $0, %%eax\n"
      "  movq (%1), %%mm0\n"
      "  punpckldq (%1), %%mm0\n"
      "1:\n"
      "  movntq %%mm0, (%0,%%eax)\n"
      "  movntq %%mm0, 8(%0,%%eax)\n"
      "  movntq %%mm0, 16(%0,%%eax)\n"
      "  movntq %%mm0, 24(%0,%%eax)\n"
      "  add $32, %%eax\n"
      "  decl %%ecx\n"
      "  jne 1b\n"
      "  sfence\n"
      "  emms\n"
      : "+r" (dest), "+r" (src)
      : "c" (n/8)
      : "eax");
}
OIL_DEFINE_IMPL_FULL (splat_u32_ns_i386_mmx, splat_u32_ns, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);



