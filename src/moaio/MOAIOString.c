// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaio\moaio_util.h>
#include <moaio\MOAIOString.h>

//================================================================//
// MOAIOString
//================================================================//

//----------------------------------------------------------------//
char* MOAIOString_Append ( MOAIOString* self, const char* str ) {

	size_t size;
	char* append;

	size = strlen ( str );
	MOAIOString_Grow ( self, self->mStrLen + size );
	
	append = &self->mMem [ self->mStrLen ];
	strcpy ( append, str );
	
	self->mStrLen += size;
	
	return self->mMem;
}

//----------------------------------------------------------------//
void MOAIOString_Delete ( MOAIOString* self ) {

	if ( self->mMem ) {
		free ( self->mMem );
	}
	free ( self );
}

//----------------------------------------------------------------//
char* MOAIOString_Grow ( MOAIOString* self, size_t size ) {

	size = size + 1;
	if ( size > self->mSize ) {		
		size = (( size_t )( size / MOAIO_STRING_BLOCK_SIZE ) + 1 ) * MOAIO_STRING_BLOCK_SIZE;
		self->mMem = realloc ( self->mMem, size );
		self->mSize = size;
	}
	return self->mMem;
}

//----------------------------------------------------------------//
MOAIOString* MOAIOString_New () {

	MOAIOString* self = ( MOAIOString* )malloc ( sizeof ( MOAIOString ));
	
	self->mMem = 0;
	self->mSize = 0;
	self->mStrLen = 0;
	
	return self;
}

//----------------------------------------------------------------//
char* MOAIOString_Set ( MOAIOString* self, const char* str ) {

	self->mStrLen = strlen ( str );
	MOAIOString_Grow ( self, self->mStrLen );
	strcpy ( self->mMem, str );
	
	return self->mMem;
}

//----------------------------------------------------------------//
char* MOAIOString_Shift ( MOAIOString* self, size_t base, size_t length, size_t newbase ) {

	size_t i;

	if ( base == newbase ) return self->mMem;

	for ( i = base; self->mMem [ i ]; ++i );
	
	if ( i < length ) {
		length = i;
	}

	MOAIOString_Grow ( self, newbase + length );
	
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
