// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBase64Reader.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIBase64Reader::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBase64Reader, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIBase64Reader::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBase64Reader, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	bool result = self->Open ( stream );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIBase64Reader
//================================================================//

//----------------------------------------------------------------//
void MOAIBase64Reader::Close () {
	
	if ( this->mStream ) {
		this->mReader.Close ();
		this->SetUSStream ( 0 );
		this->mStream.Set ( *this, 0 );
	}
}

//----------------------------------------------------------------//
MOAIBase64Reader::MOAIBase64Reader () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBase64Reader::~MOAIBase64Reader () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIBase64Reader::Open ( MOAIStream* stream ) {

	this->Close ();

	if ( !stream ) return false;
	
	USStream* usStream = stream->GetUSStream ();
	if ( usStream ) {
	
		this->mReader.Open ( *usStream );
		this->mStream.Set ( *this, stream );
		this->SetUSStream ( &this->mReader );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIBase64Reader::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBase64Reader::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

