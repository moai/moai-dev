// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIDataIOTask.h>

//================================================================//
// MOAIDataIOTask
//================================================================//

//----------------------------------------------------------------//
void MOAIDataIOTask::Execute () {

	if ( this->mAction == LOAD_ACTION ) { 
		this->mData->Load ( this->mFilename );
		
		if ( this->mInflateOnTaskThread ) {
			this->mData->Inflate ( this->mWindowBits );
		}
	}
	else if ( this->mAction == SAVE_ACTION ) {
		this->mData->Save ( this->mFilename );
	}
}

//----------------------------------------------------------------//
void MOAIDataIOTask::Init ( cc8* filename, MOAIDataBuffer& target, u32 action ) {

	this->mFilename = filename;
	this->mAction = action;
	this->mData = &target;
	
	this->mData->LuaRetain ( this->mData );
}

//----------------------------------------------------------------//
MOAIDataIOTask::MOAIDataIOTask () :
	mData ( 0 ),
	mAction ( NONE ),
	mInflateOnLoad ( false ),
	mInflateOnTaskThread ( false ),
	mWindowBits ( 0 ) {
}

//----------------------------------------------------------------//
MOAIDataIOTask::~MOAIDataIOTask () {

	this->mData->LuaRelease ( this->mData );
	this->mData = 0;
}

//----------------------------------------------------------------//
void MOAIDataIOTask::Publish () {

	if ( this->mInflateOnLoad && ( !this->mInflateOnTaskThread )) {
		this->mData->Inflate ( this->mWindowBits );
	}

	if ( this->mOnFinish ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnFinish.PushRef ( state )) {
			this->mData->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIDataIOTask::SetCallback ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	this->mOnFinish.SetRef ( state, idx );
}

//----------------------------------------------------------------//
void MOAIDataIOTask::SetInflateOnLoad ( bool inflateOnLoad, bool inflateOnTaskThread, int windowBits ) {

	this->mInflateOnLoad = inflateOnLoad;
	this->mInflateOnTaskThread = inflateOnTaskThread;
	this->mWindowBits = windowBits;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDataIOTask::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDataIOTask::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}
