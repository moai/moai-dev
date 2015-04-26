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

#ifndef _LIBOIL_JPEG_H_
#define _LIBOIL_JPEG_H_

#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS(conv8x8_s16_f64);
OIL_DECLARE_CLASS(conv8x8_f64_s16);
OIL_DECLARE_CLASS(clipconv8x8_u8_s16);

OIL_DECLARE_CLASS(scanlinescale2_u8);

OIL_DECLARE_CLASS(dequantize8x8_s16);

OIL_DECLARE_CLASS(yuv2rgbx_u8);
OIL_DECLARE_CLASS(yuv2rgbx_sub2_u8);
OIL_DECLARE_CLASS(yuv2rgbx_sub4_u8);

OIL_DECLARE_CLASS(unzigzag8x8_s16);
OIL_DECLARE_CLASS(zigzag8x8_s16);

#endif

