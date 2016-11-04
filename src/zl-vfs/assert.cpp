// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-vfs/assert.h>
#include <zl-vfs/headers.h>

//================================================================//
// assert
//================================================================//

ZLThreadLocalPtr < jmp_buf > sAssertEnv;

//----------------------------------------------------------------//
void zl_handle_assert ( const char* condition, const char* funcname, const char* filename, int lineno ) {

	zl_printf ( "Assertion (%s) failed in function %s, %s (%d)\n", condition, funcname, filename, lineno );
	
	jmp_buf* assertEnv = sAssertEnv.Get ();
	
	if ( assertEnv ) {
		longjmp ( *assertEnv, 1 );
	}
	#ifndef NDEBUG
		abort ();
	#endif
}

//----------------------------------------------------------------//
jmp_buf* zl_set_assert_jmp_buf ( jmp_buf* env ) {

	jmp_buf* assertEnv = sAssertEnv.Get ();

	if ( assertEnv ) {
		free ( assertEnv );
	}
	sAssertEnv.Set ( env );
	return env;
}
