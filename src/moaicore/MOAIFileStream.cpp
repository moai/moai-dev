// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIFileStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFileStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFileStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIFileStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFileStream, "US" );
	
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	u32 mode		= state.GetValue < u32 >( 3, USFileStream::DEFAULT_FILE_MODE );
	
	bool result = self->Open ( filename, mode );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIFileStream
//================================================================//

//----------------------------------------------------------------//
void MOAIFileStream::Close () {

	this->mFileStream.Close ();
	this->SetStream ( 0 );
}

//----------------------------------------------------------------//
MOAIFileStream::MOAIFileStream () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFileStream::~MOAIFileStream () {
	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIFileStream::Open ( cc8* filename, u32 mode ) {

	this->Close ();
	bool result = this->mFileStream.Open ( filename, mode );
	this->SetStream ( &this->mFileStream );
	return result;
}

//----------------------------------------------------------------//
void MOAIFileStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );

	state.SetField ( -1, "READ",				( u32 )USFileStream::READ );
	state.SetField ( -1, "READ_WRITE",			( u32 )USFileStream::READ_WRITE );
	state.SetField ( -1, "READ_WRITE_AFFIRM",	( u32 )USFileStream::READ_WRITE_AFFIRM );
	state.SetField ( -1, "READ_WRITE_NEW",		( u32 )USFileStream::READ_WRITE_NEW );
	state.SetField ( -1, "WRITE",				( u32 )USFileStream::WRITE );
	
	state.SetField ( -1, "DEFAULT_FILE_MODE", ( u32 )USFileStream::DEFAULT_FILE_MODE );
}

//----------------------------------------------------------------//
void MOAIFileStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

