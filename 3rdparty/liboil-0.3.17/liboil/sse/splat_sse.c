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

SSE_FUNCTION static void
splat_u32_ns_sse (uint32_t *dest, const uint32_t *param, int n)
{
  __m128i v;

  v = _mm_set1_epi32(*param);

  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *param;
  }
  for (; n >= 4; n -= 4) {
    _mm_store_si128((__m128i *)dest, v);
    dest += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *param;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u32_ns_sse, splat_u32_ns, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
splat_u32_ns_sse_unroll2 (uint32_t *dest, const uint32_t *param, int n)
{
  __m128i v;

  v = _mm_set1_epi32(*param);

  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *param;
  }
  for (; n >= 8; n -= 8) {
    _mm_store_si128((__m128i *)dest, v);
    _mm_store_si128((__m128i *)(dest + 4), v);
    dest += 8;
  }
  for (; n >= 4; n -= 4) {
    _mm_store_si128((__m128i *)dest, v);
    dest += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *param;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u32_ns_sse_unroll2, splat_u32_ns, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
splat_u8_ns_sse (uint8_t *dest, const uint8_t *param, int n)
{
  __m128i v;

  v = _mm_set1_epi8(*param);

  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *param;
  }
  for (; n >= 16; n -= 16) {
    _mm_store_si128((__m128i *)dest, v);
    dest += 16;
  }
  for (; n > 0; n--) {
    *dest++ = *param;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u8_ns_sse, splat_u8_ns, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
splat_u8_ns_sse_unroll2 (uint8_t *dest, const uint8_t *param, int n)
{
  __m128i v;

  v = _mm_set1_epi8(*param);

  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *param;
  }
  for (; n >= 32; n -= 32) {
    _mm_store_si128((__m128i *)dest, v);
    _mm_store_si128((__m128i *)(dest + 16), v);
    dest += 32;
  }
  for (; n >= 16; n -= 16) {
    _mm_store_si128((__m128i *)dest, v);
    dest += 16;
  }
  for (; n > 0; n--) {
    *dest++ = *param;
  }
}
OIL_DEFINE_IMPL_FULL (splat_u8_ns_sse_unroll2, splat_u8_ns, OIL_IMPL_FLAG_SSE2);
