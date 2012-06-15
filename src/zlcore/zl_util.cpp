// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlcore/zl_util.h>

//================================================================//
// util
//================================================================//

//----------------------------------------------------------------//
size_t count_same ( char const* str0, char const* str1 ) {

	size_t c;
	for ( c = 0; str0 [ c ] && str1 [ c ] && ( str0 [ c ] == str1 [ c ]); ++c );
	return c;
}

//----------------------------------------------------------------//
size_t count_same_nocase ( char const* str0, char const* str1 ) {

	size_t c;
	for ( c = 0; str0 [ c ] && str1 [ c ]; ++c ) {
	
		char h = ( char )tolower ( str0 [ c ]);
		char v = ( char )tolower ( str1 [ c ]);
		
		if ( h != v ) break;
	}
	return c;
}

//----------------------------------------------------------------//
int strcmp_ignore_case	( char const* str0, char const* str1 ) {

	#ifdef _WIN32
		return stricmp ( str0, str1 );
	#else
		return strcasecmp ( str0, str1 );
	#endif
}
