// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIOVIRTUALPATH_H
#define MOAIOVIRTUALPATH_H

#include <moaio/MOAIOZipFile.h>

//================================================================//
// MOAIOVirtualPath
//================================================================//
typedef struct MOAIOVirtualPath {

	char*			mPath;
	MOAIOZipFile*	mArchive;

	struct MOAIOVirtualPath* mNext;

} MOAIOVirtualPath;

//----------------------------------------------------------------//
extern void					MOAIOVirtualPath_Delete				( MOAIOVirtualPath* self );
extern const char*			MOAIOVirtualPath_GetLocalPath		( MOAIOVirtualPath* self, const char* path );
extern MOAIOVirtualPath*	MOAIOVirtualPath_New				();
extern MOAIOVirtualPath*	MOAIOVirtualPath_PushFront			( MOAIOVirtualPath* self, MOAIOVirtualPath* list );
extern int					MOAIOVirtualPath_SetArchive			( MOAIOVirtualPath* self, const char* archive );
extern int					MOAIOVirtualPath_SetPath			( MOAIOVirtualPath* self, const char* path );

#endif