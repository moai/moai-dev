// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIDataBufferStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBufferStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBufferStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBufferStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBufferStream, "UU" );
	
	self->Close ();
	
	MOAIDataBuffer* buffer = state.GetLuaObject < MOAIDataBuffer >( 2, true );
	if ( !buffer ) return 0;
	
	bool result = self->Open ( buffer );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIDataBufferStream
//================================================================//

//----------------------------------------------------------------//
void MOAIDataBufferStream::Close () {

	if ( this->mDataBuffer ) {
		
		this->mByteStream.SetBuffer ( 0, 0 );
		this->mByteStream.SetLength ( 0 );
		
		this->SetStream ( 0 );
		this->mDataBuffer.Set ( *this, 0 );
		this->mDataBuffer->Unlock ();
	}
}

//----------------------------------------------------------------//
MOAIDataBufferStream::MOAIDataBufferStream () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDataBufferStream::~MOAIDataBufferStream () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIDataBufferStream::Open ( MOAIDataBuffer* buffer ) {

	this->Close ();

	if ( !buffer ) return false;
	
	this->mDataBuffer.Set ( *this, buffer );
	
	void* bytes = 0;
	size_t size = 0;
	
	buffer->Lock ( &bytes, &size );
	
	this->mByteStream.SetBuffer ( buffer, size );
	this->mByteStream.SetLength ( size );
	
	this->SetStream ( &this->mByteStream );
	
	return true;
}

//----------------------------------------------------------------//
void MOAIDataBufferStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIDataBufferStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

