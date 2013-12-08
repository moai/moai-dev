// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIProfiler.h>
#include <moai-sim/MOAIProfilerScope.h>
	
#if defined ( MOAI_OS_WINDOWS )
	#pragma optimize ( "", on ) 
#endif

//================================================================//
// MOAIProfilerScope
//================================================================//

//----------------------------------------------------------------//
void MOAIProfilerScope::Enter ( MOAIProfilerScope* parent, const ZLHashedString& name ) {

	this->mParent = parent;
	this->mName = name;

	ZLDeviceTime::GetTimeStamp ( this->mStartTime );
}

//----------------------------------------------------------------//
void MOAIProfilerScope::Leave ( const ZLHashedString& name ) {

	assert ( this->mName == name );
	
	ZLDeviceTime::TimeStamp timeLeft;
	ZLDeviceTime::GetTimeStamp ( timeLeft );
	this->mDuration = timeLeft - this->mStartTime;
}

//----------------------------------------------------------------//
MOAIProfilerScope::MOAIProfilerScope ( const ZLHashedString& name ) :
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
