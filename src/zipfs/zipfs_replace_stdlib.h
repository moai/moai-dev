// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFS_REPLACE_STDLIB_H
#define ZIPFS_REPLACE_STDLIB_H

#include <zipfs/zipfs.h>

#ifdef  __cplusplus
	extern "C" {
#endif

//================================================================//
// stdlib
//================================================================//

#undef calloc
#define calloc zipfs_calloc

#undef free
#define free zipfs_free

#undef malloc
#define malloc zipfs_malloc

#undef realloc
#define realloc zipfs_realloc

#ifdef  __cplusplus
	}
#endif

#endif