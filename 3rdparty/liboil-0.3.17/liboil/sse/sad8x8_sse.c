/*
 * Copyright (c) 2005
 *	Eric Anholt.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilclasses.h>
#include <liboil/liboilfunction.h>
#include <emmintrin.h>

#ifdef HAVE_I386
#define SSE_FUNCTION __attribute__((force_align_arg_pointer))
#else
#define SSE_FUNCTION
#endif

#ifdef ENABLE_BROKEN_IMPLS
union m128_int {
  __m128i m128;
  uint32_t i[4];
  uint16_t s[8];
};

SSE_FUNCTION static void
sad8x8_u8_sse (uint32_t *dest, uint8_t *src1, int sstr1, uint8_t *src2,
    int sstr2)
{
  int i;
  __m128i sum = _mm_setzero_si128();
  union m128_int sumi;

  for (i = 0; i < 4; i++) {
    __m128i xmm0, xmm1, xmm2, xmm3;
    xmm0 = _mm_loadl_epi64((__m128i *)src1);
    xmm1 = _mm_loadl_epi64((__m128i *)(src1 + sstr1));
    xmm2 = _mm_loadl_epi64((__m128i *)src2);
    xmm3 = _mm_loadl_epi64((__m128i *)(src2 + sstr2));
    xmm0 = _mm_unpacklo_epi8(xmm0, xmm1);
    xmm2 = _mm_unpacklo_epi8(xmm2, xmm3);
    sum = _mm_add_epi64(sum, _mm_sad_epu8(xmm0, xmm2));
    src1 += 2 * sstr1;
    src2 += 2 * sstr2;
  }
  sumi.m128 = sum;
  *dest = sumi.i[0] + sumi.i[2];
}
OIL_DEFINE_IMPL_FULL_WRAPPER(sad8x8_u8_sse, sad8x8_u8, OIL_IMPL_FLAG_SSE2);
#endif

