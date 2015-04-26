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
#include <liboil/liboilcolorspace.h>

#ifdef HAVE_I386
#define SSE_FUNCTION __attribute__((force_align_arg_pointer))
#else
#define SSE_FUNCTION
#endif

#ifdef ENABLE_BROKEN_IMPLS

union m128_int {
  __m128i m128;
  uint64_t ull[2];
};

static const struct _SSEData {
  union m128_int sse_16xff;
  union m128_int sse_8x0080;
} c = {
    .sse_16xff.ull =	{0xffffffffffffffffULL, 0xffffffffffffffffULL},
    .sse_8x0080.ull =	{0x0080008000800080ULL, 0x0080008000800080ULL},
};

#define MC(x) (c.sse_##x.m128)

/* non-SSE2 compositing support */
#define COMPOSITE_OVER(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))
#define COMPOSITE_ADD(d,s) oil_clamp_255((d) + (s))
#define COMPOSITE_IN(s,m) oil_muldiv_255((s),(m))

/* This SSE2 code is based around operations on four pixels at a time.  The
 * exception is muldiv_255_sse2, which needs to expand the four pixels into
 * 2 sets of 2 pixels at 16 bits per channel each, for the purpose of doing
 * the appropriate rounding on division.
 */

/* Shuffles the given value such that the alpha for each pixel appears in each
 * channel of the pixel.
 */
SSE_FUNCTION static inline __m128i
argb_A_sse2(__m128i a)
{
#if 0
  /* Shift the alpha channel of each pixel into the low byte */
  a = _mm_srli_epi32(a, 24);
  /* Now, shift and or so we can get it into all the channels */
  a = _mm_or_si128(a, _mm_slli_epi32(a, 8));
  a = _mm_or_si128(a, _mm_slli_epi32(a, 16));
  return a;
#else
  /* Move the alpha channel into the low byte */
  a = _mm_srli_epi32(a, 24);
  /* Pack our four alpha channels down into the lower 32 bits */
  a = _mm_packus_epi16(a, _mm_setzero_si128());
  a = _mm_packus_epi16(a, _mm_setzero_si128());
  /* And expand it back out into four pixels of all channels the same */
  a = _mm_unpacklo_epi8(a, a);
  return _mm_unpacklo_epi16(a, a);
#endif
}

/* Multiplies the unpacked 16-bits-per-channel pixel data in a
 * channel-by-channel by b, and divides the result by 255, with rounding.
 */
SSE_FUNCTION static inline __m128i
inner_muldiv_255_sse2(__m128i a, __m128i b)
{
  __m128i ret;
  __m128i roundconst = MC(8x0080);

  ret = _mm_mullo_epi16(a, b);
  ret = _mm_adds_epu16(ret, roundconst);
  ret = _mm_adds_epu16(ret, _mm_srli_epi16(ret, 8));
  ret = _mm_srli_epi16(ret, 8);

  return ret;
}

SSE_FUNCTION static inline __m128i
muldiv_255_sse2(__m128i a, __m128i b)
{
  __m128i alow, blow, ahigh, bhigh, low, high;

  alow = _mm_unpacklo_epi8(a, _mm_setzero_si128());
  blow = _mm_unpacklo_epi8(b, _mm_setzero_si128());
  ahigh = _mm_unpackhi_epi8(a, _mm_setzero_si128());
  bhigh = _mm_unpackhi_epi8(b, _mm_setzero_si128());
  low = inner_muldiv_255_sse2(alow, blow);
  high = inner_muldiv_255_sse2(ahigh, bhigh);
  return _mm_packus_epi16(low, high);
}

SSE_FUNCTION static inline __m128i
negate_argb_sse2(__m128i a)
{
  return _mm_xor_si128(a, MC(16xff));
}

SSE_FUNCTION static inline __m128i
load_argb_sse2(const uint32_t *src)
{
  return _mm_loadu_si128((__m128i *)src);
}

SSE_FUNCTION static inline __m128i
set1_argb_sse2(uint32_t src)
{
  return _mm_set1_epi32(src);
}

SSE_FUNCTION static inline __m128i
load_u8_mask(const uint8_t *m)
{
  __m128i a;
  a = _mm_cvtsi32_si128(*(uint32_t *)m);
  a = _mm_unpacklo_epi8(a, a);
  a = _mm_unpacklo_epi16(a, a);
  return a;
}

SSE_FUNCTION static inline __m128i
set1_u8_mask(uint8_t m)
{
  return _mm_set1_epi8(m);
}

SSE_FUNCTION static void
store_argb_sse2(uint32_t *dest, __m128i pix)
{
  _mm_store_si128((__m128i *)dest, pix);
}

SSE_FUNCTION static __m128i 
over_argb_sse2(__m128i dest, __m128i src, __m128i srca)
{
  return _mm_adds_epu8(src, muldiv_255_sse2(dest, negate_argb_sse2(srca)));
}

SSE_FUNCTION static void
composite_in_argb_sse (uint32_t *dest, const uint32_t *src, const uint8_t *mask,
    int n)
{
  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t s = *src++;
    uint8_t m = *mask++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(s), m),
	COMPOSITE_IN(oil_argb_R(s), m),
	COMPOSITE_IN(oil_argb_G(s), m),
	COMPOSITE_IN(oil_argb_B(s), m));
  }
  for (; n >= 4; n -= 4) {
    __m128i s, m;
    s = load_argb_sse2(src);
    m = load_u8_mask(mask);
    store_argb_sse2(dest, muldiv_255_sse2(s, m));
    src += 4;
    mask += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t s = *src++;
    uint8_t m = *mask++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(s), m),
	COMPOSITE_IN(oil_argb_R(s), m),
	COMPOSITE_IN(oil_argb_G(s), m),
	COMPOSITE_IN(oil_argb_B(s), m));
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_argb_sse, composite_in_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_argb_const_src_sse (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i s;

  s = set1_argb_sse2(*src);

  for (; ((long)dest & 15) && (n > 0); n--) {
    uint8_t m = *mask++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(*src), m),
	COMPOSITE_IN(oil_argb_R(*src), m),
	COMPOSITE_IN(oil_argb_G(*src), m),
	COMPOSITE_IN(oil_argb_B(*src), m));
  }
  for (; n >= 4; n -= 4) {
    __m128i m;
    m = load_u8_mask(mask);
    store_argb_sse2(dest, muldiv_255_sse2(s, m));
    mask += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint8_t m = *mask++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(*src), m),
	COMPOSITE_IN(oil_argb_R(*src), m),
	COMPOSITE_IN(oil_argb_G(*src), m),
	COMPOSITE_IN(oil_argb_B(*src), m));
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_argb_const_src_sse,
    composite_in_argb_const_src, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_argb_const_mask_sse (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i m;

  m = set1_u8_mask(*mask);

  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t s = *src++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(s), mask[0]),
	COMPOSITE_IN(oil_argb_R(s), mask[0]),
	COMPOSITE_IN(oil_argb_G(s), mask[0]),
	COMPOSITE_IN(oil_argb_B(s), mask[0]));
  }
  for (; n >= 4; n -= 4) {
    __m128i s;
    s = load_argb_sse2(src);
    store_argb_sse2(dest,  muldiv_255_sse2(s, m));
    src += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t s = *src++;

    *dest++ = oil_argb(
	COMPOSITE_IN(oil_argb_A(s), mask[0]),
	COMPOSITE_IN(oil_argb_R(s), mask[0]),
	COMPOSITE_IN(oil_argb_G(s), mask[0]),
	COMPOSITE_IN(oil_argb_B(s), mask[0]));
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_argb_const_mask_sse,
    composite_in_argb_const_mask, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_over_argb_sse (uint32_t *dest, const uint32_t *src, int n)
{
  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t d = *dest, s = *src++;
    uint8_t srca = oil_argb_A(s);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(s), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(s), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(s), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(s), srca));
    *dest++ = d;
  }
  for (; n >= 4; n -= 4) {
    __m128i d, s;
    s = load_argb_sse2(src);
    d = over_argb_sse2(*(__m128i *)dest, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src++;
    uint8_t srca = oil_argb_A(s);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(s), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(s), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(s), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(s), srca));
    *dest++ = d;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_over_argb_sse, composite_over_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_over_argb_const_src_sse (uint32_t *dest, const uint32_t *src, int n)
{
  __m128i s, sa;
  uint32_t srca;

  srca = oil_argb_A(*src);
  s = set1_argb_sse2(*src);
  sa = negate_argb_sse2(argb_A_sse2(s));
  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t d = *dest;
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(*src), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(*src), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(*src), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(*src), srca));
    *dest++ = d;
  }
  for (; n >= 4; n -= 4) {
    __m128i d;
    d = _mm_adds_epu8(s, muldiv_255_sse2(*(__m128i *)dest, sa));
    store_argb_sse2(dest, d);
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t d = *dest;
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(*src), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(*src), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(*src), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(*src), srca));
    *dest++ = d;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_over_argb_const_src_sse,
    composite_over_argb_const_src, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_sse (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t d = *dest, s = *src++, m = *mask++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(s), m),
        COMPOSITE_IN(oil_argb_R(s), m),
        COMPOSITE_IN(oil_argb_G(s), m),
        COMPOSITE_IN(oil_argb_B(s), m));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
  for (; n >= 4; n -= 4) {
    __m128i d, s, m;
    s = load_argb_sse2(src);
    m = load_u8_mask(mask);
    s = muldiv_255_sse2(s, m);
    d = over_argb_sse2(*(__m128i *)dest, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 4;
    mask += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src++, m = *mask++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(s), m),
        COMPOSITE_IN(oil_argb_R(s), m),
        COMPOSITE_IN(oil_argb_G(s), m),
        COMPOSITE_IN(oil_argb_B(s), m));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_over_argb_sse, composite_in_over_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_const_src_sse (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i s;

  s = set1_argb_sse2(*src);

  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t d = *dest, m = *mask++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(*src), m),
        COMPOSITE_IN(oil_argb_R(*src), m),
        COMPOSITE_IN(oil_argb_G(*src), m),
        COMPOSITE_IN(oil_argb_B(*src), m));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
  for (; n >= 4; n -= 4) {
    __m128i d, color, m;
    m = load_u8_mask(mask);
    color = muldiv_255_sse2(s, m);
    d = over_argb_sse2(*(__m128i *)dest, color, argb_A_sse2(color));
    store_argb_sse2(dest, d);
    mask += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t d = *dest, m = *mask++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(*src), m),
        COMPOSITE_IN(oil_argb_R(*src), m),
        COMPOSITE_IN(oil_argb_G(*src), m),
        COMPOSITE_IN(oil_argb_B(*src), m));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_over_argb_const_src_sse,
    composite_in_over_argb_const_src, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_const_mask_sse (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i m;

  m = set1_u8_mask(*mask);

  for (; ((long)dest & 15) && (n > 0); n--) {
    uint32_t d = *dest, s = *src++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(s), *mask),
        COMPOSITE_IN(oil_argb_R(s), *mask),
        COMPOSITE_IN(oil_argb_G(s), *mask),
        COMPOSITE_IN(oil_argb_B(s), *mask));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
  for (; n >= 4; n -= 4) {
    __m128i d, s;
    s = load_argb_sse2(src);
    s = muldiv_255_sse2(s, m);
    d = over_argb_sse2(*(__m128i *)dest, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src++, color;
    uint8_t srca;

    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(s), *mask),
        COMPOSITE_IN(oil_argb_R(s), *mask),
        COMPOSITE_IN(oil_argb_G(s), *mask),
        COMPOSITE_IN(oil_argb_B(s), *mask));
    srca = oil_argb_A(color);
    d = oil_argb(
	COMPOSITE_OVER(oil_argb_A(d), oil_argb_A(color), srca),
	COMPOSITE_OVER(oil_argb_R(d), oil_argb_R(color), srca),
	COMPOSITE_OVER(oil_argb_G(d), oil_argb_G(color), srca),
	COMPOSITE_OVER(oil_argb_B(d), oil_argb_B(color), srca));
    *dest++ = d;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_in_over_argb_const_mask_sse,
    composite_in_over_argb_const_mask, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_over_u8_sse (uint8_t *dest, const uint8_t *src, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest = COMPOSITE_OVER(*dest, *src, *src);
    src++;
    dest++;
  }
  /* over_u8 can be dealt with using our argb code, with srca = s */
  for (; n >= 16; n -= 16) {
    __m128i d, s;
    d = *(__m128i *)dest;
    s = load_argb_sse2((uint32_t *)src);
    store_argb_sse2((uint32_t *)dest, over_argb_sse2(d, s, s));
    src += 16;
    dest += 16;
  }
  for (; n > 0; n--) {
    *dest = COMPOSITE_OVER(*dest, *src, *src);
    src++;
    dest++;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER (composite_over_u8_sse, composite_over_u8,
    OIL_IMPL_FLAG_SSE2);

#endif

