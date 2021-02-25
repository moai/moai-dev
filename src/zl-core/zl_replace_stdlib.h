// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_STDLIB_H
#define ZL_REPLACE_STDLIB_H

#ifndef ZL_PRESERVE_STDLIB

#include <zl-core/zl_cstd.h>

#ifdef  __cplusplus
	extern "C" {
#endif

//================================================================//
// stdlib
//================================================================//

#undef calloc
#define calloc zl_calloc

#undef free
#define free zl_free

#undef malloc
#define malloc zl_malloc

#undef realloc
#define realloc zl_realloc

#ifndef MOAI_COMPILER_MSVC
	#undef system
	#define system zl_system
#endif

#ifdef  __cplusplus
	}
#endif

#endif
#endif
