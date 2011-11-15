// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZIPFSVIRTUALPATH_H
#define ZIPFSVIRTUALPATH_H

#include <zipfs/ZIPFSZipFile.h>

//================================================================//
// ZIPFSVirtualPath
//================================================================//
typedef struct ZIPFSVirtualPath {

	char*			mPath;
	char*			mName;
	ZIPFSZipFile*	mArchive;

	struct ZIPFSVirtualPath* mNext;

} ZIPFSVirtualPath;

//----------------------------------------------------------------//
extern void					ZIPFSVirtualPath_Delete				( ZIPFSVirtualPath* self );
extern const char*			ZIPFSVirtualPath_GetLocalPath		( ZIPFSVirtualPath* self, const char* path );
extern ZIPFSVirtualPath*	ZIPFSVirtualPath_New				( void );
extern ZIPFSVirtualPath*	ZIPFSVirtualPath_PushFront			( ZIPFSVirtualPath* self, ZIPFSVirtualPath* list );
extern int					ZIPFSVirtualPath_SetArchive			( ZIPFSVirtualPath* self, const char* archive );
extern int					ZIPFSVirtualPath_SetPath			( ZIPFSVirtualPath* self, const char* path );

#endif