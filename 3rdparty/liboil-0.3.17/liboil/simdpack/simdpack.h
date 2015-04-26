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

#ifndef _OIL_SIMDPACK_SIMDPACK_H_
#define _OIL_SIMDPACK_SIMDPACK_H_

OIL_DECLARE_CLASS(conv8x8_s16_f64);
OIL_DECLARE_CLASS(conv8x8_f64_s16);
OIL_DECLARE_CLASS(idct8x8_f64);

OIL_DECLARE_CLASS(abs_u8_s8);
OIL_DECLARE_CLASS(abs_u16_s16);
OIL_DECLARE_CLASS(abs_u32_s32);
OIL_DECLARE_CLASS(abs_f32_f32);
OIL_DECLARE_CLASS(abs_f64_f64);

OIL_DECLARE_CLASS(average2_u8);

OIL_DECLARE_CLASS(clip_s8);
OIL_DECLARE_CLASS(clip_u8);
OIL_DECLARE_CLASS(clip_s16);
OIL_DECLARE_CLASS(clip_u16);
OIL_DECLARE_CLASS(clip_s32);
OIL_DECLARE_CLASS(clip_u32);
OIL_DECLARE_CLASS(clip_f32);
OIL_DECLARE_CLASS(clip_f64);

OIL_DECLARE_CLASS(dct12_f32);

OIL_DECLARE_CLASS(dct36_f32);

OIL_DECLARE_CLASS(diffsquaresum_f64);

OIL_DECLARE_CLASS(downsample1x_f64);

OIL_DECLARE_CLASS(fdct8_f64);

OIL_DECLARE_CLASS(fdct8x8_f64);

OIL_DECLARE_CLASS(fdct8x8s_s16);

OIL_DECLARE_CLASS(get8x8_f64);

OIL_DECLARE_CLASS(idct8_f64);

OIL_DECLARE_CLASS(idct8x8_s16);
OIL_DECLARE_CLASS(idct8x8s_s16);
OIL_DECLARE_CLASS(imdct32_f32);
OIL_DECLARE_CLASS(mix_u8);
OIL_DECLARE_CLASS(mult8x8_s16);

OIL_DECLARE_CLASS(multsum_f32);
OIL_DECLARE_CLASS(multsum_f64);

OIL_DECLARE_CLASS(permute_s8);
OIL_DECLARE_CLASS(permute_u8);
OIL_DECLARE_CLASS(permute_s16);
OIL_DECLARE_CLASS(permute_u16);
OIL_DECLARE_CLASS(permute_s32);
OIL_DECLARE_CLASS(permute_u32);
OIL_DECLARE_CLASS(permute_f32);
OIL_DECLARE_CLASS(permute_f64);

OIL_DECLARE_CLASS(sad8x8_f64);
OIL_DECLARE_CLASS(sad8x8_s16);

OIL_DECLARE_CLASS(scalaradd_s8);
OIL_DECLARE_CLASS(scalaradd_u8);
OIL_DECLARE_CLASS(scalaradd_s16);
OIL_DECLARE_CLASS(scalaradd_u16);
OIL_DECLARE_CLASS(scalaradd_s32);
OIL_DECLARE_CLASS(scalaradd_u32);
OIL_DECLARE_CLASS(scalaradd_f32);
OIL_DECLARE_CLASS(scalaradd_f64);

OIL_DECLARE_CLASS(scalarmult_s8);
OIL_DECLARE_CLASS(scalarmult_u8);
OIL_DECLARE_CLASS(scalarmult_s16);
OIL_DECLARE_CLASS(scalarmult_u16);
OIL_DECLARE_CLASS(scalarmult_s32);
OIL_DECLARE_CLASS(scalarmult_u32);
OIL_DECLARE_CLASS(scalarmult_f32);
OIL_DECLARE_CLASS(scalarmult_f64);

OIL_DECLARE_CLASS(sincos_f64);
OIL_DECLARE_CLASS(squaresum_f64);
OIL_DECLARE_CLASS(sum_f64);

OIL_DECLARE_CLASS(trans8x8_s8);
OIL_DECLARE_CLASS(trans8x8_u8);
OIL_DECLARE_CLASS(trans8x8_s16);
OIL_DECLARE_CLASS(trans8x8_u16);
OIL_DECLARE_CLASS(trans8x8_s32);
OIL_DECLARE_CLASS(trans8x8_u32);
OIL_DECLARE_CLASS(trans8x8_f32);
OIL_DECLARE_CLASS(trans8x8_f64);

OIL_DECLARE_CLASS(vectoradd_s8);
OIL_DECLARE_CLASS(vectoradd_u8);
OIL_DECLARE_CLASS(vectoradd_s16);
OIL_DECLARE_CLASS(vectoradd_u16);
OIL_DECLARE_CLASS(vectoradd_s32);
OIL_DECLARE_CLASS(vectoradd_u32);
OIL_DECLARE_CLASS(vectoradd_f32);
OIL_DECLARE_CLASS(vectoradd_f64);

OIL_DECLARE_CLASS(zigzag8x8_s16);


#endif

