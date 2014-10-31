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
	
	return self->Open ( state, 2, new ZLBase64Reader ());
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
	
	return self->Open ( state, 2, new ZLBase64Writer ());
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
	
	bool result = self->Open ( reader, stream );
	
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
	
	bool result = self->Open ( writer, stream );
	
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
	
	return self->Open ( state, 2, new ZLHexAdapter ());
}

//================================================================//
// MOAIStreamAdapter
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamAdapter::Clear () {
	
	if ( this->mAdapter ) {
		delete this->mAdapter;
		this->mAdapter = 0;
	}
}

//----------------------------------------------------------------//
void MOAIStreamAdapter::Close () {
	
	if ( this->mAdapter ) {
		this->mAdapter->Close (); // would be called anyway by the destructor; doing it explicitly here
	}
	this->mAdaptedStream.Set ( *this, 0 );
	this->SetProxiedStream ( 0 );
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

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAIStreamAdapter::Open ( ZLStreamAdapter* adapter, MOAIStream* stream ) {

	this->Clear ();
	
	this->mAdapter = adapter;
	this->SetProxiedStream ( this->mAdapter );
	this->mAdaptedStream.Set ( *this, stream );
	
	bool result = this->mAdapter->Open ( this->mAdaptedStream );
	
	if ( !result ) {
		this->Close ();
	}
	return result;
}

//----------------------------------------------------------------//
int MOAIStreamAdapter::Open ( MOAILuaState& state, int idx, ZLStreamAdapter* adapter) {

	MOAIStream* stream = state.GetLuaObject < MOAIStream >( idx, true );

	bool result = this->Open ( adapter, stream );
	state.Push ( result );
	return 1;
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

