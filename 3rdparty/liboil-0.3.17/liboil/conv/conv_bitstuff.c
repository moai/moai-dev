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
#include <liboil/liboilfunction.h>
#include "conv.h"

#ifdef HAVE_IEEE754_H

#include <ieee754.h>

static void conv_f32_u8_bitstuff(float *dst, int dest_stride, const uint8_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_u8_bitstuff, conv_f32_u8);

static void conv_f32_s8_bitstuff(float *dst, int dest_stride, const int8_t *src,
	int src_stride, int n)
{
	const float offset = -384;
	union ieee754_float id;
	int i;

	id.f = 0x1ff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x80)<<15);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_s8_bitstuff, conv_f32_s8);

static void conv_f32_u16_bitstuff(float *dst, int dest_stride, const uint16_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_u16_bitstuff, conv_f32_u16);

#ifdef ENABLE_BROKEN_IMPLS
/* This is intermittently broken on powerpc for unknown reasons */
static void conv_f32_s16_bitstuff(float *dst, int dest_stride, const int16_t *src,
	int src_stride, int n)
{
	const float offset = -98304;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x8000)<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f32_s16_bitstuff, conv_f32_s16);
#endif


#define signbit_S32(x) (((uint32_t)(x))>>31)

#if 0
/* broken */
/* This implementation is slightly inaccurate */
static void conv_s16_f32_bitstuff(int16_t *dst, int dest_stride, const float *src,
	int src_stride, int n)
{
	const float offset = 98304.5;
	union ieee754_double id;
	int i;
	int16_t d;

	for(i=0;i<n;i++){
		id.d = offset + *src;
		d = 0x8000 | (id.ieee.mantissa0 >> 4);
		d += (-32768-d)*signbit_S32(id.ieee.exponent-1039);
		d += (32767-d)*signbit_S32(1039-id.ieee.exponent);
		*dst = d;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_s16_f32_bitstuff, conv_s16_f32);
#endif


#if 0
static void conv_f64_u8_bitstuff(float *dst, int dest_stride, const uint8_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_u8_bitstuff, conv_f64_u8);

static void conv_f64_s8_bitstuff(float *dst, int dest_stride, const int8_t *src,
	int src_stride, int n)
{
	const float offset = -384;
	union ieee754_float id;
	int i;

	id.f = 0x1ff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x80)<<15);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s8_bitstuff, conv_f64_s8);

static void conv_f64_u16_bitstuff(float *dst, int dest_stride, const uint16_t *src,
	int src_stride, int n)
{
	const float offset = -65536;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = (*src<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_u16_bitstuff, conv_f64_u16);

static void conv_f64_s16_bitstuff(float *dst, int dest_stride, const int16_t *src,
	int src_stride, int n)
{
	const float offset = -98304;
	union ieee754_float id;
	int i;

	id.f = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa = ((*src^0x8000)<<7);
		*dst = id.f + offset;
		OIL_INCREMENT(dst, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_bitstuff, conv_f64_s16);
#endif

#if 0
/* broken */
/* This implementation is slightly inaccurate */
static void conv_s16_f64_bitstuff(int16_t *dst, int dest_stride, const float *src,
	int src_stride, int n)
{
	const float offset = 98304.5;
	union ieee754_double id;
	int i;
	int16_t d;

	for(i=0;i<n;i++){
		id.d = offset + *src;
		d = 0x8000 ^ (id.ieee.mantissa0 >> 4);
		d += (-32768-d)*signbit_S32(id.ieee.exponent-1039);
		d += (32767-d)*signbit_S32(1039-id.ieee.exponent);
#if 0
/* for clipping */
		if (id.ieee.exponent < 1039) {
		  d = -32768;
		}
		if (id.ieee.exponent > 1039) {
		  d = 32767;
		}
#endif
		*dst = d;
		OIL_INCREMENT (dst, dest_stride);
		OIL_INCREMENT (src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_s16_f64_bitstuff, conv_s16_f64);
#endif

#endif

