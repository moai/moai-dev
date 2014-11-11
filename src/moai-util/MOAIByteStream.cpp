// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIByteStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIByteStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIByteStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIByteStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIByteStream, "U" );
	
	if ( state.IsType ( 2, LUA_TSTRING )) {
		void* data;
		size_t size;
		data = ( void* )lua_tolstring ( state, 2, &size );
		self->Open ( data, size );
		
	}
	
	if ( state.IsType ( 2, LUA_TNUMBER )) {
		u32 size = state.GetValue < u32 >( 2, 0 );
		self->Open ( size );
	
	}
	return 0;
}

//================================================================//
// MOAIByteStream
//================================================================//

//----------------------------------------------------------------//
void MOAIByteStream::Close () {

	this->Clear ();

	if ( this->mData ) {
		free ( this->mData );
		this->mData = 0;
	}
}

//----------------------------------------------------------------//
MOAIByteStream::MOAIByteStream () :
	mData ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIByteStream::~MOAIByteStream () {
}

//----------------------------------------------------------------//
void MOAIByteStream::Open ( size_t size ) {

	this->Close ();
	if ( size ) {
		this->mData = calloc ( size, 1 );
		this->SetBuffer ( this->mData, size );
	}
}

//----------------------------------------------------------------//
void MOAIByteStream::Open ( void* data, size_t size ) {

	this->Open ( size );
	
	if ( data && size ) {
		memcpy ( this->mData, data, size );
	}
	this->SetLength ( size );
}

//----------------------------------------------------------------//
void MOAIByteStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIByteStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

