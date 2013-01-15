// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITask.h>
#include <moaicore/MOAITaskThread.h>

//================================================================//
// MOAITaskBase
//================================================================//

//----------------------------------------------------------------//
MOAITaskBase::MOAITaskBase () :
	mThread ( 0 ),
	mSubscriber ( 0 ),
	mPriority ( PRIORITY_HIGH ) {
}

//----------------------------------------------------------------//
MOAITaskBase::~MOAITaskBase () {
}

//----------------------------------------------------------------//
void MOAITaskBase::Start () {

	assert ( this->mThread );
	this->mThread->PushTask ( *this );
}
