// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIFileStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	close
	@text	Close and release the associated file handle.
	
	@in		MOAIFileStream self
	@out	nil
*/
int MOAIFileStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFileStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	open
	@text	Open or create a file stream given a valid path.
	
	@in		MOAIFileStream self
	@in		string fileName
	@opt	number mode			One of MOAIFileStream.APPEND, MOAIFileStream.READ, MOAIFileStream.READ_WRITE, MOAIFileStream.READ_WRITE_AFFIRM,
								MOAIFileStream.READ_WRITE_NEW, MOAIFileStream.WRITE. Default value is MOAIFileStream.READ.
	@out	boolean success
*/
int MOAIFileStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFileStream, "US" );
	
	cc8* filename	= state.GetValue < cc8* >( 2, "" );
	u32 mode		= state.GetValue < u32 >( 3, ZLFileStream::READ );
	
	bool result = self->Open ( filename, mode );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIFileStream
//================================================================//

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
void MOAIFileStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );

	state.SetField ( -1, "APPEND",				( u32 )ZLFileStream::APPEND );
	state.SetField ( -1, "READ",				( u32 )ZLFileStream::READ );
	state.SetField ( -1, "READ_WRITE",			( u32 )ZLFileStream::READ_WRITE );
	state.SetField ( -1, "READ_WRITE_AFFIRM",	( u32 )ZLFileStream::READ_WRITE_AFFIRM );
	state.SetField ( -1, "READ_WRITE_NEW",		( u32 )ZLFileStream::READ_WRITE_NEW );
	state.SetField ( -1, "WRITE",				( u32 )ZLFileStream::WRITE );
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

