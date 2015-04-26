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

OIL_DECLARE_CLASS (err_intra8x8_u8);
OIL_DECLARE_CLASS (err_inter8x8_u8);
OIL_DECLARE_CLASS (err_inter8x8_u8_avg);

static void
err_intra8x8_u8_mmx (uint32_t *dest, uint8_t *src1, int ss1)
{
  uint32_t  xsum;
  uint32_t  xxsum;

  __asm__ __volatile__ (
    "  pxor        %%mm5, %%mm5     \n\t"
    "  pxor        %%mm6, %%mm6     \n\t"
    "  pxor        %%mm7, %%mm7     \n\t"
    "  mov         $8, %%edi        \n\t"
    "1:                             \n\t"
    "  movq        (%2), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        %%mm0, %%mm2     \n\t"

    "  punpcklbw   %%mm6, %%mm0     \n\t"
    "  punpckhbw   %%mm6, %%mm2     \n\t"

    "  paddw       %%mm0, %%mm5     \n\t"
    "  paddw       %%mm2, %%mm5     \n\t"

    "  pmaddwd     %%mm0, %%mm0     \n\t"
    "  pmaddwd     %%mm2, %%mm2     \n\t"
    
    "  paddd       %%mm0, %%mm7     \n\t"
    "  paddd       %%mm2, %%mm7     \n\t"

    "  add         %3, %2           \n\t"	/* Inc pointer into src data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $32, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $16, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movd        %%mm5, %%edi     \n\t"
    "  movswl       %%di, %%edi      \n\t"
    "  movl        %%edi, %0        \n\t"

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddd       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %1        \n\t"
    "  emms                         \n\t"

     : "=r" (xsum),
       "=r" (xxsum),
       "+r" (src1) 
     : "r" (ss1)
     : "edi", "memory"
  );

  /* Compute population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum)); 
}
OIL_DEFINE_IMPL_FULL (err_intra8x8_u8_mmx, err_intra8x8_u8, OIL_IMPL_FLAG_MMX);

static void
err_inter8x8_u8_mmx (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  uint32_t  xsum;
  uint32_t  xxsum;

  __asm__ __volatile__ (
    "  pxor        %%mm5, %%mm5     \n\t"
    "  pxor        %%mm6, %%mm6     \n\t"
    "  pxor        %%mm7, %%mm7     \n\t"
    "  mov         $8, %%edi        \n\t"
    "1:                             \n\t"
    "  movq        (%2), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%3), %%mm1      \n\t"
    "  movq        %%mm0, %%mm2     \n\t"
    "  movq        %%mm1, %%mm3     \n\t"

    "  punpcklbw   %%mm6, %%mm0     \n\t"
    "  punpcklbw   %%mm6, %%mm1     \n\t"
    "  punpckhbw   %%mm6, %%mm2     \n\t"
    "  punpckhbw   %%mm6, %%mm3     \n\t"

    "  psubsw      %%mm1, %%mm0     \n\t"
    "  psubsw      %%mm3, %%mm2     \n\t"

    "  paddw       %%mm0, %%mm5     \n\t"
    "  paddw       %%mm2, %%mm5     \n\t"

    "  pmaddwd     %%mm0, %%mm0     \n\t"
    "  pmaddwd     %%mm2, %%mm2     \n\t"
    
    "  paddd       %%mm0, %%mm7     \n\t"
    "  paddd       %%mm2, %%mm7     \n\t"

    "  add         %4, %2           \n\t"	/* Inc pointer into src data */
    "  add         %5, %3           \n\t"	/* Inc pointer into ref data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $32, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $16, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movd        %%mm5, %%edi     \n\t"
    "  movswl       %%di, %%edi      \n\t"
    "  movl        %%edi, %0        \n\t"

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddd       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %1        \n\t"
    "  emms                         \n\t"

     : "=m" (xsum),
       "=m" (xxsum),
       "+r" (src1), 
       "+r" (src2) 
     : "m" (ss1),
       "m" (ss2)
     : "edi", "memory"
  );

  /* Compute and return population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum));
}
OIL_DEFINE_IMPL_FULL (err_inter8x8_u8_mmx, err_inter8x8_u8, OIL_IMPL_FLAG_MMX);

static void
err_inter8x8_u8_avg_mmx (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, uint8_t *src3, int ss2)
{
  uint32_t xsum;
  uint32_t xxsum;

  __asm__ __volatile__ (
    "  pcmpeqd     %%mm4, %%mm4     \n\t"	/* fefefefefefefefe in mm4 */
    "  paddb       %%mm4, %%mm4     \n\t"
    "  pxor        %%mm5, %%mm5     \n\t"
    "  pxor        %%mm6, %%mm6     \n\t"
    "  pxor        %%mm7, %%mm7     \n\t"
    "  mov         $8, %%edi        \n\t"
    "1:                             \n\t"
    "  movq        (%2), %%mm0      \n\t"	/* take 8 bytes */

    "  movq        (%3), %%mm2      \n\t"
    "  movq        (%4), %%mm3      \n\t"	/* take average of mm2 and mm3 */
    "  movq        %%mm2, %%mm1     \n\t"
    "  pand        %%mm3, %%mm1     \n\t"
    "  pxor        %%mm2, %%mm3     \n\t"
    "  pand        %%mm4, %%mm3     \n\t"
    "  psrlq       $1, %%mm3        \n\t"
    "  paddb       %%mm3, %%mm1     \n\t"

    "  movq        %%mm0, %%mm2     \n\t"
    "  movq        %%mm1, %%mm3     \n\t"

    "  punpcklbw   %%mm6, %%mm0     \n\t"
    "  punpcklbw   %%mm6, %%mm1     \n\t"
    "  punpckhbw   %%mm6, %%mm2     \n\t"
    "  punpckhbw   %%mm6, %%mm3     \n\t"

    "  psubsw      %%mm1, %%mm0     \n\t"
    "  psubsw      %%mm3, %%mm2     \n\t"

    "  paddw       %%mm0, %%mm5     \n\t"
    "  paddw       %%mm2, %%mm5     \n\t"

    "  pmaddwd     %%mm0, %%mm0     \n\t"
    "  pmaddwd     %%mm2, %%mm2     \n\t"
    
    "  paddd       %%mm0, %%mm7     \n\t"
    "  paddd       %%mm2, %%mm7     \n\t"

    "  add         %5, %2           \n\t"	/* Inc pointer into src data */
    "  add         %6, %3           \n\t"	/* Inc pointer into ref data */
    "  add         %6, %4           \n\t"	/* Inc pointer into ref data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $32, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $16, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movd        %%mm5, %%edi     \n\t"
    "  movswl       %%di, %%edi      \n\t"
    "  movl        %%edi, %0        \n\t"

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddd       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %1        \n\t"
    "  emms                         \n\t"

     : "=m" (xsum),
       "=m" (xxsum),
       "+r" (src1), 
       "+r" (src2),
       "+r" (src3) 
     : "m" (ss1),
       "m" (ss2)
     : "edi", "memory"
  );

  /* Compute and return population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum));
}

OIL_DEFINE_IMPL_FULL (err_inter8x8_u8_avg_mmx, err_inter8x8_u8_avg, OIL_IMPL_FLAG_MMX);
 
#ifdef ENABLE_BROKEN_IMPLS
static void
err_inter8x8_u8_avg_mmxext (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, uint8_t *src3, int ss2)
{
  uint32_t xsum;
  uint32_t xxsum;

  __asm__ __volatile__ (
    "  pxor        %%mm4, %%mm4     \n\t"
    "  pxor        %%mm5, %%mm5     \n\t"
    "  mov $0x01010101, %%edi \n\t"
    "  movd %%edi, %%mm6 \n\t"
    "  punpcklbw %%mm6, %%mm6 \n\t"
    "  pxor        %%mm7, %%mm7     \n\t"
    "  mov         $8, %%edi        \n\t"
    "1:                             \n\t"
    "  movq        (%2), %%mm0      \n\t"	/* take 8 bytes */

    "  movq        (%3), %%mm2      \n\t"
    "  movq        (%4), %%mm1      \n\t"	/* take average of mm2 and mm1 */
    "  movq        %%mm1, %%mm3     \n\t"
    "  pavgb       %%mm2, %%mm1     \n\t"
    "  pxor        %%mm2, %%mm3     \n\t"
    "  pand        %%mm6, %%mm3     \n\t"
    "  psubb       %%mm3, %%mm1     \n\t"

    "  movq        %%mm0, %%mm2     \n\t"
    "  movq        %%mm1, %%mm3     \n\t"

    "  punpcklbw   %%mm4, %%mm0     \n\t"
    "  punpcklbw   %%mm4, %%mm1     \n\t"
    "  punpckhbw   %%mm4, %%mm2     \n\t"
    "  punpckhbw   %%mm4, %%mm3     \n\t"

    "  psubsw      %%mm1, %%mm0     \n\t"
    "  psubsw      %%mm3, %%mm2     \n\t"

    "  paddw       %%mm0, %%mm5     \n\t"
    "  paddw       %%mm2, %%mm5     \n\t"

    "  pmaddwd     %%mm0, %%mm0     \n\t"
    "  pmaddwd     %%mm2, %%mm2     \n\t"
    
    "  paddd       %%mm0, %%mm7     \n\t"
    "  paddd       %%mm2, %%mm7     \n\t"

    "  add         %5, %2           \n\t"	/* Inc pointer into src data */
    "  add         %6, %3           \n\t"	/* Inc pointer into ref data */
    "  add         %6, %4           \n\t"	/* Inc pointer into ref data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $32, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movq        %%mm5, %%mm0     \n\t"
    "  psrlq       $16, %%mm5       \n\t"
    "  paddw       %%mm0, %%mm5     \n\t"
    "  movd        %%mm5, %%edi     \n\t"
    "  movswl       %%di, %%edi      \n\t"
    "  movl        %%edi, %0        \n\t"

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddd       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %1        \n\t"
    "  emms                         \n\t"

     : "=m" (xsum),
       "=m" (xxsum),
       "+r" (src1), 
       "+r" (src2),
       "+r" (src3) 
     : "m" (ss1),
       "m" (ss2)
     : "edi", "memory"
  );

  /* Compute and return population variance as mis-match metric. */
  *dest = (((xxsum<<6) - xsum*xsum));
}

OIL_DEFINE_IMPL_FULL (err_inter8x8_u8_avg_mmxext, err_inter8x8_u8_avg, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);
#endif

