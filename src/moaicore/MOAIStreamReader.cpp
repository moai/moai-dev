// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIStreamReader.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStreamReader::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamReader, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStreamReader::_openBase64 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamReader, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	USBase64Reader* reader = new USBase64Reader ();
	
	bool result = self->Open ( stream, reader );
	
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStreamReader::_openDeflate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamReader, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	int windowBits	= state.GetValue < int >( 3, USDeflateWriter::DEFAULT_WBITS );
	
	USDeflateReader* reader = new USDeflateReader ();
	
	reader->SetWindowBits ( windowBits );
	
	bool result = self->Open ( stream, reader );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIStreamReader
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamReader::Close () {
	
	if ( this->mReader ) {
		delete this->mReader;
		this->mReader = 0;
	}
	
	this->SetUSStream ( 0 );
	this->mStream.Set ( *this, 0 );
}

//----------------------------------------------------------------//
MOAIStreamReader::MOAIStreamReader () :
	mReader ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamReader::~MOAIStreamReader () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIStreamReader::Open ( MOAIStream* stream, USStreamReader* reader ) {

	this->Close ();
	this->mReader = reader;

	if ( !stream ) return false;
	
	USStream* usStream = stream->GetUSStream ();
	if ( usStream ) {
		
		this->mStream.Set ( *this, stream );
		
		this->mReader->Open ( *usStream );
		this->SetUSStream ( this->mReader );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIStreamReader::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_WBITS", USDeflateReader::DEFAULT_WBITS );
}

//----------------------------------------------------------------//
void MOAIStreamReader::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "openBase64",			_openBase64 },
		{ "openDeflate",		_openDeflate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

