// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfiler.h>
#include <moai-sim/MOAIProfilerAutoScope.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerAutoScope
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerAutoScope::EnterScope ( const ZLHashedString& name ) {

	this->mName = &name;
	MOAIProfiler::Get ().EnterScope ( *this->mName );
}

//----------------------------------------------------------------//
MOAIProfilerAutoScope::MOAIProfilerAutoScope () :
	mName ( 0 ) {
	
}

//----------------------------------------------------------------//
MOAIProfilerAutoScope::~MOAIProfilerAutoScope () {

	if ( this->mName ) {
		MOAIProfiler::Get ().LeaveScope ( *this->mName );
	}
}
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
