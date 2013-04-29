// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_REPLACE_H
#define ZL_REPLACE_H

#include <zl-vfs/zl_replace_stdio.h>
#include <zl-vfs/zl_replace_stdlib.h>

#if !defined(ANDROID)
	#include <zl-vfs/zl_replace_new_delete.h>
#endif

#endif
