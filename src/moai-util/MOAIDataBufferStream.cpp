// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIDataBuffer.h>
#include <moai-util/MOAIDataBufferStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	close
	@text	Disassociates and unlocks the stream's MOAIDataBuffer.
	
	@in		MOAIDataBufferStream self
	@out	nil
*/
int MOAIDataBufferStream::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIDataBufferStream, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	open
	@text	Associate the stream with a MOAIDataBuffer. Note that the
			MOAIDataBuffer will be locked with a mutex while it is open
			thus blocking any asynchronous operations.
	
	@in		MOAIDataBufferStream self
	@in		MOAIDataBuffer buffer
	@out	boolean success
*/
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

	this->Clear ();

	if ( this->mDataBuffer ) {
		this->mDataBuffer->Unlock ();
		this->mDataBuffer.Set ( *this, 0 );
	}
}

//----------------------------------------------------------------//
MOAIDataBufferStream::MOAIDataBufferStream () {
	
	RTTI_BEGIN ( MOAIDataBufferStream )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIDataBufferStream >)
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIDataBufferStream::~MOAIDataBufferStream () {
}

//----------------------------------------------------------------//
bool MOAIDataBufferStream::Open ( MOAIDataBuffer* buffer ) {

	this->Close ();

	if ( !buffer ) return false;
	
	this->mDataBuffer.Set ( *this, buffer );
	
	void* bytes = 0;
	size_t size = 0;
	
	buffer->Lock ( &bytes, &size );
	
	this->SetBuffer ( buffer, size );
	this->SetLength ( size );
	
	return true;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIDataBufferStream::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIDataBufferStream::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "open",				_open },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

