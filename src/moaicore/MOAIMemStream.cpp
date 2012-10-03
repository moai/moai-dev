// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMemStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	close
	@text	Close the mem stream and release its buffers.
	
	@in		MOAIMemStream self
	@out	nil
*/
int MOAIMemStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMemStream::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );

	size_t size = self->mMemStream.GetLength ();
	
	if ( size ) {
		
		size_t cursor = self->mMemStream.GetCursor ();
		
		self->mMemStream.Seek ( 0, SEEK_SET );
		void* str = 0;
		
		if ( size > ALLOCA_MAX ) {
			str = malloc ( size );
		}
		else {
			str = alloca ( size );
		}
		
		assert ( str );
		self->mMemStream.ReadBytes ( str, size );
		lua_pushlstring ( state, ( cc8* )str, size );
		
		if ( size > ALLOCA_MAX ) {
			free ( str );
		}
		
		self->mMemStream.Seek ( cursor, SEEK_SET );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	open
	@text	Create a mem stream and optionally reserve some memory and set
			the chunk size by which the stream will grow if additional memory
			is needed.
	
	@in		MOAIMemStream self
	@opt	number reserve			Default value is 0.
	@opt	number chunkSize		Default value is MOAIMemStream.DEFAULT_CHUNK_SIZE (2048 bytes).
	@out	boolean success
*/
int MOAIMemStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
	
	u32 reserve			= state.GetValue < u32 >( 2, 0 );
	u32 chunkSize		= state.GetValue < u32 >( 3, USMemStream::DEFAULT_CHUNK_SIZE );
	
	bool result = self->Open ( reserve, chunkSize );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIMemStream
//================================================================//

//----------------------------------------------------------------//
void MOAIMemStream::Close () {

	this->SetUSStream ( 0 );
	this->mMemStream.Clear ();
}

//----------------------------------------------------------------//
MOAIMemStream::MOAIMemStream () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMemStream::~MOAIMemStream () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIMemStream::Open ( u32 reserve, u32 chunkSize ) {

	this->Close ();

	if ( !chunkSize ) return false;

	this->mMemStream.SetChunkSize ( chunkSize );
	this->mMemStream.Reserve ( reserve );

	this->SetUSStream ( &this->mMemStream );

	return true;
}

//----------------------------------------------------------------//
void MOAIMemStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_CHUNK_SIZE", USMemStream::DEFAULT_CHUNK_SIZE );
}

//----------------------------------------------------------------//
void MOAIMemStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "getString",			_getString },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

