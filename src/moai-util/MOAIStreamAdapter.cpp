// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIStreamAdapter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	close
	@text	Flush any remaining buffered data (if a write) and detach the target stream.
			(This only detaches the target from the formatter; it does
			not also close the target stream).
	
	@in		MOAIStreamWriter self
	@out	nil
*/
int MOAIStreamAdapter::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	self->Close ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openBase64
	@text	Open a base 64 formatted stream for reading (i.e. decode
			bytes from base64).
	
	@in		MOAIStreamAdapter self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIStreamAdapter::_openBase64Reader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	return self->Open < ZLBase64Reader >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	openBase64Writer
	@text	Open a base 64 formatted stream for writing (i.e. encode
			bytes to base64).
	
	@in		MOAIStreamAdapter self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIStreamAdapter::_openBase64Writer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	return self->Open < ZLBase64Writer >( state, 2 );
}

//----------------------------------------------------------------//
/**	@lua	openDeflate
	@text	Open a 'deflate' formatted stream for reading (i.e. decompress
			bytes using the 'deflate' algorithm).
	
	@in		MOAIStreamAdapter self
	@in		MOAIStream target
	@opt	number windowBits		The window bits used in the DEFLATE algorithm.
	@out	boolean success
*/
int MOAIStreamAdapter::_openDeflateReader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	int windowBits	= state.GetValue < int >( 3, ZLDeflateWriter::DEFAULT_WBITS );
	
	ZLDeflateReader* reader = new ZLDeflateReader ();
	
	reader->SetWindowBits ( windowBits );
	
	bool result = self->Open ( stream, reader );
	
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	openDeflateWriter
	@text	Open a 'deflate' formatted stream for writing (i.e. compress
			bytes using the 'deflate' algorithm).
	
	@in		MOAIStreamAdapter self
	@in		MOAIStream target
	@opt	number level			The level used in the DEFLATE algorithm.
	@opt	number windowBits		The window bits used in the DEFLATE algorithm.
	@out	boolean success
*/
int MOAIStreamAdapter::_openDeflateWriter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
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

//----------------------------------------------------------------//
/**	@lua	openHex
	@text	Open a hex formatted stream for reading and writing (i.e. decode
			and encode bytes to and from hex).
	
	@in		MOAIStreamReader self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIStreamAdapter::_openHex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	return self->Open < ZLHexAdapter >( state, 2 );
}

//================================================================//
// MOAIStreamAdapter
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamAdapter::Close () {
	
	if ( this->mAdapter ) {
		this->mAdapter->Close (); // would be called anyway by the destructor; doing it explicitly here
		delete this->mAdapter;
		this->mAdapter = 0;
	}
	
	this->SetZLStream ( 0 );
	this->mStream.Set ( *this, 0 );
}

//----------------------------------------------------------------//
MOAIStreamAdapter::MOAIStreamAdapter () :
	mAdapter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamAdapter::~MOAIStreamAdapter () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIStreamAdapter::Open ( MOAIStream* stream, ZLStreamAdapter* reader ) {

	this->Close ();
	this->mAdapter = reader;

	if ( !stream ) return false;
	
	ZLStream* zlStream = stream->GetZLStream ();
	if ( zlStream ) {
		
		this->mStream.Set ( *this, stream );
		
		this->mAdapter->Open ( zlStream );
		this->SetZLStream ( this->mAdapter );
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIStreamAdapter::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
	
	state.SetField ( -1, "DEFAULT_LEVEL", ZLDeflateWriter::DEFAULT_LEVEL );
	state.SetField ( -1, "DEFAULT_WBITS", ZLDeflateWriter::DEFAULT_WBITS );
}

//----------------------------------------------------------------//
void MOAIStreamAdapter::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "close",					_close },
		{ "openBase64Reader",		_openBase64Reader },
		{ "openBase64Writer",		_openBase64Writer },
		{ "openDeflateReader",		_openDeflateReader },
		{ "openDeflateWriter",		_openDeflateWriter },
		{ "openHex",				_openHex },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

