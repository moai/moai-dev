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

/* This is a suprisingly fast implementation of clipping
 * in straight C.  It would be difficult to do it faster in asm
 * without specialized opcodes. */

static void
clip_s16_fast (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int i;
	int32_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr, int16_t);
		x = x - (((x-*low)>>31)&(x-*low)) + (((*hi-x)>>31)&(*hi-x));
		OIL_GET(dest,i*dstr, int16_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s16_fast, clip_s16);

static void
clip_s16_fast2 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int32_t x1, x2;

        if (n&1) {
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
		n--;
        }
	while(n>0){
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		 x2 = *src;
		 OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		 x2 -= (((x2-*low)>>31)&(x2-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
		 *dest = x2 + (((*hi-x2)>>31)&(*hi-x2));
		 OIL_INCREMENT(dest,dstr);
		n-=2;
	}
}
OIL_DEFINE_IMPL (clip_s16_fast2, clip_s16);




#if 0
/* broken */

/* This trick clips
 * the range min^(1<<31) to max^(1<<31) incorrectly with int32_t.
 * Thus the use of int64_t. */

static void
clip_s32_fast (int32_t *dest, int dstr, int32_t *src, int sstr, int n,
    int32_t *low, int32_t *hi)
{
	int i;
	int64_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr,int32_t);
		x = x - (((x-*low)>>63)&(x-*low)) + (((*hi-x)>>63)&(*hi-x));
		OIL_GET(dest,i*dstr,int32_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s32_fast, clip_s32);
#endif


