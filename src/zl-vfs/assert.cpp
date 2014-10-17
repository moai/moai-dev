// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-vfs/assert.h>
#include <zl-vfs/headers.h>

//================================================================//
// assert
//================================================================//

jmp_buf* sAssertEnv = 0;

//----------------------------------------------------------------//
void zl_handle_assert ( const char* condition, const char* funcname, const char* filename, int lineno ) {

	zl_printf ( "Assertion (%s) failed in function %s, %s (%d)\n", condition, funcname, filename, lineno );
	
	if ( sAssertEnv ) {
		longjmp ( *sAssertEnv, 1 );
	}
	#ifndef NDEBUG
		abort ();
	#endif
}

//----------------------------------------------------------------//
jmp_buf* zl_set_assert_jmp_buf ( jmp_buf* env ) {

	if ( sAssertEnv ) {
		free ( sAssertEnv );
	}
	sAssertEnv = env;
	return env;
}
