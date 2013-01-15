// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAITaskThread.h>

//================================================================//
// MOAITaskThread main
//================================================================//

//----------------------------------------------------------------//
void MOAITaskThread::_main ( void* param, MOAIThreadState& threadState ) {

	MOAITaskThread* taskThread = ( MOAITaskThread* )param;
	
	while ( threadState.IsRunning ()) {
		taskThread->Process ();
		MOAIThread::Sleep ();
	}
}

//================================================================//
// MOAITaskThread
//================================================================//

//----------------------------------------------------------------//
MOAITaskThread::MOAITaskThread () {

	RTTI_SINGLE ( MOAITaskQueue )
}

//----------------------------------------------------------------//
MOAITaskThread::~MOAITaskThread () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAITaskThread::PushTask ( MOAITask& task ) {

	MOAITaskQueue::PushTask ( task );
	this->mThread.Start ( _main, this, 0 );
}

//----------------------------------------------------------------//
void MOAITaskThread::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITaskQueue::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITaskThread::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITaskQueue::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAITaskThread::Stop () {

	this->mThread.Stop ();
	this->mThread.Join ();
}
