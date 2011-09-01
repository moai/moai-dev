// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaio\moaio_util.h>

//================================================================//
// util
//================================================================//

//----------------------------------------------------------------//
char* clear_string ( char* str ) {

	if ( str ) {
		free ( str );
	}
	return 0;
}

//----------------------------------------------------------------//
char* copy_string ( char const* str ) {

	if ( str ) {
		size_t size = strlen ( str ) + 1;

		if ( size ) {
			char* copy = ( char* )malloc ( size );
			strcpy ( copy, str );
			return copy;
		}
	}
	return 0;
}

