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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilclasses.h>


#if 0
static int16_t matrix[][4] = {
  { 0,      0,      -8192,   -8192  },
  { 16384,  0,      0,       0      },
  { 0,      16384,  16384,   16384  },
  { 0,      0,      -5638,   29032  },
  { 0,      22970,  -11700,  0      },
  { 0, 0, 0, 0 } 
#if 0
  { 0, 0, 0, 0 },
  { 16384,  0,      0,       0      },
  { 0, 16384,  0,      0,        },
  { 0, 0, 16384,  0,         },
  { 0, 0, 0, 16384,    },
  { 0, 0, 0, 0 } 
#endif
};
#endif

static void
colorspace_argb_mmx (uint32_t *argb, uint32_t *ayuv, int16_t *matrix, int n)
{
  asm volatile (
      "  sub $48, %%esp\n"
      "  pshufw $0x1b, 0(%3), %%mm0\n"
      "  movq %%mm0, 0(%%esp)\n"
      "  pshufw $0x1b, 8(%3), %%mm0\n"
      "  movq %%mm0, 8(%%esp)\n"
      "  pshufw $0x1b, 16(%3), %%mm0\n"
      "  movq %%mm0, 16(%%esp)\n"
      "  pshufw $0x1b, 16(%3), %%mm0\n"
      "  movq %%mm0, 16(%%esp)\n"
      "  pshufw $0x1b, 24(%3), %%mm0\n"
      "  movq %%mm0, 24(%%esp)\n"
      "  pshufw $0x1b, 32(%3), %%mm0\n"
      "  movq %%mm0, 32(%%esp)\n"
      "  pshufw $0x1b, 40(%3), %%mm7\n"
      "  pxor %%mm0, %%mm0\n"
      "  mov $0x08080808, %%eax\n"
      "  movd %%eax, %%mm1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  paddw %%mm1, %%mm7\n"
      "1:\n"
      "  movd (%1), %%mm1\n"
      "  add $4, %1\n"
      "  punpcklbw %%mm0, %%mm1\n"
      "  psllw $6, %%mm1\n"
      "  paddw 0(%%esp), %%mm1\n"
      "  pshufw $0x00, %%mm1, %%mm4\n"
      "  pshufw $0x55, %%mm1, %%mm3\n"
      "  pshufw $0xaa, %%mm1, %%mm2\n"
      "  pshufw $0xff, %%mm1, %%mm1\n"
      "  pmulhw 8(%%esp), %%mm1\n"
      "  pmulhw 16(%%esp), %%mm2\n"
      "  pmulhw 24(%%esp), %%mm3\n"
      "  pmulhw 32(%%esp), %%mm4\n"
      "  paddsw %%mm2, %%mm1\n"
      "  paddsw %%mm4, %%mm3\n"
      "  paddsw %%mm3, %%mm1\n"
      "  paddsw %%mm7, %%mm1\n"
      "  psraw $4, %%mm1\n"
      "  packuswb %%mm0, %%mm1\n"
      "  movd %%mm1, (%0)\n"
      "  add $4, %0\n"
      "  decl %2\n"
      "  jne 1b\n"
      "  emms\n"
      "  add $48, %%esp\n"
      : "+r" (argb), "+r" (ayuv), "+r" (n)
      : "r" (matrix)
      : "eax" );

}
OIL_DEFINE_IMPL_FULL (colorspace_argb_mmx, colorspace_argb, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

