// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USTask.h>
#include <uslsext/USTaskThread.h>

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
