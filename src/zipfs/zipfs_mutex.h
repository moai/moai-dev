// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFS_MUTEX_H
#define ZIPFS_MUTEX_H

//================================================================//
// mutex
//================================================================//

typedef void ZIPFS_MUTEX;

//----------------------------------------------------------------//
extern ZIPFS_MUTEX*		zipfs_mutex_create			();
extern void				zipfs_mutex_destroy			( ZIPFS_MUTEX* mutex );
extern void				zipfs_mutex_lock			( ZIPFS_MUTEX* mutex );
extern void				zipfs_mutex_unlock			( ZIPFS_MUTEX* mutex );

#endif
