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
#include <xmmintrin.h>

#ifdef HAVE_I386
#define SSE_FUNCTION __attribute__((force_align_arg_pointer))
#else
#define SSE_FUNCTION
#endif

SSE_FUNCTION static void
add_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ + *src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_add_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ + *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (add_f32_sse, add_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
add_f64_sse2 (double *dest, double *src1, double *src2, int n)
{
  __m128d xmm0, xmm1;
  while (((long)dest & 15) && (0 < n)) {
    *dest++ = *src1++ + *src2++;
    n--;
  }
  while (1 < n) {
    xmm0 = _mm_loadu_pd(src1);
    xmm1 = _mm_loadu_pd(src2);
    xmm0 = _mm_add_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
    src2 += 2;
    n -= 2;
  }
  while (0 < n) {
    *dest++ = *src1++ + *src2++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (add_f64_sse2, add_f64, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
add_f64_sse2_unroll (double *dest, double *src1, double *src2, int n)
{
  __m128d xmm0, xmm1;
  while (((long)dest & 15) && (0 < n)) {
    *dest++ = *src1++ + *src2++;
    n--;
  }
  while (3 < n) {
    xmm0 = _mm_loadu_pd(src1);
    xmm1 = _mm_loadu_pd(src2);
    xmm0 = _mm_add_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);

    xmm0 = _mm_loadu_pd(src1+2);
    xmm1 = _mm_loadu_pd(src2+2);
    xmm0 = _mm_add_pd(xmm0, xmm1);
    _mm_store_pd(dest+2, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
    n -= 4;
  }
  while (1 < n) {
    xmm0 = _mm_loadu_pd(src1);
    xmm1 = _mm_loadu_pd(src2);
    xmm0 = _mm_add_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
    src2 += 2;
    n -= 2;
  }
  while (0 < n) {
    *dest++ = *src1++ + *src2++;
    n--;
  }
}
OIL_DEFINE_IMPL_FULL (add_f64_sse2_unroll, add_f64, OIL_IMPL_FLAG_SSE2);

SSE_FUNCTION static void
subtract_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ - *src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_sub_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ - *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (subtract_f32_sse, subtract_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
multiply_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ * *src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_mul_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ * *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (multiply_f32_sse, multiply_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
divide_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ / *src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_div_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ / *src2++;
  }
}
OIL_DEFINE_IMPL_FULL (divide_f32_sse, divide_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
minimum_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1 < *src2 ? *src1 : *src2;
    src1++;
    src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_min_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1 < *src2 ? *src1 : *src2;
    src1++;
    src2++;
  }
}
OIL_DEFINE_IMPL_FULL (minimum_f32_sse, minimum_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
maximum_f32_sse (float *dest, float *src1, float *src2, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1 > *src2 ? *src1 : *src2;
    src1++;
    src2++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_loadu_ps(src1);
    xmm1 = _mm_loadu_ps(src2);
    xmm0 = _mm_max_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
    src2 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1 > *src2 ? *src1 : *src2;
    src1++;
    src2++;
  }
}
OIL_DEFINE_IMPL_FULL (maximum_f32_sse, maximum_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
inverse_f32_sse (float *dest, float *src1, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = 1.0 / *src1++;
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    /* While _mm_rcp_ps sounds promising, the results it gives are rather
     * different from the 1.0 / src1 reference implementation, so do that.
     */
    xmm0 = _mm_set_ps1(1.0);
    xmm1 = _mm_loadu_ps(src1);
    xmm0 = _mm_div_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = 1.0 / *src1++;
  }
}
OIL_DEFINE_IMPL_FULL (inverse_f32_sse, inverse_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
negative_f32_sse (float *dest, float *src1, int n)
{
  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = -(*src1++);
  }
  for (; n >= 4; n -= 4) {
    __m128 xmm0, xmm1;
    xmm0 = _mm_setzero_ps();
    xmm1 = _mm_loadu_ps(src1);
    xmm0 = _mm_sub_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = -(*src1++);
  }
}
OIL_DEFINE_IMPL_FULL (negative_f32_sse, negative_f32, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
scalaradd_f32_ns_sse (float *dest, float *src1, float *val, int n)
{
  __m128 xmm1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ + *val;
  }
  xmm1 = _mm_load_ps1(val);
  for (; n >= 4; n -= 4) {
    __m128 xmm0;
    xmm0 = _mm_loadu_ps(src1);
    xmm0 = _mm_add_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ + *val;
  }
}
OIL_DEFINE_IMPL_FULL (scalaradd_f32_ns_sse, scalaradd_f32_ns, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
scalarmultiply_f32_ns_sse (float *dest, float *src1, float *val, int n)
{
  __m128 xmm1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ * *val;
  }
  xmm1 = _mm_load_ps1(val);
  for (; n >= 4; n -= 4) {
    __m128 xmm0;
    xmm0 = _mm_loadu_ps(src1);
    xmm0 = _mm_mul_ps(xmm0, xmm1);
    _mm_store_ps(dest, xmm0);
    dest += 4;
    src1 += 4;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ * *val;
  }
}
OIL_DEFINE_IMPL_FULL (scalarmultiply_f32_ns_sse, scalarmultiply_f32_ns, OIL_IMPL_FLAG_SSE);

SSE_FUNCTION static void
scalarmultiply_f64_ns_sse2 (double *dest, double *src1, double *val, int n)
{
  __m128d xmm1;

  /* Initial operations to align the destination pointer */
  for (; ((long)dest & 15) && (n > 0); n--) {
    *dest++ = *src1++ * *val;
  }
  xmm1 = _mm_load1_pd(val);
  for (; n >= 2; n -= 2) {
    __m128d xmm0;
    xmm0 = _mm_loadu_pd(src1);
    xmm0 = _mm_mul_pd(xmm0, xmm1);
    _mm_store_pd(dest, xmm0);
    dest += 2;
    src1 += 2;
  }
  for (; n > 0; n--) {
    *dest++ = *src1++ * *val;
  }
}
OIL_DEFINE_IMPL_FULL (scalarmultiply_f64_ns_sse2, scalarmultiply_f64_ns, OIL_IMPL_FLAG_SSE2);

