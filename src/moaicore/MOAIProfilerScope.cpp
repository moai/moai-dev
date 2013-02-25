// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfiler.h>
#include <moaicore/MOAIProfilerScope.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif
	
#if USE_MOAI_PROFILER

//================================================================//
// MOAIProfilerScope
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerScope::Enter ( MOAIProfilerScope* parent, const USHashedString& name ) {

	this->mParent = parent;
	this->mName = name;

	USDeviceTime::GetTimeStamp ( this->mDuration );
}

//----------------------------------------------------------------//
void MOAIProfilerScope::Leave ( const USHashedString& name ) {

	assert ( this->mName == name );
	
	USDeviceTime::TimeStamp timeLeft;
	USDeviceTime::GetTimeStamp ( timeLeft );
	this->mDuration = timeLeft - this->mDuration;
}

//----------------------------------------------------------------//
MOAIProfilerScope::MOAIProfilerScope ( const USHashedString& name ) :
	mNext ( 0 ),
	mParent ( 0 ),
	mName ( name ),
	mDuration ( 0 ) {
}

//----------------------------------------------------------------//
void MOAIProfilerScope::Reset () {

	mNext = 0;
	mParent = 0;
	mDuration = 0;
}

#endif	

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
