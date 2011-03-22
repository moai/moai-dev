// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/uslscore.h>

//----------------------------------------------------------------//
static void _cleanup () {

	USGlobals::Get ()->Finalize ();
}

//================================================================//
// uslscore
//================================================================//

//----------------------------------------------------------------//
void uslscore::InitGlobals ( USGlobals* globals ) {

	static bool sysInit = true;
	if ( sysInit ) {;

		#ifdef _WIN32
			_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
		#endif
		
		srand (( u32 )time ( 0 ));
		atexit ( _cleanup );

		sysInit = false;
	}

	USGlobals::Get ()->Set ( globals );

	USLuaRuntime::Get ();
}
