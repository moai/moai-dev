// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeflateReader.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeflateReader::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeflateReader, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeflateReader::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeflateReader, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	bool result = self->Open ( stream );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIDeflateReader
//================================================================//

//----------------------------------------------------------------//
void MOAIDeflateReader::Close () {
	
	if ( this->mStream ) {
		this->mReader.Close ();
		this->SetUSStream ( 0 );
		this->mStream.Set ( *this, 0 );
	}
}

//----------------------------------------------------------------//
MOAIDeflateReader::MOAIDeflateReader () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeflateReader::~MOAIDeflateReader () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIDeflateReader::Open ( MOAIStream* stream ) {

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
void MOAIDeflateReader::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDeflateReader::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

