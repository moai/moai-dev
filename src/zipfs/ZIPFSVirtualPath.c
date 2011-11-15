// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSVirtualPath.h>

//================================================================//
// ZIPFSVirtualPath
//================================================================//

//----------------------------------------------------------------//
void ZIPFSVirtualPath_Delete ( ZIPFSVirtualPath* self ) {
	
	if ( self->mArchive ) {
		ZIPFSZipFile_Delete ( self->mArchive );
	}
	clear_string ( self->mPath );
	free ( self );
}

//----------------------------------------------------------------//
const char* ZIPFSVirtualPath_GetLocalPath ( ZIPFSVirtualPath* self, const char* path ) {

	if ( self->mArchive ) {
		
		size_t baselen = strlen ( self->mPath );
		path = &path [ baselen ];
		
		if ( ZIPFSZipFile_FindDir ( self->mArchive, path )) return path;
	}
	return 0;
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* ZIPFSVirtualPath_New ( void ) {

	return ( ZIPFSVirtualPath* )calloc ( 1, sizeof ( ZIPFSVirtualPath ));
}

//----------------------------------------------------------------//
ZIPFSVirtualPath* ZIPFSVirtualPath_PushFront ( ZIPFSVirtualPath* self, ZIPFSVirtualPath* list ) {
	
	self->mNext = list;
	return self;
}

//----------------------------------------------------------------//
int ZIPFSVirtualPath_SetArchive ( ZIPFSVirtualPath* self, const char* archive ) {
	
	if ( self->mArchive ) {
		ZIPFSZipFile_Delete ( self->mArchive );
	}
	
	self->mArchive = ZIPFSZipFile_New ( archive );
	if ( !self->mArchive ) return -1;
	
	return 0;
}

//----------------------------------------------------------------//
int ZIPFSVirtualPath_SetPath ( ZIPFSVirtualPath* self, const char* path ) {

	size_t base = 0;
	size_t i = 0;
	size_t namelen = 0;

	clear_string ( self->mPath );
	self->mPath = copy_string ( path );
	
	clear_string ( self->mName );
	
	for ( ; path [ i ]; ++i ) {
		if (( path [ i ] == '/' ) && path [ i + 1 ]) {
			base = i + 1;
		}
	}
	
	namelen = i - base;
	if ( path [ i - 1 ] == '/' ) namelen--;

	self->mName = malloc ( namelen + 1 );
	memcpy ( self->mName, &path [ base ], namelen );
	self->mName [ namelen ] = 0;

	return 0;
}
