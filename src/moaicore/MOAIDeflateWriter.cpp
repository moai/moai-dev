// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDeflateWriter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeflateWriter::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeflateWriter, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDeflateWriter::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDeflateWriter, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	int level		= state.GetValue < int >( 3, USDeflateWriter::DEFAULT_LEVEL );
	int windowBits	= state.GetValue < int >( 4, USDeflateWriter::DEFAULT_WBITS );
	
	bool result = self->Open ( stream, level, windowBits );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIDeflateWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIDeflateWriter::Close () {
	
	if ( this->mStream ) {
		this->mWriter.Close ();
		this->SetUSStream ( 0 );
		this->mStream.Set ( *this, 0 );
	}
}

//----------------------------------------------------------------//
MOAIDeflateWriter::MOAIDeflateWriter () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDeflateWriter::~MOAIDeflateWriter () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIDeflateWriter::Open ( MOAIStream* stream, int level, int windowBits ) {

	this->Close ();

	if ( !stream ) return false;
	
	USStream* usStream = stream->GetUSStream ();
	if ( usStream ) {
	
		this->mWriter.Open ( *usStream, level, windowBits );
		this->mStream.Set ( *this, stream );
		this->SetUSStream ( &this->mWriter );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIDeflateWriter::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_LEVEL", USDeflateWriter::DEFAULT_LEVEL );
	state.SetField ( -1, "DEFAULT_WBITS", USDeflateWriter::DEFAULT_WBITS );
}

//----------------------------------------------------------------//
void MOAIDeflateWriter::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

