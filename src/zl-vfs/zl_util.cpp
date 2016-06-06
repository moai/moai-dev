// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#include <zl-vfs/zl_util.h>

//================================================================//
// util
//================================================================//

//----------------------------------------------------------------//
size_t zl_count_same ( char const* str0, char const* str1 ) {

	size_t c;
	for ( c = 0; str0 [ c ] && str1 [ c ] && ( str0 [ c ] == str1 [ c ]); ++c );
	return c;
}

//----------------------------------------------------------------//
size_t zl_count_same_nocase ( char const* str0, char const* str1 ) {

	size_t c;
	for ( c = 0; str0 [ c ] && str1 [ c ]; ++c ) {
	
		char h = ( char )tolower ( str0 [ c ]);
		char v = ( char )tolower ( str1 [ c ]);
		
		if ( h != v ) break;
	}
	return c;
}

//----------------------------------------------------------------//
int zl_strcmp_ignore_case	( char const* str0, char const* str1 ) {

	#ifdef MOAI_OS_WINDOWS
		return stricmp ( str0, str1 );
	#else
		return strcasecmp ( str0, str1 );
	#endif
}

//----------------------------------------------------------------//
char* zl_vsnprintf_alloc ( char* s, size_t n, const char* format, va_list arg ) {

	char* buffer = s;
	int buffSize = ( int )n;
	
	int result;
	
	for ( ;; ) {
		
		va_list copy;
		va_copy ( copy, arg );
		result = vsnprintf ( buffer, buffSize, format, copy );
		va_end ( copy );

		// thanks to http://perfec.to/vsnprintf/ for a discussion of vsnprintf portability issues
		if (( result == buffSize ) || ( result == -1 ) || ( result == buffSize - 1 ))  {
			buffSize = buffSize << 1;
		}
		else if ( result > buffSize ) {
			buffSize = ( size_t )result + 2;
		}
		else {
			break;
		}
		
		if ( buffer == s ) {
			buffer = 0;
		}
		
		if ( buffer ) {
			buffer = ( char* )realloc ( buffer, buffSize );
		}
		else {
			buffer = ( char* )malloc ( buffSize );
		}
	}

	return buffer;
}
