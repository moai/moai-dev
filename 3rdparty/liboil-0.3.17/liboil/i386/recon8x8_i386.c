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

OIL_DECLARE_CLASS (recon8x8_intra);
OIL_DECLARE_CLASS (recon8x8_inter);
OIL_DECLARE_CLASS (recon8x8_inter2);

const uint8_t c0x80[8] = {
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
};

static void
recon8x8_intra_i386_mmx (uint8_t *dest, int ds, int16_t *change)
{
  __asm__ __volatile__ (
    "  movq        (%3), %%mm0       \n\t" /* Set mm0 to 0x8080808080808080 */

    "  lea         128(%1), %%edi      \n\t" /* Endpoint in input buffer */
    "1:                                \n\t" 
    "  movq         (%1), %%mm2        \n\t" /* First four input values */

    "  packsswb    8(%1), %%mm2        \n\t" /* pack with next(high) four values */
    "  por         %%mm0, %%mm0        \n\t" 
    "  pxor        %%mm0, %%mm2        \n\t" /* Convert result to unsigned (same as add 128) */
    "  lea         16(%1), %1          \n\t" /* Step source buffer */
    "  cmp         %%edi, %1           \n\t" /* are we done */

    "  movq        %%mm2, (%0)         \n\t" /* store results */

    "  lea         (%0, %2), %0        \n\t" /* Step output buffer */
    "  jc          1b                  \n\t" /* Loop back if we are not done */
    "  emms                            \n\t"
      : "+r" (dest)
      : "r" (change),
        "r" (ds),
        "r" (c0x80)
      : "memory", "edi"
  );
}

OIL_DEFINE_IMPL_FULL (recon8x8_intra_i386_mmx, recon8x8_intra, OIL_IMPL_FLAG_MMX);

#if 0
static void
recon8x8_inter_i386_mmx (uint8_t *dest, int ds, uint8_t *src, int ss, int16_t *change)
{
  /* FIXME doesn't handle ss */
  __asm__ __volatile__ (
    "  pxor        %%mm0, %%mm0        \n\t"
    "  lea         128(%1), %%edi      \n\t"

    "1:                                \n\t"
    "  movq        (%2), %%mm2         \n\t" /* (+3 misaligned) 8 reference pixels */

    "  movq        (%1), %%mm4         \n\t" /* first 4 changes */
    "  movq        %%mm2, %%mm3        \n\t"
    "  movq        8(%1), %%mm5        \n\t" /* last 4 changes */
    "  punpcklbw   %%mm0, %%mm2        \n\t" /* turn first 4 refs into positive 16-bit #s */
    "  paddsw      %%mm4, %%mm2        \n\t" /* add in first 4 changes */
    "  punpckhbw   %%mm0, %%mm3        \n\t" /* turn last 4 refs into positive 16-bit #s */
    "  paddsw      %%mm5, %%mm3        \n\t" /* add in last 4 changes */
    "  add         %3, %2              \n\t" /* next row of reference pixels */
    "  packuswb    %%mm3, %%mm2        \n\t" /* pack result to unsigned 8-bit values */
    "  lea         16(%1), %1          \n\t" /* next row of changes */
    "  cmp         %%edi, %1           \n\t" /* are we done? */

    "  movq        %%mm2, (%0)         \n\t" /* store result */

    "  lea         (%0, %3), %0        \n\t" /* next row of output */
    "  jc          1b                  \n\t"
    "  emms                            \n\t"
      : "+r" (dest)
      : "r" (change),
        "r" (src),
        "r" (ds)
      : "memory", "edi"
  );
}

OIL_DEFINE_IMPL_FULL (recon8x8_inter_i386_mmx, recon8x8_inter, OIL_IMPL_FLAG_MMX);

static void
recon8x8_inter2_i386_mmx (uint8_t *dest, int ds, uint8_t *s1, int ss1, uint8_t *s2, int ss2, int16_t *change)
{
  /* FIXME doesn't handle ss1, ss2 */
  __asm__ __volatile__ (
    "  pxor        %%mm0, %%mm0        \n\t"
    "  lea         128(%1), %%edi      \n\t"

    "1:                                \n\t"
    "  movq        (%2), %%mm2         \n\t" /* (+3 misaligned) 8 reference pixels */
    "  movq        (%3), %%mm4         \n\t" /* (+3 misaligned) 8 reference pixels */

    "  movq        %%mm2, %%mm3        \n\t"
    "  punpcklbw   %%mm0, %%mm2        \n\t" /* mm2 = start ref1 as positive 16-bit #s */
    "  movq        %%mm4, %%mm5        \n\t"
    "  movq        (%1), %%mm6         \n\t" /* first 4 changes */
    "  punpckhbw   %%mm0, %%mm3        \n\t" /* mm3 = end ref1 as positive 16-bit #s */
    "  movq        8(%1), %%mm7        \n\t" /* last 4 changes */
    "  punpcklbw   %%mm0, %%mm4        \n\t" /* mm4 = start ref2 as positive 16-bit #s */
    "  punpckhbw   %%mm0, %%mm5        \n\t" /* mm5 = end ref2 as positive 16-bit #s */
    "  paddw       %%mm4, %%mm2        \n\t" /* mm2 = start (ref1 + ref2) */
    "  paddw       %%mm5, %%mm3        \n\t" /* mm3 = end (ref1 + ref2) */
    "  psrlw       $1, %%mm2           \n\t" /* mm2 = start (ref1 + ref2)/2 */
    "  psrlw       $1, %%mm3           \n\t" /* mm3 = end (ref1 + ref2)/2 */
    "  paddw       %%mm6, %%mm2        \n\t" /* add changes to start */
    "  paddw       %%mm7, %%mm3        \n\t" /* add changes to end */
    "  lea         16(%1), %1          \n\t" /* next row of changes */
    "  packuswb    %%mm3, %%mm2        \n\t" /* pack start|end to unsigned 8-bit */
    "  add         %4, %2              \n\t" /* next row of reference pixels */
    "  add         %4, %3              \n\t" /* next row of reference pixels */
    "  movq        %%mm2, (%0)         \n\t" /* store result */
    "  add         %4, %0              \n\t" /* next row of output */
    "  cmp         %%edi, %1           \n\t" /* are we done? */
    "  jc          1b                  \n\t"
    "  emms                            \n\t"
      : "+r" (dest)
      : "r" (change),
        "r" (s1),
        "r" (s2),
        "m" (ds)
      : "memory", "edi"
  );
}

OIL_DEFINE_IMPL_FULL (recon8x8_inter2_i386_mmx, recon8x8_inter2, OIL_IMPL_FLAG_MMX);
#endif
