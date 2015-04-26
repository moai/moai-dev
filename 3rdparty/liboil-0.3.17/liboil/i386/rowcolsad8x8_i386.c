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

OIL_DECLARE_CLASS (rowsad8x8_u8);
OIL_DECLARE_CLASS (colsad8x8_u8);

static void
rowsad8x8_u8_mmx (uint32_t *dest, uint8_t *src1, uint8_t *src2)
{
  uint32_t MaxSad;

  __asm__ __volatile__ (
    "  pxor        %%mm6, %%mm6     \n\t"	/* zero out mm6 for unpack */
    "  pxor        %%mm7, %%mm7     \n\t" 	/* zero out mm7 for unpack */
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%2), %%mm1      \n\t"

    "  movq        %%mm0, %%mm2     \n\t"
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */

    "  movq        %%mm0, %%mm1     \n\t"

    "  punpcklbw   %%mm6, %%mm0     \n\t"       /* ; unpack low four bytes to higher precision */
    "  punpckhbw   %%mm7, %%mm1     \n\t"       /* ; unpack high four bytes to higher precision */

    "  movq        %%mm0, %%mm2     \n\t"
    "  movq        %%mm1, %%mm3     \n\t"
    "  psrlq       $32, %%mm2       \n\t"	/* fold and add */
    "  psrlq       $32, %%mm3       \n\t"
    "  paddw       %%mm2, %%mm0     \n\t"
    "  paddw       %%mm3, %%mm1     \n\t"
    "  movq        %%mm0, %%mm2     \n\t"
    "  movq        %%mm1, %%mm3     \n\t"
    "  psrlq       $16, %%mm2       \n\t"
    "  psrlq       $16, %%mm3       \n\t"
    "  paddw       %%mm2, %%mm0     \n\t"
    "  paddw       %%mm3, %%mm1     \n\t"

    "  psubusw     %%mm0, %%mm1     \n\t"
    "  paddw       %%mm0, %%mm1     \n\t" 	/* mm1 = max(mm1, mm0) */
    "  movd        %%mm1, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=m" (MaxSad),
       "+r" (src1), 
       "+r" (src2) 
     :
     : "memory"
  );
  *dest = MaxSad;
}
OIL_DEFINE_IMPL_FULL (rowsad8x8_u8_mmx, rowsad8x8_u8, OIL_IMPL_FLAG_MMX);

static void
rowsad8x8_u8_mmxext (uint32_t *dest, uint8_t *src1, uint8_t *src2)
{
  uint32_t MaxSad;

  __asm__ __volatile__ (
    "  movd        (%1), %%mm0      \n\t"
    "  movd        (%2), %%mm1      \n\t"
    "  psadbw      %%mm0, %%mm1     \n\t"
    "  movd        4(%1), %%mm2     \n\t"
    "  movd        4(%2), %%mm3     \n\t"
    "  psadbw      %%mm2, %%mm3     \n\t"

    "  pmaxsw      %%mm1, %%mm3     \n\t"
    "  movd        %%mm3, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=m" (MaxSad),
       "+r" (src1), 
       "+r" (src2) 
     :
     : "memory"
  );
  *dest = MaxSad;
}
OIL_DEFINE_IMPL_FULL (rowsad8x8_u8_mmxext, rowsad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
colsad8x8_u8_mmx (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  uint32_t MaxSad;

  __asm__ __volatile__ (
    "  pxor        %%mm3, %%mm3     \n\t"	/* zero out mm3 for unpack */
    "  pxor        %%mm4, %%mm4     \n\t"	/* mm4 low sum */
    "  pxor        %%mm5, %%mm5     \n\t" 	/* mm5 high sum */
    "  pxor        %%mm6, %%mm6     \n\t"	/* mm6 low sum */
    "  pxor        %%mm7, %%mm7     \n\t" 	/* mm7 high sum */
    "  mov         $4, %%edi        \n\t"	/* 4 rows */
    "1:                             \n\t"
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%2), %%mm1      \n\t"	/* take 8 bytes */

    "  movq        %%mm0, %%mm2     \n\t"
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */
    "  movq        %%mm0, %%mm1     \n\t"

    "  punpcklbw   %%mm3, %%mm0     \n\t"	/* unpack to higher precision for accumulation */
    "  paddw       %%mm0, %%mm4     \n\t"	/* accumulate difference... */
    "  punpckhbw   %%mm3, %%mm1     \n\t"	/* unpack high four bytes to higher precision */
    "  paddw       %%mm1, %%mm5     \n\t"	/* accumulate difference... */
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */
    "  add         %3, %2           \n\t"	/* Inc pointer into the new data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  mov         $4, %%edi        \n\t"	/* 4 rows */
    "2:                             \n\t"
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%2), %%mm1      \n\t"	/* take 8 bytes */

    "  movq        %%mm0, %%mm2     \n\t"
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */
    "  movq        %%mm0, %%mm1     \n\t"

    "  punpcklbw   %%mm3, %%mm0     \n\t"	/* unpack to higher precision for accumulation */
    "  paddw       %%mm0, %%mm6     \n\t"	/* accumulate difference... */
    "  punpckhbw   %%mm3, %%mm1     \n\t"	/* unpack high four bytes to higher precision */
    "  paddw       %%mm1, %%mm7     \n\t"	/* accumulate difference... */
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */
    "  add         %3, %2           \n\t"	/* Inc pointer into the new data */

    "  dec         %%edi            \n\t"
    "  jnz 2b                       \n\t"

    "  psubusw     %%mm6, %%mm7     \n\t"
    "  paddw       %%mm6, %%mm7     \n\t" 	/* mm7 = max(mm7, mm6) */
    "  psubusw     %%mm4, %%mm5     \n\t" 	
    "  paddw       %%mm4, %%mm5     \n\t" 	/* mm5 = max(mm5, mm4) */
    "  psubusw     %%mm5, %%mm7     \n\t" 	
    "  paddw       %%mm5, %%mm7     \n\t" 	/* mm7 = max(mm5, mm7) */
    "  movq        %%mm7, %%mm6     \n\t"
    "  psrlq       $32, %%mm6       \n\t"
    "  psubusw     %%mm6, %%mm7     \n\t" 	
    "  paddw       %%mm6, %%mm7     \n\t" 	/* mm7 = max(mm5, mm7) */
    "  movq        %%mm7, %%mm6     \n\t"
    "  psrlq       $16, %%mm6       \n\t"
    "  psubusw     %%mm6, %%mm7     \n\t" 	
    "  paddw       %%mm6, %%mm7     \n\t" 	/* mm7 = max(mm5, mm7) */
    "  movd        %%mm7, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=r" (MaxSad),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (ss1)
     : "memory", "edi"
  );
  *dest = MaxSad;
}
OIL_DEFINE_IMPL_FULL (colsad8x8_u8_mmx, colsad8x8_u8, OIL_IMPL_FLAG_MMX);

static void
colsad8x8_u8_mmxext (uint32_t *dest, uint8_t *src1, int ss1, uint8_t *src2, int ss2)
{
  uint32_t MaxSad;

  __asm__ __volatile__ (
    "  pxor        %%mm3, %%mm3     \n\t"	/* zero out mm3 for unpack */
    "  pxor        %%mm4, %%mm4     \n\t"	/* mm4 low sum */
    "  pxor        %%mm5, %%mm5     \n\t" 	/* mm5 high sum */
    "  pxor        %%mm6, %%mm6     \n\t"	/* mm6 low sum */
    "  pxor        %%mm7, %%mm7     \n\t" 	/* mm7 high sum */
    "  mov         $4, %%edi        \n\t"	/* 4 rows */
    "1:                             \n\t"
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%2), %%mm1      \n\t"	/* take 8 bytes */

    "  movq        %%mm0, %%mm2     \n\t"
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */
    "  movq        %%mm0, %%mm1     \n\t"

    "  punpcklbw   %%mm3, %%mm0     \n\t"	/* unpack to higher precision for accumulation */
    "  paddw       %%mm0, %%mm4     \n\t"	/* accumulate difference... */
    "  punpckhbw   %%mm3, %%mm1     \n\t"	/* unpack high four bytes to higher precision */
    "  paddw       %%mm1, %%mm5     \n\t"	/* accumulate difference... */
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */
    "  add         %3, %2           \n\t"	/* Inc pointer into the new data */

    "  dec         %%edi            \n\t"
    "  jnz 1b                       \n\t"

    "  mov         $4, %%edi        \n\t"	/* 4 rows */
    "2:                             \n\t"
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */
    "  movq        (%2), %%mm1      \n\t"	/* take 8 bytes */

    "  movq        %%mm0, %%mm2     \n\t"
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */
    "  movq        %%mm0, %%mm1     \n\t"

    "  punpcklbw   %%mm3, %%mm0     \n\t"	/* unpack to higher precision for accumulation */
    "  paddw       %%mm0, %%mm6     \n\t"	/* accumulate difference... */
    "  punpckhbw   %%mm3, %%mm1     \n\t"	/* unpack high four bytes to higher precision */
    "  paddw       %%mm1, %%mm7     \n\t"	/* accumulate difference... */
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */
    "  add         %3, %2           \n\t"	/* Inc pointer into the new data */

    "  dec         %%edi            \n\t"
    "  jnz 2b                       \n\t"

    "  pmaxsw      %%mm6, %%mm7     \n\t"
    "  pmaxsw      %%mm4, %%mm5     \n\t"
    "  pmaxsw      %%mm5, %%mm7     \n\t"
    "  movq        %%mm7, %%mm6     \n\t"
    "  psrlq       $32, %%mm6       \n\t"
    "  pmaxsw      %%mm6, %%mm7     \n\t"
    "  movq        %%mm7, %%mm6     \n\t"
    "  psrlq       $16, %%mm6       \n\t"
    "  pmaxsw      %%mm6, %%mm7     \n\t"
    "  movd        %%mm7, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=r" (MaxSad),
       "+r" (src1), 
       "+r" (src2) 
     : "r" (ss1)
     : "memory", "edi"
  );

  *dest = MaxSad;
}
OIL_DEFINE_IMPL_FULL (colsad8x8_u8_mmxext, colsad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

