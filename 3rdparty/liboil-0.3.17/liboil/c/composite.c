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
#include <liboil/liboilcolorspace.h>

#define COMPOSITE_ADD(d,s) oil_clamp_255((d) + (s))
#define COMPOSITE_OVER(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))

static void
composite_in_argb_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  for (; n > 0; n--) {
    uint32_t src1, src2;
    uint8_t m = *mask++;

    /* Split the pixel into two sets of two channels, and multiply by the
     * mask.
     */
    src1 = *src & 0x00ff00ff;
    src1 *= m;
    src1 += 0x00800080;
    src1 += (src1 >> 8) & 0x00ff00ff;
    src1 >>= 8;
    src1 &= 0x00ff00ff;

    src2 = (*src >> 8) & 0x00ff00ff;
    src2 *= m;
    src2 += 0x00800080;
    src2 += (src2 >> 8) & 0x00ff00ff;
    src2 &= 0xff00ff00;

    *dest++ = src1 | src2;
    src++;
  }
}
OIL_DEFINE_IMPL (composite_in_argb_fast, composite_in_argb);

static void
composite_in_argb_const_src_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  uint32_t src1, src2;

  src1 = *src & 0x00ff00ff;
  src2 = (*src >> 8) & 0x00ff00ff;

  for (; n > 0; n--) {
    uint32_t temp1, temp2;
    uint8_t m = *mask++;

    /* Split the pixel into two sets of two channels, and multiply by the
     * mask.
     */
    temp1 = src1 * m;
    temp1 += 0x00800080;
    temp1 += (temp1 >> 8) & 0x00ff00ff;
    temp1 >>= 8;
    temp1 &= 0x00ff00ff;

    temp2 = src2 * m;
    temp2 += 0x00800080;
    temp2 += (temp2 >> 8) & 0x00ff00ff;
    temp2 &= 0xff00ff00;

    *dest++ = temp1 | temp2;
  }
}
OIL_DEFINE_IMPL (composite_in_argb_const_src_fast, composite_in_argb_const_src);

static void
composite_in_argb_const_mask_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  uint8_t m = *mask;

  for (; n > 0; n--) {
    uint32_t src1, src2;

    /* Split the pixel into two sets of two channels, and multiply by the
     * mask.
     */
    src1 = *src & 0x00ff00ff;
    src1 *= m;
    src1 += 0x00800080;
    src1 += (src1 >> 8) & 0x00ff00ff;
    src1 >>= 8;
    src1 &= 0x00ff00ff;

    src2 = (*src >> 8) & 0x00ff00ff;
    src2 *= m;
    src2 += 0x00800080;
    src2 += (src2 >> 8) & 0x00ff00ff;
    src2 &= 0xff00ff00;

    *dest++ = src1 | src2;
    src++;
  }
}
OIL_DEFINE_IMPL (composite_in_argb_const_mask_fast,
    composite_in_argb_const_mask);

static void
composite_over_argb_fast (uint32_t *dest, const uint32_t *src, int n)
{
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src, sa;
    uint32_t s1, s2, d1, d2;

    sa = ~s >> 24;

    s1 = s & 0x00ff00ff;
    d1 = d & 0x00ff00ff;
    d1 *= sa;
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    s2 = (s >> 8) & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;
    d2 *= sa;
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
    src++;
  }
}
OIL_DEFINE_IMPL (composite_over_argb_fast, composite_over_argb);

static void
composite_over_argb_const_src_fast (uint32_t *dest, const uint32_t *src, int n)
{
  uint32_t s = *src;
  uint32_t sa, s1, s2;
  sa = ~s >> 24;

  s1 = s & 0x00ff00ff;
  s2 = (s >> 8) & 0x00ff00ff;

  for (; n > 0; n--) {
    uint32_t d = *dest;
    uint32_t d1, d2;

    d1 = d & 0x00ff00ff;
    d1 *= sa;
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 = (d >> 8) & 0x00ff00ff;
    d2 *= sa;
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
    src++;
  }
}
OIL_DEFINE_IMPL (composite_over_argb_const_src_fast,
    composite_over_argb_const_src);

static void
composite_add_argb_fast (uint32_t *dest, const uint32_t *src, int n)
{
  for (; n > 0; n--) {
    uint32_t s = *src++, d = *dest;
    uint32_t s1, s2, d1, d2;

    s1 = s & 0x00ff00ff;
    s2 = (s >> 8) & 0x00ff00ff;
    d1 = d & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;

    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
  }
}
OIL_DEFINE_IMPL (composite_add_argb_fast, composite_add_argb);

static void
composite_add_argb_const_src_fast (uint32_t *dest, const uint32_t *src, int n)
{
  uint32_t s1, s2;

  s1 = *src & 0x00ff00ff;
  s2 = (*src >> 8) & 0x00ff00ff;
  for (; n > 0; n--) {
    uint32_t d = *dest;
    uint32_t d1, d2;

    d1 = d & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;

    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
  }
}
OIL_DEFINE_IMPL (composite_add_argb_const_src_fast,
    composite_add_argb_const_src);

static void
composite_in_over_argb_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src++;
    uint32_t s1, s2, d1, d2, sa;
    uint8_t m = *mask++;

    s1 = s & 0x00ff00ff;
    s2 = (s >> 8) & 0x00ff00ff;

    /* in */
    s1 *= m;
    s1 += 0x00800080;
    s1 += (s1 >> 8) & 0x00ff00ff;
    s1 >>= 8;
    s1 &= 0x00ff00ff;

    s2 *= m;
    s2 += 0x00800080;
    s2 += (s2 >> 8) & 0x00ff00ff;
    s2 >>= 8;
    s2 &= 0x00ff00ff;

    /* over */
    sa = (~s2 >> 16) & 0xff;

    d1 = d & 0x00ff00ff;
    d1 *= sa;
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 = (d >> 8) & 0x00ff00ff;
    d2 *= sa;
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
  }
}
OIL_DEFINE_IMPL (composite_in_over_argb_fast, composite_in_over_argb);

static void
composite_in_over_argb_const_src_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  uint32_t s = *src;
  uint32_t s1, s2;

  s1 = s & 0x00ff00ff;
  s2 = (s >> 8) & 0x00ff00ff;

  for (; n > 0; n--) {
    uint32_t d = *dest;
    uint32_t temp1, temp2, d1, d2, sa;
    uint8_t m = *mask++;

    /* in */
    temp1 = s1 * m;
    temp1 += 0x00800080;
    temp1 += (temp1 >> 8) & 0x00ff00ff;
    temp1 >>= 8;
    temp1 &= 0x00ff00ff;

    temp2 = s2 * m;
    temp2 += 0x00800080;
    temp2 += (temp2 >> 8) & 0x00ff00ff;
    temp2 >>= 8;
    temp2 &= 0x00ff00ff;

    /* over */
    sa = (~temp2 >> 16) & 0xff;

    d1 = d & 0x00ff00ff;
    d1 *= sa;
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += temp1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 = (d >> 8) & 0x00ff00ff;
    d2 *= sa;
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += temp2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
  }
}
OIL_DEFINE_IMPL (composite_in_over_argb_const_src_fast,
    composite_in_over_argb_const_src);

static void
composite_in_over_argb_const_mask_fast (uint32_t *dest, const uint32_t *src,
    const uint8_t *mask, int n)
{
  uint8_t m = *mask;
  for (; n > 0; n--) {
    uint32_t d = *dest, s = *src++;
    uint32_t s1, s2, d1, d2, sa;

    s1 = s & 0x00ff00ff;
    s2 = (s >> 8) & 0x00ff00ff;

    /* in */
    s1 *= m;
    s1 += 0x00800080;
    s1 += (s1 >> 8) & 0x00ff00ff;
    s1 >>= 8;
    s1 &= 0x00ff00ff;

    s2 *= m;
    s2 += 0x00800080;
    s2 += (s2 >> 8) & 0x00ff00ff;
    s2 >>= 8;
    s2 &= 0x00ff00ff;

    /* over */
    sa = (~s2 >> 16) & 0xff;

    d1 = d & 0x00ff00ff;
    d1 *= sa;
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 = (d >> 8) & 0x00ff00ff;
    d2 *= sa;
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *dest++ = d1 | (d2 << 8);
  }
}
OIL_DEFINE_IMPL (composite_in_over_argb_const_mask_fast,
    composite_in_over_argb_const_mask);

#ifdef HAVE_UNALIGNED_ACCESS
static void
composite_add_u8_fast (uint8_t *dest, const uint8_t *src, int n)
{
  for (; n > 3; n-= 4) {
    uint32_t s = *(uint32_t *)src, d = *(uint32_t *)dest;
    uint32_t s1, s2, d1, d2;

    s1 = s & 0x00ff00ff;
    s2 = (s >> 8) & 0x00ff00ff;
    d1 = d & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;

    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *(uint32_t *)dest = d1 | (d2 << 8);
    src += 4;
    dest += 4;
  }
  for (; n > 0; n--) {
    *dest = COMPOSITE_ADD(*dest, *src);
    src++;
    dest++;
  }
}
OIL_DEFINE_IMPL (composite_add_u8_fast, composite_add_u8);
#endif

#ifdef HAVE_UNALIGNED_ACCESS
static void
composite_add_u8_const_src_fast (uint8_t *dest, const uint8_t *src, int n)
{
  uint32_t s;

  s = *src | (*src << 16);
  for (; n > 3; n-= 4) {
    uint32_t d = *(uint32_t *)dest;
    uint32_t d1, d2;

    d1 = d & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;

    d1 += s;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 += s;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *(uint32_t *)dest = d1 | (d2 << 8);
    dest += 4;
  }
  for (; n > 0; n--) {
    *dest = COMPOSITE_ADD(*dest, *src);
    dest++;
  }
}
OIL_DEFINE_IMPL (composite_add_u8_const_src_fast, composite_add_u8_const_src);
#endif

#ifdef HAVE_UNALIGNED_ACCESS
static void
composite_over_u8_fast (uint8_t *dest, const uint8_t *src, int n)
{
  for (; n > 3; n-= 4) {
    uint32_t d = *(uint32_t *)dest, s = *(uint32_t *)src;
    uint32_t d1, d2, s1, s2;

    d1 = d & 0x00ff00ff;
    d2 = (d >> 8) & 0x00ff00ff;
    s1 = s & 0x00ff00ff;
    s2 = (s >> 8) & 0x00ff00ff;

    d1 = ((d1 & 0xff) * (~s1 & 0xff)) |
	((d1 & 0x00ff0000) * (~s1 >> 16 & 0xff));
    d1 += 0x00800080;
    d1 += (d1 >> 8) & 0x00ff00ff;
    d1 >>= 8;
    d1 &= 0x00ff00ff;
    d1 += s1;
    d1 |= 0x01000100 - ((d1 >> 8) & 0x00ff00ff);
    d1 &= 0x00ff00ff;

    d2 = ((d2 & 0xff) * (~s2 & 0xff)) |
	((d2 & 0x00ff0000) * (~s2 >> 16 & 0xff));
    d2 += 0x00800080;
    d2 += (d2 >> 8) & 0x00ff00ff;
    d2 >>= 8;
    d2 &= 0x00ff00ff;
    d2 += s2;
    d2 |= 0x01000100 - ((d2 >> 8) & 0x00ff00ff);
    d2 &= 0x00ff00ff;

    *(uint32_t *)dest = d1 | (d2 << 8);
    dest += 4;
    src += 4;
  }
  for (; n > 0; n--) {
    *dest = COMPOSITE_OVER(*dest, *src, *src);
    dest++;
    src++;
  }
}
OIL_DEFINE_IMPL (composite_over_u8_fast, composite_over_u8);
#endif
