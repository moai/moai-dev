// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zipfs/zipfs_util.h>

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
size_t compare_paths ( const char* p0, const char* p1 ) {

	size_t i;
	size_t same = 0;

	for ( i = 0; p0 [ i ] && p1 [ i ]; ++i ) {
		
		char h = ( char )tolower ( p0 [ i ]);
		char v = ( char )tolower ( p1 [ i ]);
		
		if ( h != v ) break;
		
		if ( h == '/' ) {
			same = i + 1;
		}
	}

	return same;
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
