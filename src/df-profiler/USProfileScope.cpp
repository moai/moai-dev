// Copyright (c) 2012 Double Fine Productions, Inc. All Rights Reserved.

#include "pch.h"
#include <uslscore/USProfiler.h>
#include <uslscore/USProfileScope.h>

//================================================================//
// USProfileScope
//================================================================//

#if PROFILE_ENABLED
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

USProfileScope::USProfileScope ( USHashedString& name ) :
	mName ( name ) {

	USProfiler::Get ().EnterScope ( mName );
}

//----------------------------------------------------------------//
USProfileScope::~USProfileScope () {

	USProfiler::Get ().LeaveScope ( mName );
}
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif

#endif