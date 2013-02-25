// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfiler.h>
#include <moaicore/MOAIProfileScope.h>

//================================================================//
// MOAIProfileScope
//================================================================//

#if USE_MOAI_PROFILER
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", on ) 
#endif

//----------------------------------------------------------------//
MOAIProfileScope::MOAIProfileScope ( USHashedString& name ) :
	mName ( name ) {

	MOAIProfiler::Get ().EnterScope ( mName );
}

//----------------------------------------------------------------//
MOAIProfileScope::~MOAIProfileScope () {

	MOAIProfiler::Get ().LeaveScope ( mName );
}
	
#if defined ( MOAI_OS_WINDOWS )
#pragma optimize( "", off ) 
#endif

#endif