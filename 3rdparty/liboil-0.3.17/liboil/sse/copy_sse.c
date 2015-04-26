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
copy_u8_sse (uint8_t *dest, const uint8_t *src, int n)
{
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src++;
  }
  for (; n >= 16; n -= 16) {
    _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
    src += 16;
    dest += 16;
  }
  for (; n > 0; n--) {
    *dest++ = *src++;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_sse, copy_u8, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
copy_u8_sse_unroll2 (uint8_t *dest, const uint8_t *src, int n)
{
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src++;
  }
  for (; n >= 32; n -= 32) {
    _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
    _mm_store_si128((__m128i *)(dest + 16), _mm_loadu_si128((__m128i *)(src + 16)));
    src += 32;
    dest += 32;
  }
  if (n >= 16) {
    _mm_store_si128((__m128i *)dest, _mm_loadu_si128((__m128i *)src));
    src += 16;
    dest += 16;
    n -= 16;
  }
  for (; n > 0; n--) {
    *dest++ = *src++;
  }
}
OIL_DEFINE_IMPL_FULL (copy_u8_sse_unroll2, copy_u8, OIL_IMPL_FLAG_SSE2);
