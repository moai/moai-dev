// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaio\moaio_util.h>
#include <moaio\MOAIOVirtualPath.h>

//================================================================//
// MOAIOVirtualPath
//================================================================//

//----------------------------------------------------------------//
void MOAIOVirtualPath_Delete ( MOAIOVirtualPath* self ) {
	
	if ( self->mArchive ) {
		MOAIOZipFile_Delete ( self->mArchive );
	}
	clear_string ( self->mPath );
	free ( self );
}

//----------------------------------------------------------------//
const char* MOAIOVirtualPath_GetLocalPath ( MOAIOVirtualPath* self, const char* path ) {

	if ( self->mArchive ) {
		
		size_t baselen = strlen ( self->mPath );
		path = &path [ baselen ];
		
		if ( MOAIOZipFile_FindDir ( self->mArchive, path )) return path;
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIOVirtualPath* MOAIOVirtualPath_New () {

	return ( MOAIOVirtualPath* )calloc ( 1, sizeof ( MOAIOVirtualPath ));
}

//----------------------------------------------------------------//
MOAIOVirtualPath* MOAIOVirtualPath_PushFront ( MOAIOVirtualPath* self, MOAIOVirtualPath* list ) {
	
	self->mNext = list;
	return self;
}

//----------------------------------------------------------------//
int MOAIOVirtualPath_SetArchive ( MOAIOVirtualPath* self, const char* archive ) {
	
	if ( self->mArchive ) {
		MOAIOZipFile_Delete ( self->mArchive );
	}
	
	self->mArchive = MOAIOZipFile_New ( archive );
	if ( !self->mArchive ) return -1;
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIOVirtualPath_SetPath ( MOAIOVirtualPath* self, const char* path ) {

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
