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
	
	clear_string ( self->mPath );
	clear_string ( self->mArchive );
	free ( self );
}

//----------------------------------------------------------------//
MOAIOVirtualPath* MOAIOVirtualPath_New () {
	
	MOAIOVirtualPath* self = ( MOAIOVirtualPath* )malloc ( sizeof ( MOAIOVirtualPath ));
	
	self->mPath = 0;
	self->mArchive = 0;
	self->mNext = 0;
	
	return self;
}

//----------------------------------------------------------------//
MOAIOVirtualPath* MOAIOVirtualPath_PushFront ( MOAIOVirtualPath* self, MOAIOVirtualPath* list ) {
	
	self->mNext = list;
	return self;
}

//----------------------------------------------------------------//
void MOAIOVirtualPath_SetArchive ( MOAIOVirtualPath* self, const char* archive ) {

	self->mArchive = clear_string ( self->mArchive );
	self->mArchive = copy_string ( archive );
}

//----------------------------------------------------------------//
void MOAIOVirtualPath_SetPath ( MOAIOVirtualPath* self, const char* path ) {

	self->mPath = clear_string ( self->mPath );
	self->mPath = copy_string ( path );
}
