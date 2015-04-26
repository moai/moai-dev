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

/* non-SSE2 compositing support */
#define COMPOSITE_OVER(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))
#define COMPOSITE_ADD(d,s) oil_clamp_255((d) + (s))
#define COMPOSITE_IN(s,m) oil_muldiv_255((s),(m))

/* rgba values in SSE2 code will be unpacked as 16-bit integers per channel with
 * the channel value in the low byte.  This means 2 pixels per pass.
 */

#ifdef ENABLE_BROKEN_IMPLS

union m128_int {
  __m128i m128;
  uint64_t ull[2];
};

static const struct _SSEData {
  union m128_int sse_8x00ff;
  union m128_int sse_8x0080;
} c = {
    .sse_8x00ff.ull =	{0x00ff00ff00ff00ffULL, 0x00ff00ff00ff00ffULL},
    .sse_8x0080.ull =	{0x0080008000800080ULL, 0x0080008000800080ULL},
};

#define MC(x) (c.sse_##x.m128)

/* Shuffles the given value such that the alpha for each pixel appears in each
 * channel of the pixel.
 */
SSE_FUNCTION static inline __m128i
argb_A_sse2(__m128i a)
{
  a = _mm_shufflehi_epi16(a, _MM_SHUFFLE(3,3,3,3));
  a = _mm_shufflelo_epi16(a, _MM_SHUFFLE(3,3,3,3));
  return a;
}

/* Multiplies the pixel data in a channel-by-channel by b, and divides the
 * result by 255, with rounding.
 */
SSE_FUNCTION static inline __m128i
muldiv_255_sse2(__m128i a, __m128i b)
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
negate_argb_sse2(__m128i a)
{
  return _mm_xor_si128(a, MC(8x00ff));
}

/* Loads the 2 (unaligned) pixels at *src into unpacked SSE2 registers */
SSE_FUNCTION static inline __m128i
load_argb_sse2(const uint32_t *src)
{
  __m128i pix;

  pix = _mm_loadl_epi64((__m128i *)src);
  pix = _mm_unpacklo_epi8(pix, _mm_setzero_si128());
  return pix;
}

SSE_FUNCTION static inline __m128i
set1_argb_sse2(uint32_t src)
{
  __m128i pix;

  pix = _mm_set1_epi32(src);
  pix = _mm_unpacklo_epi8(pix, _mm_setzero_si128());
  return pix;
}

SSE_FUNCTION static inline __m128i
load_u8_mask(const uint8_t *m)
{
  return _mm_unpacklo_epi64(_mm_set1_epi16(m[0]), _mm_set1_epi16(m[1]));
}

SSE_FUNCTION static inline __m128i
set1_u8_mask(uint8_t m)
{
  return _mm_unpacklo_epi8(_mm_set1_epi8(m), _mm_setzero_si128());
}

/* Stores the 2 unpacked pixels in pix into the (unaligned) *dest */
SSE_FUNCTION static void
store_argb_sse2(uint32_t *dest, __m128i pix)
{
  pix = _mm_packus_epi16(pix, pix);
  _mm_storel_epi64((__m128i *)dest, pix);
}

SSE_FUNCTION static __m128i 
over_argb_sse2(__m128i dest, __m128i src, __m128i srca)
{
  return _mm_adds_epu8(src, muldiv_255_sse2(dest, negate_argb_sse2(srca)));
}

SSE_FUNCTION static void
composite_in_argb_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  for (; n >= 2; n -= 2) {
    __m128i s, m;
    s = load_argb_sse2(src);
    m = load_u8_mask(mask);
    store_argb_sse2(dest, muldiv_255_sse2(s, m));
    src += 2;
    mask += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_argb_sse_2pix, composite_in_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_argb_const_src_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i s;

  s = set1_argb_sse2(*src);

  for (; n >= 2; n -= 2) {
    __m128i m;
    m = load_u8_mask(mask);
    store_argb_sse2(dest, muldiv_255_sse2(s, m));
    mask += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_argb_const_src_sse_2pix,
    composite_in_argb_const_src, OIL_IMPL_FLAG_SSE2);

#ifdef SSE_ALIGN
SSE_FUNCTION static void
composite_in_argb_const_mask_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i m;

  m = set1_u8_mask(*mask);

  for (; n >= 2; n -= 2) {
    __m128i s;
    s = load_argb_sse2(src);
    store_argb_sse2(dest,  muldiv_255_sse2(s, m));
    src += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_argb_const_mask_sse_2pix,
    composite_in_argb_const_mask, OIL_IMPL_FLAG_SSE2);
#endif

SSE_FUNCTION static void
composite_over_argb_sse_2pix (uint32_t *dest, const uint32_t *src, int n)
{
  for (; n >= 2; n -= 2) {
    __m128i d, s;
    s = load_argb_sse2(src);
    d = load_argb_sse2(dest);
    d = over_argb_sse2(d, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_over_argb_sse_2pix, composite_over_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_over_argb_const_src_sse_2pix (uint32_t *dest, const uint32_t *src,
    int n)
{
  __m128i s, sa;
  uint32_t srca;

  srca = oil_argb_A(*src);
  s = set1_argb_sse2(*src);
  sa = negate_argb_sse2(argb_A_sse2(s));
  for (; n >= 2; n -= 2) {
    __m128i d;
    d = load_argb_sse2(dest);
    d = _mm_adds_epu8(s, muldiv_255_sse2(d, sa));
    store_argb_sse2(dest, d);
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_over_argb_const_src_sse_2pix,
    composite_over_argb_const_src, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  for (; n >= 2; n -= 2) {
    __m128i d, s, m;
    s = load_argb_sse2(src);
    m = load_u8_mask(mask);
    d = load_argb_sse2(dest);
    s = muldiv_255_sse2(s, m);
    d = over_argb_sse2(d, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 2;
    mask += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_over_argb_sse_2pix, composite_in_over_argb,
    OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_const_src_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i s;

  s = set1_argb_sse2(*src);

  for (; n >= 2; n -= 2) {
    __m128i d, color, m;
    m = load_u8_mask(mask);
    d = load_argb_sse2(dest);
    color = muldiv_255_sse2(s, m);
    d = over_argb_sse2(d, color, argb_A_sse2(color));
    store_argb_sse2(dest, d);
    mask += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_over_argb_const_src_sse_2pix,
    composite_in_over_argb_const_src, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_in_over_argb_const_mask_sse_2pix (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  __m128i m;

  m = set1_u8_mask(*mask);

  for (; n >= 2; n -= 2) {
    __m128i d, s;
    s = load_argb_sse2(src);
    d = load_argb_sse2(dest);
    s = muldiv_255_sse2(s, m);
    d = over_argb_sse2(d, s, argb_A_sse2(s));
    store_argb_sse2(dest, d);
    src += 2;
    dest += 2;
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
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_in_over_argb_const_mask_sse_2pix,
    composite_in_over_argb_const_mask, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
composite_over_u8_sse_2pix (uint8_t *dest, const uint8_t *src, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest = COMPOSITE_OVER(*dest, *src, *src);
    src++;
    dest++;
  }
  /* over_u8 can be dealt with using our argb code, with srca = s */
  for (; n >= 8; n -= 8) {
    __m128i d, s;
    d = load_argb_sse2((uint32_t *)dest);
    s = load_argb_sse2((uint32_t *)src);
    store_argb_sse2((uint32_t *)dest, over_argb_sse2(d, s, s));
    src += 8;
    dest += 8;
  }
  for (; n > 0; n--) {
    *dest = COMPOSITE_OVER(*dest, *src, *src);
    src++;
    dest++;
  }
}
OIL_DEFINE_IMPL_FULL_WRAPPER(composite_over_u8_sse_2pix, composite_over_u8,
    OIL_IMPL_FLAG_SSE2);
#endif

