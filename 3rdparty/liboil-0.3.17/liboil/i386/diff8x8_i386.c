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

OIL_DECLARE_CLASS (diff8x8_s16_u8);
OIL_DECLARE_CLASS (diff8x8_const128_s16_u8);
OIL_DECLARE_CLASS (diff8x8_average_s16_u8);


static void
diff8x8_s16_u8_mmx (int16_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  __asm__ __volatile__ (
    "  pxor        %%mm7, %%mm7     \n\t" 

#define LOOP \
    "  movq        (%0), %%mm0      \n\t" /* mm0 = FiltPtr */ \
    "  movq        (%1), %%mm1      \n\t" /* mm1 = ReconPtr */ \
    "  movq        %%mm0, %%mm2     \n\t" /* dup to prepare for up conversion */ \
    "  movq        %%mm1, %%mm3     \n\t" /* dup to prepare for up conversion */ \
    /* convert from UINT8 to INT16 */ \
    "  punpcklbw   %%mm7, %%mm0     \n\t" /* mm0 = INT16(FiltPtr) */ \
    "  punpcklbw   %%mm7, %%mm1     \n\t" /* mm1 = INT16(ReconPtr) */ \
    "  punpckhbw   %%mm7, %%mm2     \n\t" /* mm2 = INT16(FiltPtr) */ \
    "  punpckhbw   %%mm7, %%mm3     \n\t" /* mm3 = INT16(ReconPtr) */ \
    /* start calculation */ \
    "  psubw       %%mm1, %%mm0     \n\t" /* mm0 = FiltPtr - ReconPtr */ \
    "  psubw       %%mm3, %%mm2     \n\t" /* mm2 = FiltPtr - ReconPtr */ \
    "  movq        %%mm0,  (%2)     \n\t" /* write answer out */ \
    "  movq        %%mm2, 8(%2)     \n\t" /* write answer out */ \
    /* Increment pointers */ \
    "  add         $16, %2          \n\t" \
    "  add         %3, %0           \n\t" \
    "  add         %4, %1           \n\t"

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  emms                         \n\t"

     : "+r" (src1),
       "+r" (src2),
       "+r" (dest)
     : "m" (ss1),
       "m" (ss2) 
     : "memory"
  );
}
OIL_DEFINE_IMPL_FULL (diff8x8_s16_u8_mmx, diff8x8_s16_u8, OIL_IMPL_FLAG_MMX);

static void
diff8x8_const128_s16_u8_mmx (int16_t *dest, uint8_t *src1, int ss1)
{
  const int16_t tmp[4] = { 0x0080, 0x0080, 0x0080, 0x0080 };

  __asm__ __volatile__ (
    "  pxor        %%mm7, %%mm7     \n\t" 
    "  movq        (%3), %%mm1  \n\t"

#define LOOP \
    "  movq        (%0), %%mm0      \n\t" /* mm0 = FiltPtr */ \
    "  movq        %%mm0, %%mm2     \n\t" /* dup to prepare for up conversion */ \
    /* convert from UINT8 to INT16 */ \
    "  punpcklbw   %%mm7, %%mm0     \n\t" /* mm0 = INT16(FiltPtr) */ \
    "  punpckhbw   %%mm7, %%mm2     \n\t" /* mm2 = INT16(FiltPtr) */ \
    /* start calculation */ \
    "  psubw       %%mm1, %%mm0     \n\t" /* mm0 = FiltPtr - 128 */ \
    "  psubw       %%mm1, %%mm2     \n\t" /* mm2 = FiltPtr - 128 */ \
    "  movq        %%mm0,  (%1)     \n\t" /* write answer out */ \
    "  movq        %%mm2, 8(%1)     \n\t" /* write answer out */ \
    /* Increment pointers */ \
    "  add         $16, %1           \n\t" \
    "  add         %2, %0           \n\t"

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  emms                         \n\t"

     : "+r" (src1),
       "+r" (dest)
     : "r" (ss1),
       "r" (tmp)
     : "memory"
  );
}
OIL_DEFINE_IMPL_FULL (diff8x8_const128_s16_u8_mmx, diff8x8_const128_s16_u8, OIL_IMPL_FLAG_MMX);

static void
diff8x8_average_s16_u8_mmx (int16_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2, uint8_t *src3)
{
  __asm__ __volatile__ (
    "  pxor        %%mm7, %%mm7     \n\t" 

#define LOOP \
    "  movq        (%0), %%mm0      \n\t" /* mm0 = FiltPtr */ \
    "  movq        (%1), %%mm1      \n\t" /* mm1 = ReconPtr1 */ \
    "  movq        (%2), %%mm4      \n\t" /* mm1 = ReconPtr2 */ \
    "  movq        %%mm0, %%mm2     \n\t" /* dup to prepare for up conversion */ \
    "  movq        %%mm1, %%mm3     \n\t" /* dup to prepare for up conversion */ \
    "  movq        %%mm4, %%mm5     \n\t" /* dup to prepare for up conversion */ \
    /* convert from UINT8 to INT16 */ \
    "  punpcklbw   %%mm7, %%mm0     \n\t" /* mm0 = INT16(FiltPtr) */ \
    "  punpcklbw   %%mm7, %%mm1     \n\t" /* mm1 = INT16(ReconPtr1) */ \
    "  punpcklbw   %%mm7, %%mm4     \n\t" /* mm1 = INT16(ReconPtr2) */ \
    "  punpckhbw   %%mm7, %%mm2     \n\t" /* mm2 = INT16(FiltPtr) */ \
    "  punpckhbw   %%mm7, %%mm3     \n\t" /* mm3 = INT16(ReconPtr1) */ \
    "  punpckhbw   %%mm7, %%mm5     \n\t" /* mm3 = INT16(ReconPtr2) */ \
    /* average ReconPtr1 and ReconPtr2 */ \
    "  paddw       %%mm4, %%mm1     \n\t" /* mm1 = ReconPtr1 + ReconPtr2 */ \
    "  paddw       %%mm5, %%mm3     \n\t" /* mm3 = ReconPtr1 + ReconPtr2 */ \
    "  psrlw       $1, %%mm1        \n\t" /* mm1 = (ReconPtr1 + ReconPtr2) / 2 */ \
    "  psrlw       $1, %%mm3        \n\t" /* mm3 = (ReconPtr1 + ReconPtr2) / 2 */ \
    "  psubw       %%mm1, %%mm0     \n\t" /* mm0 = FiltPtr - ((ReconPtr1 + ReconPtr2) / 2) */ \
    "  psubw       %%mm3, %%mm2     \n\t" /* mm2 = FiltPtr - ((ReconPtr1 + ReconPtr2) / 2) */ \
    "  movq        %%mm0,  (%3)     \n\t" /* write answer out */ \
    "  movq        %%mm2, 8(%3)     \n\t" /* write answer out */ \
    /* Increment pointers */ \
    "  add         $16, %3           \n\t" \
    "  add         %4, %0           \n\t" \
    "  add         %5, %1           \n\t" \
    "  add         %5, %2           \n\t"

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  emms                         \n\t"

     : "+r" (src1),
       "+r" (src2),
       "+r" (src3),
       "+r" (dest)
     : "m" (ss1),
       "m" (ss2) 
     : "memory"
  );
}
OIL_DEFINE_IMPL_FULL (diff8x8_average_s16_u8_mmx, diff8x8_average_s16_u8, OIL_IMPL_FLAG_MMX);

