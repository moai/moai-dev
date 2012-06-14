// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	setCallback
	@text	Sets the callback to be used when the asynchronous data IO operation completes.

	@in		MOAIDataIOAction self
	@in		function callback		The function to be called when the asynchronous operation is complete.  The MOAIDataBuffer object is passed as the first parameter.
	@out	nil
*/
int MOAIDataIOAction::_setCallback ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "UF" )) return 0;
	
	MOAIDataIOAction* self = state.GetLuaObject < MOAIDataIOAction >( 1, true );
	if ( !self ) return 0;

	self->SetLocal ( state, 2, self->mOnFinish );

	return 0;
}

//================================================================//
// MOAIDataIOAction
//================================================================//

//----------------------------------------------------------------//
void MOAIDataIOAction::Finished ( USDataIOTask* task ) {
	UNUSED ( task );

	if ( this->mOnFinish ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnFinish );
		this->mData->PushLuaUserdata ( state );
		state.DebugCall ( 1, 0 );
	}
	this->mState = DONE;
}

//----------------------------------------------------------------//
void MOAIDataIOAction::Init ( cc8* filename, MOAIDataBuffer* data ) {

	if ( this->mState != IDLE ) return;
	
	this->mFilename = filename;
	this->mData.Set ( *this, data );
}

//----------------------------------------------------------------//
bool MOAIDataIOAction::IsDone () {

	return ( this->mState == DONE );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::Load () {

	USTaskThread& taskThread = MOAISim::Get ().GetDataIOThread ();
	USDataIOTask* task = taskThread.NewTask < USDataIOTask >();
	
	task->LoadData ( this->mFilename, *this->mData );
	task->SetDelegate ( this, &MOAIDataIOAction::Finished );
	
	this->mState = BUSY;
}

//----------------------------------------------------------------//
MOAIDataIOAction::MOAIDataIOAction () :
	mState ( IDLE ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIDataIOAction::~MOAIDataIOAction () {

	this->mData.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::OnUpdate ( float step ) {
	UNUSED ( step );

	if ( this->mState == READY_LOAD ) {
		this->Load ();
	}

	if ( this->mState == READY_SAVE ) {
		this->Save ();
	}

	if ( this->mState == DONE ) {
		this->Stop ();
	}
}

//----------------------------------------------------------------//
void MOAIDataIOAction::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::Save () {

	USTaskThread& taskThread = MOAISim::Get ().GetDataIOThread ();
	USDataIOTask* task = taskThread.NewTask < USDataIOTask >();
	
	task->SaveData ( this->mFilename, *this->mData );
	task->SetDelegate ( this, &MOAIDataIOAction::Finished );
	
	this->mState = BUSY;
}

//----------------------------------------------------------------//
void MOAIDataIOAction::StartLoad () {

	this->mState = READY_LOAD;
	this->Start ();
}

//----------------------------------------------------------------//
void MOAIDataIOAction::StartSave () {

	this->mState = READY_SAVE;
	this->Start ();
}
