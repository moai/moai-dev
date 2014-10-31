// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIMemStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	close
	@text	Close the memory stream and release its buffers.
	
	@in		MOAIMemStream self
	@out	nil
*/
int MOAIMemStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
	
	self->Clear ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIMemStream::_getString ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );

	size_t size = self->GetLength ();
	
	if ( size ) {
		
		size_t cursor = self->GetCursor ();
		
		self->Seek ( 0, SEEK_SET );
		void* str = 0;
		
		if ( size > ALLOCA_MAX ) {
			str = malloc ( size );
		}
		else {
			str = alloca ( size );
		}
		
		assert ( str );
		self->ReadBytes ( str, size );
		lua_pushlstring ( state, ( cc8* )str, size );
		
		if ( size > ALLOCA_MAX ) {
			free ( str );
		}
		
		self->Seek ( cursor, SEEK_SET );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	open
	@text	Create a memory stream and optionally reserve some memory and set
			the chunk size by which the stream will grow if additional memory
			is needed.
	
	@in		MOAIMemStream self
	@opt	number reserve			Default value is 0.
	@opt	number chunkSize		Default value is MOAIMemStream.DEFAULT_CHUNK_SIZE (2048 bytes).
	@out	boolean success
*/
int MOAIMemStream::_open ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
	
	self->Clear ();
	
	u32 reserve			= state.GetValue < u32 >( 2, 0 );
	u32 chunkSize		= state.GetValue < u32 >( 3, ZLMemStream::DEFAULT_CHUNK_SIZE );
	
	bool result = false;

	if ( chunkSize ) {
		self->SetChunkSize ( chunkSize );
		self->Reserve ( reserve );
		result = true;
	}
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIMemStream
//================================================================//

//----------------------------------------------------------------//
MOAIMemStream::MOAIMemStream () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMemStream::~MOAIMemStream () {
}

//----------------------------------------------------------------//
void MOAIMemStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_CHUNK_SIZE", ( u32 )ZLMemStream::DEFAULT_CHUNK_SIZE );
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

