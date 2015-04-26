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
#include <mmintrin.h>

#ifdef ENABLE_BROKEN_IMPLS
union m64_int {
  __m64 m64;
  uint64_t ull;
};

static const struct _MMXData {
  union m64_int mmx_4x0080;
} c = {
    .mmx_4x0080.ull =	0x0080008000800080ULL,
};

#define MC(x) (c.mmx_##x.m64)

static void
recon8x8_intra_mmx (uint8_t *dest, int ds, int16_t *change)
{
  int i;
  __m64 offset = MC(4x0080);

  for (i = 8; i; i--) {
    __m64 mm0, mm1, c0, c1;
    c0 = ((__m64 *)change)[0];
    c1 = ((__m64 *)change)[1];
    mm0 = _mm_adds_pi16(c0, offset);
    mm1 = _mm_adds_pi16(c1, offset);
    *(__m64 *)dest = _mm_packs_pu16(mm0, mm1);

    dest += ds;
    change += 8;
  }
  _mm_empty();
}
OIL_DEFINE_IMPL_FULL (recon8x8_intra_mmx, recon8x8_intra, OIL_IMPL_FLAG_MMX);
#endif

static void
recon8x8_inter_mmx (uint8_t *dest, int ds, uint8_t *src, int ss,
    int16_t *change, int dss)
{
  int i;

  for (i = 8; i; i--) {
    __m64 mm0, mm1, c0, c1;
    c0 = ((__m64 *)change)[0];
    c1 = ((__m64 *)change)[1];
    mm0 = _mm_unpacklo_pi8(*(__m64 *)src, _mm_setzero_si64());
    mm1 = _mm_unpackhi_pi8(*(__m64 *)src, _mm_setzero_si64());
    mm0 = _mm_adds_pi16(mm0, c0);
    mm1 = _mm_adds_pi16(mm1, c1);
    *(__m64 *)dest = _mm_packs_pu16(mm0, mm1);

    change += 8;
    dest += ds;
    src += ss;
  }
  _mm_empty();
}
OIL_DEFINE_IMPL_FULL (recon8x8_inter_mmx, recon8x8_inter, OIL_IMPL_FLAG_MMX);

static void
recon8x8_inter2_mmx (uint8_t *dest, int ds, uint8_t *s1, int ss1, uint8_t *s2,
    int ss2, int16_t *change)
{
  int i;

  for (i = 8; i; i--) {
    __m64 mm0, mm1, c0, c1;
    mm0 = _mm_adds_pu16(
	_mm_unpacklo_pi8(*(__m64 *)s1, _mm_setzero_si64()),
	_mm_unpacklo_pi8(*(__m64 *)s2, _mm_setzero_si64()));
    mm1 = _mm_adds_pu16(
	_mm_unpackhi_pi8(*(__m64 *)s1, _mm_setzero_si64()),
	_mm_unpackhi_pi8(*(__m64 *)s2, _mm_setzero_si64()));
    c0 = ((__m64 *)change)[0];
    c1 = ((__m64 *)change)[1];
    mm0 = _mm_srli_pi16(mm0, 1);
    mm1 = _mm_srli_pi16(mm1, 1);
    mm0 = _mm_adds_pi16(mm0, c0);
    mm1 = _mm_adds_pi16(mm1, c1);
    *(__m64 *)dest = _mm_packs_pu16(mm0, mm1);

    change += 8;
    dest += ds;
    s1 += ss1;
    s2 += ss2;
  }
  _mm_empty();
}
OIL_DEFINE_IMPL_FULL (recon8x8_inter2_mmx, recon8x8_inter2, OIL_IMPL_FLAG_MMX);
