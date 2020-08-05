// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIAbstractDSLInterface.h>
#include <moai-core/MOAIAbstractDSLStream.h>

//================================================================//
// MOAIAbstractDSLStream
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDSLStream::MOAIAbstractDSLStream () :
	mDSLInterface ( NULL ) {

	RTTI_BEGIN ( MOAIAbstractDSLStream )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIAbstractDSLStream >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAbstractDSLStream::~MOAIAbstractDSLStream () {
}

//----------------------------------------------------------------//
void MOAIAbstractDSLStream::PushInterface ( MOAILuaState& state ) {

	if ( this->mLuaInterface ) {
		state.Push ( this->mLuaInterface );
	}
	else {
		state.PushPtrUserData (( MOAILuaObject* )this );
		this->mLuaInterface.SetRef ( state, -1 );
	}
	
	if ( this->mDSLInterface ) {
		this->mDSLInterface->PushLuaAPI ( this->mAPITypeID, state );
	}
	else {
		state.Push ();
	}
	lua_setmetatable ( state, -2 );
}

//----------------------------------------------------------------//
void MOAIAbstractDSLStream::SelectAPI ( MOAIAbstractDSLInterface* handler ) {

	this->mDSLInterface = handler;
	if ( handler ) {
		handler->InitializeBinding ( *this );
	}
	else {
		this->mAPITypeID = ZLType::NONE;
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIAbstractDSLStream::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIAbstractDSLStream::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "_",							MOAIAbstractDSLInterface::_ },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
