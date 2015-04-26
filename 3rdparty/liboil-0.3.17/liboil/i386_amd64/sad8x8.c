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
#include <stddef.h>


static void
sad8x8_u8_mmx (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor        %%mm6, %%mm6     \n\t"	/* zero out mm6 for unpack */
    "  pxor        %%mm7, %%mm7     \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq        (%1), %%mm0      \n\t"	/* take 8 bytes */ \
    "  movq        (%2), %%mm1      \n\t" \
    "  movq        %%mm0, %%mm2     \n\t" \
 \
    "  psubusb     %%mm1, %%mm0     \n\t" 	/* A - B */ \
    "  psubusb     %%mm2, %%mm1     \n\t"	/* B - A */ \
    "  por         %%mm1, %%mm0     \n\t"      	/* and or gives abs difference */ \
    "  movq        %%mm0, %%mm1     \n\t" \
 \
    "  punpcklbw   %%mm6, %%mm0     \n\t"	/* unpack to higher precision for accumulation */ \
    "  paddw       %%mm0, %%mm7     \n\t"	/* accumulate difference... */ \
    "  punpckhbw   %%mm6, %%mm1     \n\t"	/* unpack high four bytes to higher precision */ \
    "  add         %3, %1           \n\t"	/* Inc pointer into the new data */ \
    "  paddw       %%mm1, %%mm7     \n\t"	/* accumulate difference... */ \
    "  add         %4, %2           \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $32, %%mm7       \n\t"
    "  paddw       %%mm0, %%mm7     \n\t"
    "  movq        %%mm7, %%mm0     \n\t"
    "  psrlq       $16, %%mm7       \n\t"
    "  paddw       %%mm0, %%mm7     \n\t"
    "  movd        %%mm7, %0        \n\t"
    "  andl        $0xffff, %0      \n\t"
    "  emms                         \n\t"

     : "=m" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmx, sad8x8_u8, OIL_IMPL_FLAG_MMX);

static void
sad8x8_u8_mmxext (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  movq (%2), %%mm1             \n\t" \
    "  psadbw %%mm1, %%mm0          \n\t" \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  movq (%2), %%mm1             \n\t"
    "  psadbw %%mm1, %%mm0          \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_2 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  psadbw (%2), %%mm0           \n\t" \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t" \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_2, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_3 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  lea (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  lea (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  lea (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  lea (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  lea (%1,%3,2), %1           \n\t"	/* Inc pointer into the new data */
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  lea (%2,%4,2), %2           \n\t"	/* Inc pointer into ref data */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  psadbw (%2), %%mm0           \n\t"
    "  movq (%1,%3), %%mm1          \n\t"	/* take 8 bytes */
    "  psadbw (%2,%4), %%mm1        \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  paddw %%mm1, %%mm7           \n\t"	/* accumulate difference... */

    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_3, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_u8_mmxext_4 (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  movq (%1), %%mm6             \n\t"
    "   movq (%1,%3,1), %%mm7          \n\t"
    "  psadbw (%2), %%mm6           \n\t"
    "   psadbw (%2,%4,1), %%mm7      \n\t"
    "  movq (%1,%3,2), %%mm0          \n\t"
    "   movq (%1,%3,4), %%mm1          \n\t"
    "  psadbw (%2,%4,2), %%mm0      \n\t"
    "   psadbw (%2,%4,4), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"

    "  lea (%1,%3,8), %1           \n\t"
    "   lea (%2,%4,8), %2           \n\t"
    "  neg %3\n\t"
    "   neg %4\n\t"
    "  lea (%1,%3), %1           \n\t"
    "   lea (%2,%4), %2           \n\t"

    "  movq (%1), %%mm0             \n\t"
    "   movq (%1,%3,1), %%mm1          \n\t"
    "  psadbw (%2), %%mm0           \n\t"
    "   psadbw (%2,%4,1), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%1,%3,2), %%mm0          \n\t"
    "   movq (%1,%3,4), %%mm1          \n\t"
    "  psadbw (%2,%4,2), %%mm0      \n\t"
    "   psadbw (%2,%4,4), %%mm1      \n\t"
    "  paddw %%mm0, %%mm6           \n\t"
    "   paddw %%mm1, %%mm7           \n\t"

    "  paddw %%mm6, %%mm7           \n\t"
    "  movd %%mm7, %0               \n\t"

    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2)
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad8x8_u8_mmxext_4, sad8x8_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad8x8_8xn_u8_psadbw (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2, int n)
{
  int n_tmp = n;
  uint32_t *dest_tmp = dest;
  uint8_t *src2_tmp = src2;
  uint8_t *src1_tmp = src1;

  __asm__ __volatile__ ("\n"
    "1:\n"
    "  movq (%[src1]), %%mm7             \n\t"
    "  psadbw (%[src2]), %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],1), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],1), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%[src1],%[sstr1],2), %%mm0          \n\t"
    "  psadbw (%[src2],%[sstr2],2), %%mm0      \n\t"
    "  paddw %%mm0, %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],4), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],4), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movd %%mm7, 0(%[dest])               \n\t"
    "  add %[sstr2],%[src2]\n\t"
    "  add $4, %[dest]\n\t"
    "  decl %[n]\n\t"
    "  jnz 1b\n\t"
    "  emms                         \n\t"
     : [src1] "+r" (src1_tmp), 
       [src2] "+r" (src2_tmp),
       [n] "+m" (n_tmp),
       [dest] "+r" (dest_tmp)
     : [sstr1] "r" ((ptrdiff_t)(sstr1)),
       [sstr2] "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );

  src1 = OIL_OFFSET(src1, 7 * sstr1);
  src2 = OIL_OFFSET(src2, 7 * sstr2);
  sstr1 = -sstr1;
  sstr2 = -sstr2;

  __asm__ __volatile__ ("\n"
    "1:\n"
    "  movq (%[src1]), %%mm7             \n\t"
    "  psadbw (%[src2]), %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],1), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],1), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq (%[src1],%[sstr1],2), %%mm0          \n\t"
    "  psadbw (%[src2],%[sstr2],2), %%mm0      \n\t"
    "  paddw %%mm0, %%mm7           \n\t"
    "   movq (%[src1],%[sstr1],4), %%mm1          \n\t"
    "   psadbw (%[src2],%[sstr2],4), %%mm1      \n\t"
    "   paddw %%mm1, %%mm7           \n\t"
    "  movq 0(%[dest]), %%mm1\n\t"
    "  paddd %%mm7, %%mm1\n\t"
    "  movq %%mm1, 0(%[dest])\n\t"
    "  sub %[sstr2],%[src2]\n\t"
    "  add $4, %[dest]\n\t"
    "  decl %[n]\n\t"
    "  jnz 1b\n\t"

    "  emms                         \n\t"
     : [src1] "+r" (src1), 
       [src2] "+r" (src2),
       [dest] "+r" (dest),
       [n] "+m" (n)
     : [sstr1] "r" ((ptrdiff_t)(sstr1)),
       [sstr2] "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
}
OIL_DEFINE_IMPL_FULL (sad8x8_8xn_u8_psadbw, sad8x8_8xn_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);


static void
sad12x12_u8_mmxext (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */
    "  pxor %%mm2, %%mm2            \n\t"
    "  pxor %%mm3, %%mm3            \n\t"

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  movq (%2), %%mm1             \n\t" \
    "  psadbw %%mm1, %%mm0          \n\t" \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  movd 8(%1), %%mm2            \n\t"	/* take 4 bytes */ \
    "  movd 8(%2), %%mm3            \n\t" \
    "  psadbw %%mm3, %%mm2          \n\t" \
    "  paddw %%mm2, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  movq (%2), %%mm1             \n\t"
    "  psadbw %%mm1, %%mm0          \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movd 8(%1), %%mm2            \n\t"	/* take 4 bytes */
    "  movd 8(%2), %%mm3            \n\t"
    "  psadbw %%mm3, %%mm2          \n\t"
    "  paddw %%mm2, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad12x12_u8_mmxext, sad12x12_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
sad16x16_u8_mmxext (uint32_t * dest, uint8_t * src1, int sstr1, uint8_t * src2,
    int sstr2)
{
  uint32_t diff;

  __asm__ __volatile__ (
    "  pxor %%mm7, %%mm7            \n\t" 	/* mm7 contains the result */

#define LOOP \
    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */ \
    "  movq (%2), %%mm1             \n\t" \
    "  psadbw %%mm1, %%mm0          \n\t" \
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */ \
    "  movq 8(%1), %%mm2            \n\t"	/* take 8 bytes */ \
    "  movq 8(%2), %%mm3            \n\t" \
    "  psadbw %%mm3, %%mm2          \n\t" \
    "  paddw %%mm2, %%mm7           \n\t"	/* accumulate difference... */ \
    "  add %3, %1                   \n\t"	/* Inc pointer into the new data */ \
    "  add %4, %2                   \n\t"	/* Inc pointer into ref data */

    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
    LOOP
#undef LOOP

    "  movq (%1), %%mm0             \n\t"	/* take 8 bytes */
    "  movq (%2), %%mm1             \n\t"
    "  psadbw %%mm1, %%mm0          \n\t"
    "  paddw %%mm0, %%mm7           \n\t"	/* accumulate difference... */
    "  movq 8(%1), %%mm2            \n\t"	/* take 8 bytes */
    "  movq 8(%2), %%mm3            \n\t"
    "  psadbw %%mm3, %%mm2          \n\t"
    "  paddw %%mm2, %%mm7           \n\t"	/* accumulate difference... */
    "  movd %%mm7, %0               \n\t"
    "  emms                         \n\t"

     : "=r" (diff),
       "+r" (src1), 
       "+r" (src2) 
     : "r" ((ptrdiff_t)(sstr1)),
       "r" ((ptrdiff_t)(sstr2))
     : "memory"
  );
  *dest = diff;
}
OIL_DEFINE_IMPL_FULL (sad16x16_u8_mmxext, sad16x16_u8, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

void
combine4_12xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    uint8_t *s3, int ss3,
    uint8_t *s4, int ss4,
    int16_t *s5_6, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 8(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      ::"r" (s5_6));

  for(j=0;j<n;j++){
    asm volatile ("\n"
#define DO_4(offset) \
        "  movd " #offset "(%1), %%mm0\n" \
        "  punpcklbw %%mm7, %%mm0\n" \
        "  pshufw $0x00, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm0\n" \
        "  movd " #offset "(%2), %%mm1\n" \
        "  punpcklbw %%mm7, %%mm1\n" \
        "  pshufw $0x55, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm1\n" \
        "  movd " #offset "(%3), %%mm2\n" \
        "  punpcklbw %%mm7, %%mm2\n" \
        "  pshufw $0xaa, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm2\n" \
        "  movd " #offset "(%4), %%mm3\n" \
        "  punpcklbw %%mm7, %%mm3\n" \
        "  pshufw $0xff, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm3\n" \
        "  paddw %%mm1, %%mm0\n" \
        "  paddw %%mm2, %%mm0\n" \
        "  paddw %%mm3, %%mm0\n" \
        "  paddw %%mm4, %%mm0\n" \
        "  psrlw $4, %%mm0\n" \
        "  packuswb %%mm0, %%mm0\n" \
        "  movd %%mm0, " #offset "(%0)\n"

        DO_4(0)
        DO_4(4)
        DO_4(8)

        :
        : "r" (d), "r" (s1), "r" (s2), "r" (s3), "r" (s4));

    s1 += ss1;
    s2 += ss2;
    s3 += ss3;
    s4 += ss4;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine4_12xn_u8_mmx, combine4_12xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine4_8xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    uint8_t *s3, int ss3,
    uint8_t *s4, int ss4,
    int16_t *s5_6, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 8(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      ::"r" (s5_6));

  for(j=0;j<n;j++){
    asm volatile ("\n"
        DO_4(0)
        DO_4(4)

        :
        : "r" (d), "r" (s1), "r" (s2), "r" (s3), "r" (s4));

    s1 += ss1;
    s2 += ss2;
    s3 += ss3;
    s4 += ss4;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine4_8xn_u8_mmx, combine4_8xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine4_16xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    uint8_t *s3, int ss3,
    uint8_t *s4, int ss4,
    int16_t *s5_6, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 8(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      ::"r" (s5_6));

  for(j=0;j<n;j++){
    asm volatile ("\n"
        DO_4(0)
        DO_4(4)
        DO_4(8)
        DO_4(12)

        :
        : "r" (d), "r" (s1), "r" (s2), "r" (s3), "r" (s4));

    s1 += ss1;
    s2 += ss2;
    s3 += ss3;
    s4 += ss4;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine4_16xn_u8_mmx, combine4_16xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine4_32xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    uint8_t *s3, int ss3,
    uint8_t *s4, int ss4,
    int16_t *s5_6, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 8(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      ::"r" (s5_6));

  for(j=0;j<n;j++){
    asm volatile ("\n"
        DO_4(0)
        DO_4(4)
        DO_4(8)
        DO_4(12)
        DO_4(16)
        DO_4(20)
        DO_4(24)
        DO_4(28)

        :
        : "r" (d), "r" (s1), "r" (s2), "r" (s3), "r" (s4));

    s1 += ss1;
    s2 += ss2;
    s3 += ss3;
    s4 += ss4;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine4_32xn_u8_mmx, combine4_32xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine2_12xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    int16_t *s3_4, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 4(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 6(%0), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      ::"r" (s3_4)
      :"ecx");

  for(j=0;j<n;j++){
    asm volatile ("\n"
#define COMBINE2_4(offset) \
        "  movd " #offset "(%1), %%mm0\n" \
        "  punpcklbw %%mm7, %%mm0\n" \
        "  pshufw $0x00, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm0\n" \
        "  movd " #offset "(%2), %%mm1\n" \
        "  punpcklbw %%mm7, %%mm1\n" \
        "  pshufw $0x55, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm1\n" \
        "  paddw %%mm1, %%mm0\n" \
        "  paddw %%mm4, %%mm0\n" \
        "  psrlw %%mm3, %%mm0\n" \
        "  packuswb %%mm0, %%mm0\n" \
        "  movd %%mm0, " #offset "(%0)\n"

        COMBINE2_4(0)
        COMBINE2_4(4)
        COMBINE2_4(8)

        :
        : "r" (d), "r" (s1), "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine2_12xn_u8_mmx, combine2_12xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine2_8xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    int16_t *s3_4, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 4(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 6(%0), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      ::"r" (s3_4)
      :"ecx");

  for(j=0;j<n;j++){
    asm volatile ("\n"
        COMBINE2_4(0)
        COMBINE2_4(4)

        :
        : "r" (d), "r" (s1), "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine2_8xn_u8_mmx, combine2_8xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
combine2_16xn_u8_mmx (uint8_t *d, int ds1,
    uint8_t *s1, int ss1,
    uint8_t *s2, int ss2,
    int16_t *s3_4, int n)
{
  int j;

  asm volatile ("\n"
      "  pxor %%mm7, %%mm7\n"
      "  movq 0(%0), %%mm6\n"
      "  movd 4(%0), %%mm4\n"
      "  pshufw $0x00, %%mm4, %%mm4\n"
      "  movzwl 6(%0), %%ecx\n"
      "  movd %%ecx, %%mm3\n"
      ::"r" (s3_4)
      :"ecx");

  for(j=0;j<n;j++){
    asm volatile ("\n"
        COMBINE2_4(0)
        COMBINE2_4(4)
        COMBINE2_4(8)
        COMBINE2_4(12)

        :
        : "r" (d), "r" (s1), "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (combine2_16xn_u8_mmx, combine2_16xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

#define AVG2_4(offset) \
        "  movd " #offset "(%1), %%mm0\n" \
        "  punpcklbw %%mm7, %%mm0\n" \
        "  pshufw $0x00, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm0\n" \
        "  movd " #offset "(%2), %%mm1\n" \
        "  punpcklbw %%mm7, %%mm1\n" \
        "  pshufw $0x55, %%mm6, %%mm5\n" \
        "  pmullw %%mm5, %%mm1\n" \
        "  paddw %%mm1, %%mm0\n" \
        "  paddw %%mm4, %%mm0\n" \
        "  psrlw %%mm3, %%mm0\n" \
        "  packuswb %%mm0, %%mm0\n" \
        "  movd %%mm0, " #offset "(%0)\n"
void
avg2_8xn_u8_mmx (uint8_t *d, int ds1, uint8_t *s1, int ss1,
    uint8_t *s2, int ss2, int n)
{
  int j;
  for(j=0;j<n;j++){
    asm volatile ("\n"
        "  movq 0(%[s1]), %%mm0\n"
        "  pavgb 0(%[s2]), %%mm0\n"
        "  movq %%mm0, 0(%[d])\n"
        :
        : [d] "r" (d), [s1] "r" (s1), [s2] "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (avg2_8xn_u8_mmx, avg2_8xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
avg2_12xn_u8_mmx (uint8_t *d, int ds1, uint8_t *s1, int ss1,
    uint8_t *s2, int ss2, int n)
{
  int j;
  for(j=0;j<n;j++){
    asm volatile ("\n"
        "  movq 0(%[s1]), %%mm0\n"
        "  pavgb 0(%[s2]), %%mm0\n"
        "  movq %%mm0, 0(%[d])\n"
        "  movq 4(%[s1]), %%mm0\n"
        "  pavgb 4(%[s2]), %%mm0\n"
        "  movq %%mm0, 4(%[d])\n"
        :
        : [d] "r" (d), [s1] "r" (s1), [s2] "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (avg2_12xn_u8_mmx, avg2_12xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
avg2_16xn_u8_mmx (uint8_t *d, int ds1, uint8_t *s1, int ss1,
    uint8_t *s2, int ss2, int n)
{
  int j;
  for(j=0;j<n;j++){
    asm volatile ("\n"
        "  movq 0(%[s1]), %%mm0\n"
        "  pavgb 0(%[s2]), %%mm0\n"
        "  movq %%mm0, 0(%[d])\n"
        "  movq 8(%[s1]), %%mm0\n"
        "  pavgb 8(%[s2]), %%mm0\n"
        "  movq %%mm0, 8(%[d])\n"
        :
        : [d] "r" (d), [s1] "r" (s1), [s2] "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (avg2_16xn_u8_mmx, avg2_16xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

void
avg2_32xn_u8_mmx (uint8_t *d, int ds1, uint8_t *s1, int ss1,
    uint8_t *s2, int ss2, int n)
{
  int j;
  for(j=0;j<n;j++){
    asm volatile ("\n"
        "  movq 0(%[s1]), %%mm0\n"
        "  pavgb 0(%[s2]), %%mm0\n"
        "  movq %%mm0, 0(%[d])\n"
        "  movq 8(%[s1]), %%mm0\n"
        "  pavgb 8(%[s2]), %%mm0\n"
        "  movq %%mm0, 8(%[d])\n"
        "  movq 16(%[s1]), %%mm0\n"
        "  pavgb 16(%[s2]), %%mm0\n"
        "  movq %%mm0, 16(%[d])\n"
        "  movq 24(%[s1]), %%mm0\n"
        "  pavgb 24(%[s2]), %%mm0\n"
        "  movq %%mm0, 24(%[d])\n"
        :
        : [d] "r" (d), [s1] "r" (s1), [s2] "r" (s2));

    s1 += ss1;
    s2 += ss2;
    d += ds1;
  }
  asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL (avg2_32xn_u8_mmx, avg2_32xn_u8, OIL_IMPL_FLAG_MMX|OIL_IMPL_FLAG_MMXEXT);

