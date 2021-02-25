// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-core/ZLConditionVariable.h>
#include <zl-core/ZLConditionVariable_posix.h>
#include <zl-core/ZLConditionVariable_win32.h>

//================================================================//
// ZLMutex
//================================================================//

//----------------------------------------------------------------//
void ZLConditionVariable::Broadcast () {

	this->mConditionImpl->Broadcast ();
}

//----------------------------------------------------------------//
ZLConditionVariable::ZLConditionVariable () {
	
	this->mConditionImpl = new ZLConditionVariableImpl ();
}

//----------------------------------------------------------------//
ZLConditionVariable::~ZLConditionVariable () {

	delete this->mConditionImpl;
}

//----------------------------------------------------------------//
void ZLConditionVariable::Signal () {

	this->mConditionImpl->Signal ();
}

//----------------------------------------------------------------//
void ZLConditionVariable::Wait () {

	this->mConditionImpl->Wait ( *this->mMutexImpl );
}
