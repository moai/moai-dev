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

#ifndef _LIBOIL_CONV_H_
#define _LIBOIL_CONV_H_

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(conv_f64_s8);
OIL_DECLARE_CLASS(conv_f64_s16);
OIL_DECLARE_CLASS(conv_f64_s32);
OIL_DECLARE_CLASS(conv_f64_u8);
OIL_DECLARE_CLASS(conv_f64_u16);
OIL_DECLARE_CLASS(conv_f64_u32);
OIL_DECLARE_CLASS(conv_f64_f32);

OIL_DECLARE_CLASS(conv_f32_s8);
OIL_DECLARE_CLASS(conv_f32_s16);
OIL_DECLARE_CLASS(conv_f32_u8);
OIL_DECLARE_CLASS(conv_f32_u16);

OIL_DECLARE_CLASS(conv_s32_s8);
OIL_DECLARE_CLASS(conv_s32_s16);
OIL_DECLARE_CLASS(conv_s32_u8);
OIL_DECLARE_CLASS(conv_s32_u16);

OIL_DECLARE_CLASS(conv_u32_s8);
OIL_DECLARE_CLASS(conv_u32_s16);
OIL_DECLARE_CLASS(conv_u32_u8);
OIL_DECLARE_CLASS(conv_u32_u16);

OIL_DECLARE_CLASS(conv_s16_s8);
OIL_DECLARE_CLASS(conv_s16_u8);

OIL_DECLARE_CLASS(conv_u16_s8);
OIL_DECLARE_CLASS(conv_u16_u8);

OIL_DECLARE_CLASS(clipconv_s8_u8);
OIL_DECLARE_CLASS(clipconv_s8_s16);
OIL_DECLARE_CLASS(clipconv_s8_u16);
OIL_DECLARE_CLASS(clipconv_s8_s32);
OIL_DECLARE_CLASS(clipconv_s8_u32);
OIL_DECLARE_CLASS(clipconv_s8_f32);
OIL_DECLARE_CLASS(clipconv_s8_f64);

OIL_DECLARE_CLASS(clipconv_u8_s8);
OIL_DECLARE_CLASS(clipconv_u8_s16);
OIL_DECLARE_CLASS(clipconv_u8_u16);
OIL_DECLARE_CLASS(clipconv_u8_s32);
OIL_DECLARE_CLASS(clipconv_u8_u32);
OIL_DECLARE_CLASS(clipconv_u8_f32);
OIL_DECLARE_CLASS(clipconv_u8_f64);

OIL_DECLARE_CLASS(clipconv_s16_u16);
OIL_DECLARE_CLASS(clipconv_s16_s32);
OIL_DECLARE_CLASS(clipconv_s16_u32);
OIL_DECLARE_CLASS(clipconv_s16_f32);
OIL_DECLARE_CLASS(clipconv_s16_f64);

OIL_DECLARE_CLASS(clipconv_u16_s16);
OIL_DECLARE_CLASS(clipconv_u16_s32);
OIL_DECLARE_CLASS(clipconv_u16_u32);
OIL_DECLARE_CLASS(clipconv_u16_f32);
OIL_DECLARE_CLASS(clipconv_u16_f64);

OIL_DECLARE_CLASS(clipconv_s32_u32);
OIL_DECLARE_CLASS(clipconv_s32_f32);
OIL_DECLARE_CLASS(clipconv_s32_f64);

OIL_DECLARE_CLASS(clipconv_u32_s32);
OIL_DECLARE_CLASS(clipconv_u32_f32);
OIL_DECLARE_CLASS(clipconv_u32_f64);

OIL_DECLARE_CLASS(conv_f32_s32);
OIL_DECLARE_CLASS(conv_f32_u32);
OIL_DECLARE_CLASS(conv_f32_f64);




OIL_DECLARE_CLASS(conv_s8_u8);
OIL_DECLARE_CLASS(conv_s8_s16);
OIL_DECLARE_CLASS(conv_s8_u16);
OIL_DECLARE_CLASS(conv_s8_s32);
OIL_DECLARE_CLASS(conv_s8_u32);
OIL_DECLARE_CLASS(conv_s8_f32);
OIL_DECLARE_CLASS(conv_s8_f64);

OIL_DECLARE_CLASS(conv_u8_s8);
OIL_DECLARE_CLASS(conv_u8_s16);
OIL_DECLARE_CLASS(conv_u8_u16);
OIL_DECLARE_CLASS(conv_u8_s32);
OIL_DECLARE_CLASS(conv_u8_u32);
OIL_DECLARE_CLASS(conv_u8_f32);
OIL_DECLARE_CLASS(conv_u8_f64);

OIL_DECLARE_CLASS(conv_s16_u16);
OIL_DECLARE_CLASS(conv_s16_s32);
OIL_DECLARE_CLASS(conv_s16_u32);
OIL_DECLARE_CLASS(conv_s16_f32);
OIL_DECLARE_CLASS(conv_s16_f64);

OIL_DECLARE_CLASS(conv_u16_s16);
OIL_DECLARE_CLASS(conv_u16_s32);
OIL_DECLARE_CLASS(conv_u16_u32);
OIL_DECLARE_CLASS(conv_u16_f32);
OIL_DECLARE_CLASS(conv_u16_f64);

OIL_DECLARE_CLASS(conv_s32_u32);
OIL_DECLARE_CLASS(conv_s32_f32);
OIL_DECLARE_CLASS(conv_s32_f64);

OIL_DECLARE_CLASS(conv_u32_s32);
OIL_DECLARE_CLASS(conv_u32_f32);
OIL_DECLARE_CLASS(conv_u32_f64);

#endif

