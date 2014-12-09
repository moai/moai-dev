// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_TYPES
#define ZL_TYPES

#ifdef __linux__
  #include <stddef.h>
#endif

//----------------------------------------------------------------//
typedef unsigned int			uint;
typedef size_t					uintptr;
typedef ptrdiff_t				sintptr;

typedef const char				cc8;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned long long		u64;

typedef signed char				s8;
typedef signed short			s16;
typedef signed int				s32;
typedef signed long long		s64;

#if ZL_SIZEOF_REAL == 4
	typedef float				real;
	typedef double				real_alt;
#elif ZL_SIZEOF_REAL == 8
	typedef double				real;
	typedef float				real_alt;
#else
	#error "ZL_SIZEOF_REAL must be 4 or 8"
#endif

#endif
