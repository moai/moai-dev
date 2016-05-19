// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZL_UTIL_H
#define ZL_UTIL_H

//================================================================//
// util
//================================================================//

//----------------------------------------------------------------//
extern size_t	zl_count_same			( char const* str0, char const* str1 );
extern size_t	zl_count_same_nocase	( char const* str0, char const* str1 );
extern int		zl_strcmp_ignore_case	( char const* str0, char const* str1 );
extern char*	zl_vsnprintf_alloc		( char* s, size_t n, const char* format, va_list arg );

#endif
