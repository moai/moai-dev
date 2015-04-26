/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2002,2003,2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/liboilclasses.h>


#define DEFINE_CLIPCONVERT_POWERPC(dsttype,srctype,minval,maxval,insn1,insn2)					\
void clipconv_##dsttype##_##srctype##_powerpc(oil_type_##dsttype *dst,	\
	int dst_stride, oil_type_##srctype *src, int src_stride, int n)		\
{									\
	double min = (minval);						\
	double max = (maxval);						\
	double ftmp;							\
									\
	asm volatile(							\
		"	sub %1,%1,%6		\n"			\
		"	sub %0,%0,%7		\n"			\
		"	mtctr %2		\n"			\
		insn1							\
		"	fsub f1,f0,%3		\n"			\
		"	fsel f0,f1,f0,%3	\n"			\
		"	fsub f1,f0,%4		\n"			\
		"	fsel f0,f1,%4,f0	\n"			\
		"	fctiw f1,f0		\n"			\
		"	stfd f1,0(%5)		\n"			\
		insn2							\
		"	bdnz 1b			\n"			\
	: "+b" (dst), "+b" (src)					\
	: "b" (n), "f" (min), "f" (max), "b" (&ftmp), "r" (src_stride),	\
	  "r" (dst_stride)						\
	: "32", "33", "11", "ctr" );					\
} \
OIL_DEFINE_IMPL_ASM(clipconv_##dsttype##_##srctype##_powerpc, \
    clipconv_##dsttype##_##srctype);

#define LFDUX	"1:	lfdux f0,%1,%6		\n"
#define LFSUX	"1:	lfsux f0,%1,%6		\n"

#define LBZ_STBUX \
		"	lbz r11,7(%5)		\n" \
		"	stbux r11,%0,%7		\n"
#define LHZ_STHUX \
		"	lhz r11,6(%5)		\n" \
		"	sthux r11,%0,%7		\n"
#define LWZ_STWUX \
		"	lwz r11,4(%5)		\n" \
		"	stwux r11,%0,%7		\n"

#ifdef HAVE_GCC_ASM_POWERPC_FPU
DEFINE_CLIPCONVERT_POWERPC(s8,f32, -128.0, 127.0, LFSUX, LBZ_STBUX)
DEFINE_CLIPCONVERT_POWERPC(u8,f32, 0.0, 255.0, LFSUX, LBZ_STBUX)
DEFINE_CLIPCONVERT_POWERPC(s16,f32, -32768.0, 32767.0, LFSUX, LHZ_STHUX)
DEFINE_CLIPCONVERT_POWERPC(u16,f32, 0.0, 65535.0, LFSUX, LHZ_STHUX)
DEFINE_CLIPCONVERT_POWERPC(s32,f32, -2147483648.0, 2147483647.0, LFSUX, LWZ_STWUX)
DEFINE_CLIPCONVERT_POWERPC(u32,f32, 0.0, 4294967295.0, LFSUX, LWZ_STWUX)

DEFINE_CLIPCONVERT_POWERPC(s8,f64, -128.0, 127.0, LFDUX, LBZ_STBUX)
DEFINE_CLIPCONVERT_POWERPC(u8,f64, 0.0, 255.0, LFDUX, LBZ_STBUX)
DEFINE_CLIPCONVERT_POWERPC(s16,f64, -32768.0, 32767.0, LFDUX, LHZ_STHUX)
DEFINE_CLIPCONVERT_POWERPC(u16,f64, 0.0, 65535.0, LFDUX, LHZ_STHUX)
DEFINE_CLIPCONVERT_POWERPC(s32,f64, -2147483648.0, 2147483647.0, LFDUX, LWZ_STWUX)
DEFINE_CLIPCONVERT_POWERPC(u32,f64, 0.0, 4294967295.0, LFDUX, LWZ_STWUX)
#endif


#if 0
SL_FUNC void _sl_clipconv_S8_F32__powerpc_altivec(
        int8_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rint(*src);
			if(x>127.0)x=127.0;
			if(x<-128.0)x=-128.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx v0,0,%0		\n"
			"	vrfin v0,v0		\n"
			"	vctsxs v0,v0,24		\n"
			"	stvx v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int8_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[4];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[8];
		(void *)dst += dst_stride;
		*dst = ((int8_t *)vec)[12];
		(void *)dst += dst_stride;
	}
}

SL_FUNC void _sl_clipconv_S16_F32__powerpc_altivec(
        int16_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rint(*src);
			if(x>32767.0)x=32767.0;
			if(x<-32768.0)x=-32768.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx %%v0,0,%0		\n"
			"	vrfin %%v0,%%v0		\n"
			"	vctsxs %%v0,%%v0,16	\n"
			"	stvx %%v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int16_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[2];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[4];
		(void *)dst += dst_stride;
		*dst = ((int16_t *)vec)[6];
		(void *)dst += dst_stride;
	}
}

SL_FUNC void _sl_clipconvert_S32_F32__powerpc_altivec(
        int32_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	static uint8_t vec[16] __attribute__ ((__aligned__ (16)));
	int i;
	
	if(n&3){
		double x;
		for(i=0;i<(n&3);i++){
			x = rintf(*src);
			if(x>2147483647.0)x=2147483647.0;
			if(x<-2147483648.0)x=-2147483648.0;
			*dst = x;
			(void *)src += src_stride;
			(void *)dst += dst_stride;
		}
	}
	n>>=2;
	for(i=0;i<n;i++){
		((float *)vec)[0] = *src;
		(void *)src += src_stride;
		((float *)vec)[1] = *src;
		(void *)src += src_stride;
		((float *)vec)[2] = *src;
		(void *)src += src_stride;
		((float *)vec)[3] = *src;

		asm volatile(
			"	lvx %%v0,0,%0		\n"
			"	vrfin %%v0,%%v0		\n"
			"	vctsxs %%v0,%%v0,0	\n"
			"	stvx %%v0,0,%0		\n"
		:
		: "b" (&(vec[0]))
		: "memory"
		);

		*dst = ((int32_t *)vec)[0];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[1];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[2];
		(void *)dst += dst_stride;
		*dst = ((int32_t *)vec)[3];
		(void *)dst += dst_stride;
	}
}


static void
convert_s16_f64__powerpc(int16_t *dst, int dst_stride, double *src,
    int src_stride, int n)
{
	double ftmp;
	
	asm volatile(
		"	sub %1,%1,%4		\n"
		"	sub %0,%0,%5		\n"
		"	mtctr %2		\n"
		"1:	lfdux 0,%4,%1		\n"
		"	fctiw 1,0		\n"
		"	stfd 1,0(%3)		\n"
		"	lhz 11,6(%3)		\n"
		"	sthux 11,%5,(%0)	\n"
		"	bdnz 1b			\n"
	: "+b" (dst), "+b" (src)
	: "b" (n), "b" (&ftmp), "r" (src_stride), "r" (dst_stride)
	: "32", "33", "11", "ctr" );
}

static void
_sl_convert_S16_F32__powerpc(int16_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	double ftmp;
	
	asm volatile(
		"	sub %1,%1,%4		\n"
		"	sub %0,%0,%5		\n"
		"	mtctr %2		\n"
		"1:	lfdux 0,%4,%1		\n"
		"	fctiw 1,0		\n"
		"	stfd 1,0(%3)		\n"
		"	lhz 11,6(%3)		\n"
		"	sthux 11,%5,(%0)	\n"
		"	bdnz 1b			\n"
	: "+b" (dst), "+b" (src)
	: "b" (n), "b" (&ftmp), "r" (src_stride), "r" (dst_stride)
	: "32", "33", "11", "ctr" );
}


void conv_f64_s16_altivec(double *dest, int dest_stride, short *src,
	int src_stride, int n)
{
	static union { int i[4]; float f[4]; } av_tmp
		__attribute__ ((__aligned__ (16)));
	int i;

	for(i=0;i<n;i+=4){
		av_tmp.i[0] = *OIL_OFFSET(src,src_stride * 0);
		av_tmp.i[1] = *OIL_OFFSET(src,src_stride * 1);
		av_tmp.i[2] = *OIL_OFFSET(src,src_stride * 2);
		av_tmp.i[3] = *OIL_OFFSET(src,src_stride * 3);

		asm(
		"	lvx 0,0,%0\n"
		"	vcfsx 1,0,0\n"
		"	stvx 1,0,%0\n"
		: : "r" (&av_tmp)
		);

		*OIL_OFFSET(dest,dest_stride * 0) =av_tmp.f[0];
		*OIL_OFFSET(dest,dest_stride * 1) =av_tmp.f[1];
		*OIL_OFFSET(dest,dest_stride * 2) =av_tmp.f[2];
		*OIL_OFFSET(dest,dest_stride * 3) =av_tmp.f[3];
		src = OIL_OFFSET(src, src_stride * 4);
		dest = OIL_OFFSET(dest, dest_stride * 4);
	}
}
OIL_DEFINE_IMPL_ASM (conv_f64_s16_altivec, conv_f64_s16);
#endif


/* double to short */

/* FIXME wrong prototype */
#if 0
#ifdef HAVE_GCC_ASM_POWERPC_FPU
void clipconv_s16_f64_ppcasm(short *dest, double *src, int n)
{
	int tmp[2];
	double min = -32768.0;
	double max = 32767.0;
	double ftmp0, ftmp1;

	asm __volatile__(
	"	addic. %3,%3,-8		\n"
	"	addic. %6,%6,-2		\n"
	"loop:				\n"
	"	lfdu %0,8(%3)		\n"
	"	fsub %1,%0,%4		\n"
	"	fsel %0,%1,%0,%4	\n"
	"	fsub %1,%0,%5		\n"
	"	fsel %0,%1,%5,%0	\n"
	"	fctiw %1,%0		\n"
	"	addic. 5,5,-1		\n"
	"	stfd %1,0(%2)		\n"
	"	lhz 9,6(%2)		\n"
	"	sthu 9,2(%6)		\n"
	"	bne loop		\n"
	: "=&f" (ftmp0),
	  "=&f" (ftmp1)
	: "b" (tmp),
	  "r" (src),
	  "f" (min),
	  "f" (max),
	  "r" (dest)
	: "r9",
	  "r5" );
}
OIL_DEFINE_IMPL_ASM (clipconv_s16_f64_ppcasm, clipconv_s16_f64);
#endif
#endif


