// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIProfiler.h>
#include <moaicore/MOAIProfilerScope.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerScope
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerScope::Enter ( MOAIProfilerScope* parent, const USHashedString& name ) {

	this->mParent = parent;
	this->mName = name;

	USDeviceTime::GetTimeStamp ( this->mStartTime );
}

//----------------------------------------------------------------//
void MOAIProfilerScope::Leave ( const USHashedString& name ) {

	assert ( this->mName == name );
	
	USDeviceTime::TimeStamp timeLeft;
	USDeviceTime::GetTimeStamp ( timeLeft );
	this->mDuration = timeLeft - this->mStartTime;
}

//----------------------------------------------------------------//
MOAIProfilerScope::MOAIProfilerScope ( const USHashedString& name ) :
	mNext ( 0 ),
	mParent ( 0 ),
	mName ( name ),
	mStartTime ( 0 ),
	mDuration ( 0 ) {
}

//----------------------------------------------------------------//
void MOAIProfilerScope::Reset () {

	mNext = 0;
	mParent = 0;
	mStartTime = 0;
	mDuration = 0;
}

#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", off ) 
#endif
