// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIOVIRTUALPATH_H
#define MOAIOVIRTUALPATH_H

//================================================================//
// MOAIOVirtualPath
//================================================================//
typedef struct MOAIOVirtualPath {

	char*	mPath;
	char*	mArchive;

	struct MOAIOVirtualPath* mNext;

} MOAIOVirtualPath;

//----------------------------------------------------------------//
extern void					MOAIOVirtualPath_Delete			( MOAIOVirtualPath* self );
extern MOAIOVirtualPath*	MOAIOVirtualPath_New			();
extern MOAIOVirtualPath*	MOAIOVirtualPath_PushFront		( MOAIOVirtualPath* self, MOAIOVirtualPath* list );
extern void					MOAIOVirtualPath_SetArchive		( MOAIOVirtualPath* self, const char* archive );
extern void					MOAIOVirtualPath_SetPath		( MOAIOVirtualPath* self, const char* path );

#endif