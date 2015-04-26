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

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>
#include <liboil/liboilrandom.h>
#include <liboil/liboilcolorspace.h>
#include <liboil/liboiltest.h>
#include <liboil/liboildebug.h>

#define COMPOSITE_OVER(d,s,m) ((d) + (s) - oil_muldiv_255((d),(m)))
#define COMPOSITE_ADD(d,s) oil_clamp_255((d) + (s))
#define COMPOSITE_IN(s,m) oil_muldiv_255((s),(m))

/**
 * SECTION:liboilfuncs-pixel
 * @title: Pixel Operations
 * @short_description: Operations on pixels
 *
 * Pixels are 4-element arrays of type uint8_t.  The elements, in
 * memory order, represent the alpha, red, green, and blue
 * components respectively.  The color components are premultiplied
 * with the alpha component.  Liboil functions represent pixels
 * as the type uint32_t.
 *
 * The compositing operators IN, OVER, and ADD are defined the same
 * as cairo.
 *
 */
static void
handle_param (OilParameter *p)
{
  int n;

  if (p->src_data) {
    if (p->type == OIL_TYPE_u32p) {
      uint32_t *ptr;
      ptr = (uint32_t *)oil_param_get_source_data (p);
      n = p->post_n;
      oil_random_argb (ptr, n);
    }
    if (p->type == OIL_TYPE_u8p) {
      uint8_t *ptr;
      ptr = (uint8_t *)oil_param_get_source_data (p);
      n = p->post_n;
      oil_random_alpha (ptr, n);
    }
  }
}

static void
composite_test (OilTest *test)
{
  handle_param(&test->params[OIL_ARG_SRC1]);
  handle_param(&test->params[OIL_ARG_SRC2]);
  handle_param(&test->params[OIL_ARG_INPLACE1]);
}

/**
 * oil_composite_in_argb:
 * @d_n: DEST
 * @s1_n: SRC
 * @s2_n: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC IN MASK.
 */
OIL_DEFINE_CLASS_FULL (composite_in_argb,
    "uint32_t *d_n, uint32_t *s1_n, uint8_t *s2_n, int n",
    composite_test);
/**
 * oil_composite_in_argb_const_src:
 * @d_n: DEST
 * @s1_1: SRC
 * @s2_n: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC IN MASK, for a constant
 * SRC.
 */
OIL_DEFINE_CLASS_FULL (composite_in_argb_const_src,
    "uint32_t *d_n, uint32_t *s1_1, uint8_t *s2_n, int n",
    composite_test);
/**
 * oil_composite_in_argb_const_mask:
 * @d_n: DEST
 * @s1_n: SRC
 * @s2_1: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC IN MASK, for a constant
 * MASK.
 */
OIL_DEFINE_CLASS_FULL (composite_in_argb_const_mask,
    "uint32_t *d_n, uint32_t *s1_n, uint8_t *s2_1, int n",
    composite_test);
/**
 * oil_composite_over_argb:
 * @i_n: DEST
 * @s1_n: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC OVER DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_over_argb,
    "uint32_t *i_n, uint32_t *s1_n, int n",
    composite_test);
/**
 * oil_composite_over_argb_const_src:
 * @i_n: DEST
 * @s1_1: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC OVER DEST, for a
 * constant SRC.
 */
OIL_DEFINE_CLASS_FULL (composite_over_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, int n",
    composite_test);
/**
 * oil_composite_add_argb:
 * @i_n: DEST
 * @s1_n: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC ADD DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_add_argb,
    "uint32_t *i_n, uint32_t *s1_n, int n",
    composite_test);
/**
 * oil_composite_add_argb_const_src:
 * @i_n: DEST
 * @s1_1: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC ADD DEST, for a
 * constant SRC.
 */
OIL_DEFINE_CLASS_FULL (composite_add_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, int n",
    composite_test);
/**
 * oil_composite_in_over_argb:
 * @i_n: DEST
 * @s1_n: SRC
 * @s2_n: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = (SRC IN MASK) OVER DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_in_over_argb,
    "uint32_t *i_n, uint32_t *s1_n, uint8_t *s2_n, int n",
    composite_test);
/**
 * oil_composite_in_over_argb_const_src:
 * @i_n: DEST
 * @s1_1: SRC
 * @s2_n: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = (SRC IN MASK) OVER DEST,
 * for a constant SRC.
 */
OIL_DEFINE_CLASS_FULL (composite_in_over_argb_const_src,
    "uint32_t *i_n, uint32_t *s1_1, uint8_t *s2_n, int n",
    composite_test);
/**
 * oil_composite_in_over_argb_const_mask:
 * @i_n: DEST
 * @s1_n: SRC
 * @s2_1: MASK
 * @n: number of elements
 *
 * Performs the compositing operation DEST = (SRC IN MASK) OVER DEST,
 * for a constant MASK.
 */
OIL_DEFINE_CLASS_FULL (composite_in_over_argb_const_mask,
    "uint32_t *i_n, uint32_t *s1_n, uint8_t *s2_1, int n",
    composite_test);
/**
 * oil_composite_over_u8:
 * @i_n: DEST
 * @s1_n: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC OVER DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_over_u8,
    "uint8_t *i_n, uint8_t *s1_n, int n",
    composite_test);
/**
 * oil_composite_add_u8:
 * @i_n: DEST
 * @s1_n: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC ADD DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_add_u8,
    "uint8_t *i_n, uint8_t *s1_n, int n",
    composite_test);
/**
 * oil_composite_add_u8_const_src:
 * @i_n: DEST
 * @s1_1: SRC
 * @n: number of elements
 *
 * Performs the compositing operation DEST = SRC ADD DEST.
 */
OIL_DEFINE_CLASS_FULL (composite_add_u8_const_src,
    "uint8_t *i_n, uint8_t *s1_1, int n",
    composite_test);

static void
composite_in_argb_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_R(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_G(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_B(src[i]), mask[i]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_ref, composite_in_argb);

static void
composite_in_argb_const_src_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_R(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_G(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_B(src[0]), mask[i]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_const_src_ref, composite_in_argb_const_src);

static void
composite_in_argb_const_mask_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_R(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_G(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_B(src[i]), mask[0]));
  }
}
OIL_DEFINE_IMPL_REF (composite_in_argb_const_mask_ref, composite_in_argb_const_mask);

static void
composite_over_argb_ref (uint32_t *dest, const uint32_t *src, int n)
{
  int i;
  uint8_t a;

  for(i=0;i<n;i++){
    a = oil_argb_A(src[i]);
    dest[i] = oil_argb(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(src[i]),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(src[i]),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(src[i]),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(src[i]),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_over_argb_ref, composite_over_argb);

static void
composite_over_argb_const_src_ref (uint32_t *dest, const uint32_t *src, int n)
{
  int i;
  uint8_t a;

  a = oil_argb_A(src[0]);
  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(src[0]),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(src[0]),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(src[0]),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(src[0]),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_over_argb_const_src_ref, composite_over_argb_const_src);

static void
composite_add_argb_ref (uint32_t *dest, const uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_ADD(oil_argb_A(dest[i]),oil_argb_A(src[i])),
        COMPOSITE_ADD(oil_argb_R(dest[i]),oil_argb_R(src[i])),
        COMPOSITE_ADD(oil_argb_G(dest[i]),oil_argb_G(src[i])),
        COMPOSITE_ADD(oil_argb_B(dest[i]),oil_argb_B(src[i])));
  }

}
OIL_DEFINE_IMPL_REF (composite_add_argb_ref, composite_add_argb);

static void
composite_add_argb_const_src_ref (uint32_t *dest, const uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = oil_argb(
        COMPOSITE_ADD(oil_argb_A(dest[i]),oil_argb_A(src[0])),
        COMPOSITE_ADD(oil_argb_R(dest[i]),oil_argb_R(src[0])),
        COMPOSITE_ADD(oil_argb_G(dest[i]),oil_argb_G(src[0])),
        COMPOSITE_ADD(oil_argb_B(dest[i]),oil_argb_B(src[0])));
  }

}
OIL_DEFINE_IMPL_REF (composite_add_argb_const_src_ref, composite_add_argb_const_src);

static void
composite_in_over_argb_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_R(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_G(src[i]), mask[i]),
        COMPOSITE_IN(oil_argb_B(src[i]), mask[i]));
    a = oil_argb_A(color);
    dest[i] = oil_argb(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(color),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(color),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(color),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_ref, composite_in_over_argb);

static void
composite_in_over_argb_const_src_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_R(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_G(src[0]), mask[i]),
        COMPOSITE_IN(oil_argb_B(src[0]), mask[i]));
    a = oil_argb_A(color);
    dest[i] = oil_argb(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(color),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(color),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(color),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_const_src_ref, composite_in_over_argb_const_src);

static void
composite_in_over_argb_const_mask_ref (uint32_t *dest, const uint32_t *src, const uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = oil_argb(
        COMPOSITE_IN(oil_argb_A(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_R(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_G(src[i]), mask[0]),
        COMPOSITE_IN(oil_argb_B(src[i]), mask[0]));
    a = oil_argb_A(color);
    dest[i] = oil_argb(
        COMPOSITE_OVER(oil_argb_A(dest[i]),oil_argb_A(color),a),
        COMPOSITE_OVER(oil_argb_R(dest[i]),oil_argb_R(color),a),
        COMPOSITE_OVER(oil_argb_G(dest[i]),oil_argb_G(color),a),
        COMPOSITE_OVER(oil_argb_B(dest[i]),oil_argb_B(color),a));
  }

}
OIL_DEFINE_IMPL_REF (composite_in_over_argb_const_mask_ref, composite_in_over_argb_const_mask);

static void
composite_add_u8_ref (uint8_t *dest, const uint8_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = COMPOSITE_ADD(dest[i],src[i]);
  }

}
OIL_DEFINE_IMPL_REF (composite_add_u8_ref, composite_add_u8);

static void
composite_add_u8_const_src_ref (uint8_t *dest, const uint8_t *src1_1, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = COMPOSITE_ADD(dest[i],src1_1[0]);
  }

}
OIL_DEFINE_IMPL_REF (composite_add_u8_const_src_ref, composite_add_u8_const_src);

static void
composite_over_u8_ref (uint8_t *dest, const uint8_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = COMPOSITE_OVER(dest[i],src[i],src[i]);
  }

}
OIL_DEFINE_IMPL_REF (composite_over_u8_ref, composite_over_u8);


