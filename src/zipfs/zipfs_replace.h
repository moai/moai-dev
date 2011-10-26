// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFS_REPLACE_H
#define ZIPFS_REPLACE_H

#ifdef  __cplusplus
	extern "C" {
#endif

#include <zipfs/zipfs.h>

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