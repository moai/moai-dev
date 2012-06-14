// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USTask.h>
#include <uslscore/USTaskThread.h>

//================================================================//
// USTaskBase
//================================================================//

//----------------------------------------------------------------//
void USTaskBase::Start () {

	assert ( this->mThread );
	this->mThread->PushTask ( *this );
}

//----------------------------------------------------------------//
USTaskBase::USTaskBase () {
}

//----------------------------------------------------------------//
USTaskBase::~USTaskBase () {
}
