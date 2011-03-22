// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIData.h>
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>setCallback (callback)</tt>\n
\n
	Sets the function to be called when the asynchronous operation is completed.
	@param callback The function to be called.  The MOAIData object is passed as the first parameter.
*/
int MOAIDataIOAction::_setCallback ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "UF" )) return 0;
	
	MOAIDataIOAction* self = state.GetLuaData < MOAIDataIOAction >( 1 );
	if ( !self ) return 0;

	self->mOnFinish.SetRef ( state, 2, false );

	return 0;
}

//================================================================//
// MOAIDataIOAction
//================================================================//

//----------------------------------------------------------------//
void MOAIDataIOAction::Init ( cc8* filename, MOAIData* data ) {

	if ( this->mState != IDLE ) return;
	
	this->mFilename = filename;
	this->mData = data;
	this->mState = READY;
}

//----------------------------------------------------------------//
bool MOAIDataIOAction::IsBusy () {

	return ( this->mState != DONE );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::Load () {

	USTaskThread& taskThread = MOAISim::Get ().GetDataIOThread ();
	USDataIOTask* task = taskThread.NewTask < USDataIOTask >();
	
	task->LoadData ( this->mFilename, *this->mData );
	task->SetDelegate ( this, &MOAIDataIOAction::LoadFinished );
	
	this->mState = LOADING;
}

//----------------------------------------------------------------//
void MOAIDataIOAction::LoadFinished ( USDataIOTask* task ) {
	UNUSED ( task );

	if ( this->mOnFinish ) {
	
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		this->mOnFinish.PushRef ( state );
		this->mData->PushLuaInstance ( state );
		state.DebugCall ( 1, 0 );
	}

	this->mState = DONE;
}

//----------------------------------------------------------------//
MOAIDataIOAction::MOAIDataIOAction () :
	mState ( IDLE ) {
	
	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIDataIOAction::~MOAIDataIOAction () {
}

//----------------------------------------------------------------//
void MOAIDataIOAction::OnUpdate ( float step ) {
	UNUSED ( step );

	if ( this->mState == READY ) {
		this->Load ();
	}

	if ( this->mState == DONE ) {
		this->Stop ();
	}
}

//----------------------------------------------------------------//
void MOAIDataIOAction::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIDataIOAction::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );

	LuaReg regTable [] = {
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
