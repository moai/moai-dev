// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBase64Writer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIBase64Writer::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBase64Writer, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIBase64Writer::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBase64Writer, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	bool result = self->Open ( stream );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIBase64Writer
//================================================================//

//----------------------------------------------------------------//
void MOAIBase64Writer::Close () {
	
	if ( this->mStream ) {
		this->mWriter.Close ();
		this->SetUSStream ( 0 );
		this->mStream.Set ( *this, 0 );
	}
}

//----------------------------------------------------------------//
MOAIBase64Writer::MOAIBase64Writer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBase64Writer::~MOAIBase64Writer () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIBase64Writer::Open ( MOAIStream* stream ) {

	this->Close ();

	if ( !stream ) return false;
	
	USStream* usStream = stream->GetUSStream ();
	if ( usStream ) {
	
		this->mWriter.Open ( *usStream );
		this->mStream.Set ( *this, stream );
		this->SetUSStream ( &this->mWriter );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIBase64Writer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBase64Writer::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

