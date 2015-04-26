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
#include <liboil/simdpack/simdpack.h>
#include <math.h>

/**
 * oil_mix_u8:
 * @dest: DEST
 * @src1: SRC1
 * @src2: SRC2
 * @src3: MASK
 * @n:
 *
 * Performs the compisiting operation DEST = (SRC2 IN MASK) OVER SRC1,
 * except it is done incorrectly, so this function should not be used.
 *
 * FIXME: This function is broken.
 */
OIL_DEFINE_CLASS (mix_u8,
    "uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *src3, int n");

static void mix_u8_ref(uint8_t *dest, uint8_t *src1, uint8_t *src2, uint8_t *scale, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = (src1[i]*scale[i] + src2[i]*(255-scale[i]))>>8;
	}
}

OIL_DEFINE_IMPL_REF (mix_u8_ref, mix_u8);

