/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2003,2004 David A. Schleef <ds@schleef.org>
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
#include <liboil/conv/conv.h>

#include <math.h>



#define CONV_DEFINE_CAST(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src, \
	int src_stride, int n)		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*dest = *src;		\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _c,	\
	conv_ ## desttype ## _ ## srctype);

#ifdef HAVE_RINT
#define CONV_DEFINE_FLOAT(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src, \
	int src_stride, int n)		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*dest = rint(*src);	\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _c,	\
	conv_ ## desttype ## _ ## srctype);
#else
#define CONV_DEFINE_FLOAT(desttype,srctype)
#endif

CONV_DEFINE_CAST(s8,u8);
CONV_DEFINE_CAST(s8,s16);
CONV_DEFINE_CAST(s8,u16);
CONV_DEFINE_CAST(s8,s32);
CONV_DEFINE_CAST(s8,u32);
CONV_DEFINE_FLOAT(s8,f32);
CONV_DEFINE_FLOAT(s8,f64);

CONV_DEFINE_CAST(u8,s8);
CONV_DEFINE_CAST(u8,s16);
CONV_DEFINE_CAST(u8,u16);
CONV_DEFINE_CAST(u8,s32);
CONV_DEFINE_CAST(u8,u32);
CONV_DEFINE_FLOAT(u8,f32);
CONV_DEFINE_FLOAT(u8,f64);

CONV_DEFINE_CAST(s16,s8)
CONV_DEFINE_CAST(s16,u8)
CONV_DEFINE_CAST(s16,u16);
CONV_DEFINE_CAST(s16,s32);
CONV_DEFINE_CAST(s16,u32);
CONV_DEFINE_FLOAT(s16,f32);
CONV_DEFINE_FLOAT(s16,f64);

CONV_DEFINE_CAST(u16,s8)
CONV_DEFINE_CAST(u16,u8)
CONV_DEFINE_CAST(u16,s16);
CONV_DEFINE_CAST(u16,s32);
CONV_DEFINE_CAST(u16,u32);
CONV_DEFINE_FLOAT(u16,f32);
CONV_DEFINE_FLOAT(u16,f64);

CONV_DEFINE_CAST(s32,s8)
CONV_DEFINE_CAST(s32,s16)
CONV_DEFINE_CAST(s32,u8)
CONV_DEFINE_CAST(s32,u16)
CONV_DEFINE_CAST(s32,u32);
CONV_DEFINE_FLOAT(s32,f32);
CONV_DEFINE_FLOAT(s32,f64);

CONV_DEFINE_CAST(u32,s8)
CONV_DEFINE_CAST(u32,s16)
CONV_DEFINE_CAST(u32,u8)
CONV_DEFINE_CAST(u32,u16)
CONV_DEFINE_CAST(u32,s32);
CONV_DEFINE_FLOAT(u32,f32);
CONV_DEFINE_FLOAT(u32,f64);

CONV_DEFINE_CAST(f32,s8)
CONV_DEFINE_CAST(f32,s16)
CONV_DEFINE_CAST(f32,u8)
CONV_DEFINE_CAST(f32,u16)
CONV_DEFINE_CAST(f32,s32);
CONV_DEFINE_CAST(f32,u32);
CONV_DEFINE_CAST(f32,f64);

CONV_DEFINE_CAST(f64,s8)
CONV_DEFINE_CAST(f64,u8)
CONV_DEFINE_CAST(f64,s16)
CONV_DEFINE_CAST(f64,u16)
CONV_DEFINE_CAST(f64,s32)
CONV_DEFINE_CAST(f64,u32)
CONV_DEFINE_CAST(f64,f32)




#define CLIPCONV_DEFINE_BOTH(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,	\
	int src_stride, int n)		\
{					\
	int i;				\
	oil_type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *src;		\
		if(x<oil_type_min_ ## desttype) x=oil_type_min_ ## desttype;	\
		if(x>oil_type_max_ ## desttype) x=oil_type_max_ ## desttype;	\
		*dest = x;		\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(clipconv_ ## desttype ## _ ## srctype ## _c,	\
	clipconv_ ## desttype ## _ ## srctype);

#define CLIPCONV_DEFINE_UPPER(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,	\
	int src_stride, int n)		\
{					\
	int i;				\
	oil_type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *src;		\
		if(x>oil_type_max_ ## desttype) x=oil_type_max_ ## desttype;	\
		*dest = x;		\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(clipconv_ ## desttype ## _ ## srctype ## _c,	\
	clipconv_ ## desttype ## _ ## srctype);

#define CLIPCONV_DEFINE_LOWER(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,	\
	int src_stride, int n)		\
{					\
	int i;				\
	oil_type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *src;		\
		if(x<oil_type_min_ ## desttype) x=oil_type_min_ ## desttype;	\
		*dest = x;		\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(clipconv_ ## desttype ## _ ## srctype ## _c,	\
	clipconv_ ## desttype ## _ ## srctype);

#ifdef HAVE_RINT
#define CLIPCONV_DEFINE_FLOAT(desttype,srctype) \
static void clipconv_ ## desttype ## _ ## srctype ## _c ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,	\
	int src_stride, int n)		\
{					\
	int i;				\
	oil_type_ ## srctype x;		\
	for(i=0;i<n;i++){		\
		x = *src;		\
		if(x<oil_type_min_ ## desttype) x=oil_type_min_ ## desttype;	\
		if(x>oil_type_max_ ## desttype) x=oil_type_max_ ## desttype;	\
		*dest = rint(x);	\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(clipconv_ ## desttype ## _ ## srctype ## _c,	\
	clipconv_ ## desttype ## _ ## srctype);
#else
#define CLIPCONV_DEFINE_FLOAT(desttype,srctype)
#endif

/* clip upper */
CLIPCONV_DEFINE_UPPER(s8,u8);
CLIPCONV_DEFINE_UPPER(s8,u16);
CLIPCONV_DEFINE_UPPER(s8,u32);
CLIPCONV_DEFINE_UPPER(u8,u32);
CLIPCONV_DEFINE_UPPER(u8,u16);
CLIPCONV_DEFINE_UPPER(s16,u16);
CLIPCONV_DEFINE_UPPER(s16,u32);
CLIPCONV_DEFINE_UPPER(s32,u32);
CLIPCONV_DEFINE_UPPER(u16,u32);

/* clip both */
CLIPCONV_DEFINE_BOTH(s8,s16);
CLIPCONV_DEFINE_BOTH(s8,s32);
CLIPCONV_DEFINE_BOTH(u8,s16);
CLIPCONV_DEFINE_BOTH(u8,s32);
CLIPCONV_DEFINE_BOTH(s16,s32);
CLIPCONV_DEFINE_BOTH(u16,s32);

/* clip lower */
CLIPCONV_DEFINE_LOWER(u8,s8);
CLIPCONV_DEFINE_LOWER(u16,s16);
CLIPCONV_DEFINE_LOWER(u32,s32);

/* clip both, float */
CLIPCONV_DEFINE_FLOAT(s8,f32);
CLIPCONV_DEFINE_FLOAT(s8,f64);
CLIPCONV_DEFINE_FLOAT(u8,f32);
CLIPCONV_DEFINE_FLOAT(u8,f64);
CLIPCONV_DEFINE_FLOAT(s16,f32);
CLIPCONV_DEFINE_FLOAT(s16,f64);
CLIPCONV_DEFINE_FLOAT(u16,f32);
CLIPCONV_DEFINE_FLOAT(u16,f64);
CLIPCONV_DEFINE_FLOAT(s32,f32);
CLIPCONV_DEFINE_FLOAT(s32,f64);
CLIPCONV_DEFINE_FLOAT(u32,f32);
CLIPCONV_DEFINE_FLOAT(u32,f64);




#define CONV_DEFINE_CAST_UNROLL2(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _unroll2 ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	if(n&1){			\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	n>>=1;				\
	for(i=0;i<n;i++){		\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _unroll2,	\
	conv_ ## desttype ## _ ## srctype);

#ifdef HAVE_RINT
#define CONV_DEFINE_FLOAT_UNROLL2(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _unroll2 ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	if(n&1){			\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	n>>=1;				\
	for(i=0;i<n;i++){		\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _unroll2,	\
	conv_ ## desttype ## _ ## srctype);
#else
#define CONV_DEFINE_FLOAT_UNROLL2(desttype,srctype)
#endif

CONV_DEFINE_CAST_UNROLL2(s8,u8);
CONV_DEFINE_CAST_UNROLL2(s8,s16);
CONV_DEFINE_CAST_UNROLL2(s8,u16);
CONV_DEFINE_CAST_UNROLL2(s8,s32);
CONV_DEFINE_CAST_UNROLL2(s8,u32);
CONV_DEFINE_FLOAT_UNROLL2(s8,f32);
CONV_DEFINE_FLOAT_UNROLL2(s8,f64);

CONV_DEFINE_CAST_UNROLL2(u8,s8);
CONV_DEFINE_CAST_UNROLL2(u8,s16);
CONV_DEFINE_CAST_UNROLL2(u8,u16);
CONV_DEFINE_CAST_UNROLL2(u8,s32);
CONV_DEFINE_CAST_UNROLL2(u8,u32);
CONV_DEFINE_FLOAT_UNROLL2(u8,f32);
CONV_DEFINE_FLOAT_UNROLL2(u8,f64);

CONV_DEFINE_CAST_UNROLL2(s16,s8)
CONV_DEFINE_CAST_UNROLL2(s16,u8)
CONV_DEFINE_CAST_UNROLL2(s16,u16);
CONV_DEFINE_CAST_UNROLL2(s16,s32);
CONV_DEFINE_CAST_UNROLL2(s16,u32);
CONV_DEFINE_FLOAT_UNROLL2(s16,f32);
CONV_DEFINE_FLOAT_UNROLL2(s16,f64);

CONV_DEFINE_CAST_UNROLL2(u16,s8)
CONV_DEFINE_CAST_UNROLL2(u16,u8)
CONV_DEFINE_CAST_UNROLL2(u16,s16);
CONV_DEFINE_CAST_UNROLL2(u16,s32);
CONV_DEFINE_CAST_UNROLL2(u16,u32);
CONV_DEFINE_FLOAT_UNROLL2(u16,f32);
CONV_DEFINE_FLOAT_UNROLL2(u16,f64);

CONV_DEFINE_CAST_UNROLL2(s32,s8)
CONV_DEFINE_CAST_UNROLL2(s32,s16)
CONV_DEFINE_CAST_UNROLL2(s32,u8)
CONV_DEFINE_CAST_UNROLL2(s32,u16)
CONV_DEFINE_CAST_UNROLL2(s32,u32);
CONV_DEFINE_FLOAT_UNROLL2(s32,f32);
CONV_DEFINE_FLOAT_UNROLL2(s32,f64);

CONV_DEFINE_CAST_UNROLL2(u32,s8)
CONV_DEFINE_CAST_UNROLL2(u32,s16)
CONV_DEFINE_CAST_UNROLL2(u32,u8)
CONV_DEFINE_CAST_UNROLL2(u32,u16)
CONV_DEFINE_CAST_UNROLL2(u32,s32);
CONV_DEFINE_FLOAT_UNROLL2(u32,f32);
CONV_DEFINE_FLOAT_UNROLL2(u32,f64);

CONV_DEFINE_CAST_UNROLL2(f32,s8)
CONV_DEFINE_CAST_UNROLL2(f32,s16)
CONV_DEFINE_CAST_UNROLL2(f32,u8)
CONV_DEFINE_CAST_UNROLL2(f32,u16)
CONV_DEFINE_CAST_UNROLL2(f32,s32);
CONV_DEFINE_CAST_UNROLL2(f32,u32);
CONV_DEFINE_CAST_UNROLL2(f32,f64);

CONV_DEFINE_CAST_UNROLL2(f64,s8)
CONV_DEFINE_CAST_UNROLL2(f64,u8)
CONV_DEFINE_CAST_UNROLL2(f64,s16)
CONV_DEFINE_CAST_UNROLL2(f64,u16)
CONV_DEFINE_CAST_UNROLL2(f64,s32)
CONV_DEFINE_CAST_UNROLL2(f64,u32)
CONV_DEFINE_CAST_UNROLL2(f64,f32)



#define CONV_DEFINE_CAST_UNROLL4(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _unroll4 ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	if(n&1){			\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	if(n&2){			\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	n>>=2;				\
	for(i=0;i<n;i++){		\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = *src;				\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _unroll4,	\
	conv_ ## desttype ## _ ## srctype);

#ifdef HAVE_RINT
#define CONV_DEFINE_FLOAT_UNROLL4(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _unroll4 ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	if(n&1){			\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	if(n&2){			\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
	n>>=2;				\
	for(i=0;i<n;i++){		\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
		*dest = rint(*src);			\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _unroll4,	\
	conv_ ## desttype ## _ ## srctype);
#else
#define CONV_DEFINE_FLOAT_UNROLL4(desttype,srctype)
#endif

CONV_DEFINE_CAST_UNROLL4(s8,u8);
CONV_DEFINE_CAST_UNROLL4(s8,s16);
CONV_DEFINE_CAST_UNROLL4(s8,u16);
CONV_DEFINE_CAST_UNROLL4(s8,s32);
CONV_DEFINE_CAST_UNROLL4(s8,u32);
CONV_DEFINE_FLOAT_UNROLL4(s8,f32);
CONV_DEFINE_FLOAT_UNROLL4(s8,f64);

CONV_DEFINE_CAST_UNROLL4(u8,s8);
CONV_DEFINE_CAST_UNROLL4(u8,s16);
CONV_DEFINE_CAST_UNROLL4(u8,u16);
CONV_DEFINE_CAST_UNROLL4(u8,s32);
CONV_DEFINE_CAST_UNROLL4(u8,u32);
CONV_DEFINE_FLOAT_UNROLL4(u8,f32);
CONV_DEFINE_FLOAT_UNROLL4(u8,f64);

CONV_DEFINE_CAST_UNROLL4(s16,s8)
CONV_DEFINE_CAST_UNROLL4(s16,u8)
CONV_DEFINE_CAST_UNROLL4(s16,u16);
CONV_DEFINE_CAST_UNROLL4(s16,s32);
CONV_DEFINE_CAST_UNROLL4(s16,u32);
CONV_DEFINE_FLOAT_UNROLL4(s16,f32);
CONV_DEFINE_FLOAT_UNROLL4(s16,f64);

CONV_DEFINE_CAST_UNROLL4(u16,s8)
CONV_DEFINE_CAST_UNROLL4(u16,u8)
CONV_DEFINE_CAST_UNROLL4(u16,s16);
CONV_DEFINE_CAST_UNROLL4(u16,s32);
CONV_DEFINE_CAST_UNROLL4(u16,u32);
CONV_DEFINE_FLOAT_UNROLL4(u16,f32);
CONV_DEFINE_FLOAT_UNROLL4(u16,f64);

CONV_DEFINE_CAST_UNROLL4(s32,s8)
CONV_DEFINE_CAST_UNROLL4(s32,s16)
CONV_DEFINE_CAST_UNROLL4(s32,u8)
CONV_DEFINE_CAST_UNROLL4(s32,u16)
CONV_DEFINE_CAST_UNROLL4(s32,u32);
CONV_DEFINE_FLOAT_UNROLL4(s32,f32);
CONV_DEFINE_FLOAT_UNROLL4(s32,f64);

CONV_DEFINE_CAST_UNROLL4(u32,s8)
CONV_DEFINE_CAST_UNROLL4(u32,s16)
CONV_DEFINE_CAST_UNROLL4(u32,u8)
CONV_DEFINE_CAST_UNROLL4(u32,u16)
CONV_DEFINE_CAST_UNROLL4(u32,s32);
CONV_DEFINE_FLOAT_UNROLL4(u32,f32);
CONV_DEFINE_FLOAT_UNROLL4(u32,f64);

CONV_DEFINE_CAST_UNROLL4(f32,s8)
CONV_DEFINE_CAST_UNROLL4(f32,s16)
CONV_DEFINE_CAST_UNROLL4(f32,u8)
CONV_DEFINE_CAST_UNROLL4(f32,u16)
CONV_DEFINE_CAST_UNROLL4(f32,s32);
CONV_DEFINE_CAST_UNROLL4(f32,u32);
CONV_DEFINE_CAST_UNROLL4(f32,f64);

CONV_DEFINE_CAST_UNROLL4(f64,s8)
CONV_DEFINE_CAST_UNROLL4(f64,u8)
CONV_DEFINE_CAST_UNROLL4(f64,s16)
CONV_DEFINE_CAST_UNROLL4(f64,u16)
CONV_DEFINE_CAST_UNROLL4(f64,s32)
CONV_DEFINE_CAST_UNROLL4(f64,u32)
CONV_DEFINE_CAST_UNROLL4(f64,f32)


#ifdef HAVE_RINTF

#define CONV_DEFINE_FLOAT_RINTF(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _rintf ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*dest = rintf(*src);\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _rintf,	\
	conv_ ## desttype ## _ ## srctype);

CONV_DEFINE_FLOAT_RINTF(s8,f32);
CONV_DEFINE_FLOAT_RINTF(u8,f32);
CONV_DEFINE_FLOAT_RINTF(s16,f32);
CONV_DEFINE_FLOAT_RINTF(u16,f32);
CONV_DEFINE_FLOAT_RINTF(s32,f32);
CONV_DEFINE_FLOAT_RINTF(u32,f32);

#endif

#ifdef HAVE_LRINT

#define CONV_DEFINE_FLOAT_LRINT(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _lrint ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*dest = lrint(*src);	\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _lrint,	\
	conv_ ## desttype ## _ ## srctype);

CONV_DEFINE_FLOAT_LRINT(s8,f32);
CONV_DEFINE_FLOAT_LRINT(s8,f64);

CONV_DEFINE_FLOAT_LRINT(u8,f32);
CONV_DEFINE_FLOAT_LRINT(u8,f64);

CONV_DEFINE_FLOAT_LRINT(s16,f32);
CONV_DEFINE_FLOAT_LRINT(s16,f64);

CONV_DEFINE_FLOAT_LRINT(u16,f32);
CONV_DEFINE_FLOAT_LRINT(u16,f64);

CONV_DEFINE_FLOAT_LRINT(s32,f32);
CONV_DEFINE_FLOAT_LRINT(s32,f64);

//CONV_DEFINE_FLOAT_LRINT(u32,f32);
//CONV_DEFINE_FLOAT_LRINT(u32,f64);

#endif

#ifdef HAVE_LRINTF

#define CONV_DEFINE_FLOAT_LRINTF(desttype,srctype) \
static void conv_ ## desttype ## _ ## srctype ## _lrintf ( \
	oil_type_ ## desttype *dest,	\
	int dest_stride,		\
	const oil_type_ ## srctype *src,		\
	int src_stride, int n)		\
{					\
	int i;				\
	for(i=0;i<n;i++){		\
		*dest = lrintf(*src);	\
		dest = OIL_OFFSET(dest, dest_stride);	\
		src = OIL_OFFSET(src, src_stride);	\
	}				\
}					\
OIL_DEFINE_IMPL(conv_ ## desttype ## _ ## srctype ## _lrintf,	\
	conv_ ## desttype ## _ ## srctype);

CONV_DEFINE_FLOAT_LRINTF(s8,f32);
CONV_DEFINE_FLOAT_LRINTF(u8,f32);
CONV_DEFINE_FLOAT_LRINTF(s16,f32);
CONV_DEFINE_FLOAT_LRINTF(u16,f32);
CONV_DEFINE_FLOAT_LRINTF(s32,f32);
//CONV_DEFINE_FLOAT_LRINTF(u32,f32);

#endif

