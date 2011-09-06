// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>
#include <zipfs/ZIPFSString.h>

//================================================================//
// ZIPFSString
//================================================================//

//----------------------------------------------------------------//
char* ZIPFSString_Append ( ZIPFSString* self, const char* str ) {

	size_t size;
	char* append;

	size = strlen ( str );
	ZIPFSString_Grow ( self, self->mStrLen + size );
	
	append = &self->mMem [ self->mStrLen ];
	strcpy ( append, str );
	
	self->mStrLen += size;
	
	return self->mMem;
}

//----------------------------------------------------------------//
void ZIPFSString_Clear ( ZIPFSString* self ) {

	if ( self->mCleanup && self->mMem ) {
		free ( self->mMem );
	}
	memset ( self, 0, sizeof ( ZIPFSString ));
}

//----------------------------------------------------------------//
void ZIPFSString_Delete ( ZIPFSString* self ) {

	ZIPFSString_Clear ( self );
	free ( self );
}

//----------------------------------------------------------------//
char* ZIPFSString_Grow ( ZIPFSString* self, size_t size ) {

	size = size + 1;
	if ( size > self->mSize ) {
	
		char* buffer;
		
		size = (( size_t )( size / MOAIO_STRING_BLOCK_SIZE ) + 1 ) * MOAIO_STRING_BLOCK_SIZE;
		buffer = ( char* )malloc ( size );
		memcpy ( buffer, self->mMem, self->mSize );
		
		if ( self->mCleanup ) {
			free ( self->mMem );
		}
		
		self->mMem = buffer;
		self->mSize = size;
		self->mCleanup = 1;
	}
	return self->mMem;
}

//----------------------------------------------------------------//
ZIPFSString* ZIPFSString_New () {

	return ( ZIPFSString* )calloc ( 1, sizeof ( ZIPFSString ));
}

//----------------------------------------------------------------//
char ZIPFSString_PopChar ( ZIPFSString* self ) {

	char c = self->mMem [ --self->mStrLen ];
	self->mMem [ self->mStrLen ] = 0;
	return c;
}

//----------------------------------------------------------------//
void ZIPFSString_PushChar ( ZIPFSString* self, char c ) {

	ZIPFSString_Grow ( self, self->mStrLen + 1 );
	self->mMem [ self->mStrLen++ ] = c;
}

//----------------------------------------------------------------//
char* ZIPFSString_Set ( ZIPFSString* self, const char* str ) {

	self->mStrLen = strlen ( str );
	ZIPFSString_Grow ( self, self->mStrLen );
	strcpy ( self->mMem, str );
	
	return self->mMem;
}

//----------------------------------------------------------------//
void ZIPFSString_SetBuffer ( ZIPFSString* self, char* buffer, size_t size ) {

	self->mMem = buffer;
	self->mSize = size;
	self->mStrLen = 0;
	self->mCleanup = 0;
}

//----------------------------------------------------------------//
char* ZIPFSString_Shift ( ZIPFSString* self, size_t base, size_t length, size_t newbase ) {

	size_t i;

	if ( base == newbase ) return self->mMem;

	for ( i = base; self->mMem [ i ]; ++i );
	
	if ( i < length ) {
		length = i;
	}

	ZIPFSString_Grow ( self, newbase + length );
	
	if ( newbase < base ) {
		for ( i = 0; i < length; ++i ) {
			self->mMem [ newbase + i ] = self->mMem [ base + i ];
		}
	}
	else {
	
		size_t offset = length - 1;
		
		for ( i = 0; i < length; ++i ) {
			self->mMem [ newbase + offset - i ] = self->mMem [ base + offset - i ];
		}
	}

	return self->mMem;
}
