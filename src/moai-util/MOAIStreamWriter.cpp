// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStreamWriter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	close
	@text	Flush any remaining buffered data and detach the target stream.
			(This only detaches the target from the formatter; it does
			not also close the target stream).
	
	@in		MOAIStreamWriter self
	@out	nil
*/
int MOAIStreamWriter::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamWriter, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	openBase64
	@text	Open a base 64 formatted stream for writing (i.e. encode
			bytes to base64).
	
	@in		MOAIStreamWriter self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIStreamWriter::_openBase64 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamWriter, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	ZLBase64Writer* writer = new ZLBase64Writer ();
	
	bool result = self->Open ( stream, writer );
	
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	openDeflate
	@text	Open a 'deflate' formatted stream for writing (i.e. compress
			bytes using the 'deflate' algorithm).
	
	@in		MOAIStreamWriter self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIStreamWriter::_openDeflate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamWriter, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	int level		= state.GetValue < int >( 3, ZLDeflateWriter::DEFAULT_LEVEL );
	int windowBits	= state.GetValue < int >( 4, ZLDeflateWriter::DEFAULT_WBITS );
	
	ZLDeflateWriter* writer = new ZLDeflateWriter ();
	
	writer->SetCompressionLevel ( level );
	writer->SetWindowBits ( windowBits );
	
	bool result = self->Open ( stream, writer );
	
	state.Push ( result );
	return 1;
}

//================================================================//
// MOAIStreamWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamWriter::Close () {
	
	if ( this->mWriter ) {
		delete this->mWriter;
		this->mWriter = 0;
	}
	
	this->SetZLStream ( 0 );
	this->mStream.Set ( *this, 0 );
}

//----------------------------------------------------------------//
MOAIStreamWriter::MOAIStreamWriter () :
	mWriter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamWriter::~MOAIStreamWriter () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIStreamWriter::Open ( MOAIStream* stream, ZLStreamWriter* writer ) {

	this->Close ();
	this->mWriter = writer;

	if ( !stream ) return false;
	
	ZLStream* usStream = stream->GetZLStream ();
	if ( usStream ) {
		
		this->mStream.Set ( *this, stream );
		
		this->mWriter->Open ( *usStream );
		this->SetZLStream ( this->mWriter );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIStreamWriter::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_LEVEL", ZLDeflateWriter::DEFAULT_LEVEL );
	state.SetField ( -1, "DEFAULT_WBITS", ZLDeflateWriter::DEFAULT_WBITS );
}

//----------------------------------------------------------------//
void MOAIStreamWriter::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "openBase64",			_openBase64 },
		{ "openDeflate",		_openDeflate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

