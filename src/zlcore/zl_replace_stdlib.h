// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_STDLIB_H
#define ZL_REPLACE_STDLIB_H

#include <zlcore/zlcore.h>

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

#ifdef  __cplusplus
	}
#endif

#endif