// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_MUTEX_H
#define ZL_MUTEX_H

//================================================================//
// mutex
//================================================================//

typedef void ZL_MUTEX;

//----------------------------------------------------------------//
extern ZL_MUTEX*		zl_mutex_create			();
extern void				zl_mutex_destroy		( ZL_MUTEX* mutex );
extern void				zl_mutex_lock			( ZL_MUTEX* mutex );
extern void				zl_mutex_unlock			( ZL_MUTEX* mutex );

#endif
