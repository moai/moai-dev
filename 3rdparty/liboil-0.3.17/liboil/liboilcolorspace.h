/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_COLORSPACE_H_
#define _LIBOIL_COLORSPACE_H_

#include <liboil/liboilutils.h>

OIL_BEGIN_DECLS

#ifdef OIL_ENABLE_UNSTABLE_API

/**
 * oil_max:
 * @x: a value
 * @y: a value
 *
 * Evaluates to the maximum of @x and @y.
 */
#define oil_max(x,y) ((x)>(y)?(x):(y))
/**
 * oil_min:
 * @x: a value
 * @y: a value
 *
 * Evaluates to the minimum of @x and @y.
 */
#define oil_min(x,y) ((x)<(y)?(x):(y))

/**
 * oil_clamp_255:
 * @x: a value
 *
 * Clamps @x to the range [0,255].
 *
 * Evaluates to the clamped value.
 */
#define oil_clamp_255(x) oil_max(0,oil_min((x),255))

/**
 * oil_argb:
 * @a: alpha component
 * @r: red component
 * @g: green component
 * @b: blue component
 *
 * Creates a Liboil ARGB value from individual components.  Clamps
 * each component to [0,255].
 *
 * Evaluates to the ARGB value
 */
#define oil_argb(a,r,g,b) \
    ((oil_clamp_255(a)<<24) | \
     (oil_clamp_255(r)<<16) | \
     (oil_clamp_255(g)<<8) | \
     (oil_clamp_255(b)<<0))

/**
 * oil_argb_noclamp:
 * @a: alpha component
 * @r: red component
 * @g: green component
 * @b: blue component
 *
 * Creates a Liboil ARGB value from individual components.  Does not
 * clamp components.
 *
 * Evaluates to the ARGB value
 */
#define oil_argb_noclamp(a,r,g,b) \
    (((a)<<24) | ((r)<<16) | ((g)<<8) | ((b)<<0))

/**
 * oil_argb_A:
 * @color: an ARGB value
 *
 * Extracts the alpha component from @color.
 *
 * Evaluates to the alpha component
 */
#define oil_argb_A(color) (((color)>>24)&0xff)
/**
 * oil_argb_R:
 * @color: an ARGB value
 *
 * Extracts the red component from @color.
 *
 * Evaluates to the red component
 */
#define oil_argb_R(color) (((color)>>16)&0xff)
/**
 * oil_argb_G:
 * @color: an ARGB value
 *
 * Extracts the green component from @color.
 *
 * Evaluates to the green component
 */
#define oil_argb_G(color) (((color)>>8)&0xff)
/**
 * oil_argb_B:
 * @color: an ARGB value
 *
 * Extracts the blue component from @color.
 *
 * Evaluates to the blue component
 */
#define oil_argb_B(color) (((color)>>0)&0xff)

/**
 * oil_divide_255:
 * @x: a value in the range [0,65025]
 *
 * Divides @x by 255 in a way that is compatible with the pixel
 * operations in Liboil.  The number 65025 is 255*255.
 *
 * Evaluates to @x divided by 255.
 */
#define oil_divide_255(x) ((((x)+128) + (((x)+128)>>8))>>8)
/**
 * oil_muldiv_255:
 * @a: a value in the range [0,255]
 * @b: a value in the range [0,255]
 *
 * Multiplies @a and @b and divides the result by 255 in a way that
 * is compatible with the pixel operations in Liboil.
 *
 * Evaluates to the result.
 */
#define oil_muldiv_255(a,b) oil_divide_255((a)*(b))

#endif

OIL_END_DECLS

#endif


