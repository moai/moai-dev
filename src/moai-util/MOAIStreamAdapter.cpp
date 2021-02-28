// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	
	ZLResultCode result = self->Open ( reader, stream );
	
	state.Push ( result == ZL_OK );
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
	
	ZLResultCode result = self->Open ( writer, stream );
	
	state.Push ( result == ZL_OK );
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

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIStreamAdapter::_openRing ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIStreamAdapter, "U" );
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	u32 size = state.GetValue < u32 >( 3, ( u32 )stream->GetLength ());
	
	if ( size > 0 ) {
	
		ZLRingAdapter* adapter = new ZLRingAdapter ();
		ZLResultCode result = self->Open ( adapter, stream );
		
		if ( result == ZL_OK ) {
			adapter->SetLength ( size );
			state.Push ( result );
			return 1;
		}
	}
	return 0;
}

//================================================================//
// MOAIStreamAdapter
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamAdapter::Clear () {
	
	this->Close ();
	
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
MOAIStreamAdapter::MOAIStreamAdapter ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIStream ( context ),
	mAdapter ( 0 ) {
	
	RTTI_BEGIN ( MOAIStreamAdapter )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIStreamAdapter >)
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIStreamAdapter::~MOAIStreamAdapter () {

	this->Clear ();
}

//----------------------------------------------------------------//
ZLResultCode MOAIStreamAdapter::Open ( ZLStreamAdapter* adapter, MOAIStream* stream ) {

	this->Clear ();
	
	this->mAdapter = adapter;
	this->SetProxiedStream ( this->mAdapter );
	this->mAdaptedStream.Set ( *this, stream );
	
	ZLResultCode result = this->mAdapter->Open ( this->mAdaptedStream );
	
	if ( result != ZL_OK ) {
		this->Close ();
	}
	return result;
}

//----------------------------------------------------------------//
int MOAIStreamAdapter::Open ( MOAILuaState& state, int idx, ZLStreamAdapter* adapter) {

	MOAIStream* stream = state.GetLuaObject < MOAIStream >( idx, true );

	ZLResultCode result = this->Open ( adapter, stream );
	state.Push ( result == ZL_OK );
	return 1;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIStreamAdapter::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "DEFAULT_LEVEL", ZLDeflateWriter::DEFAULT_LEVEL );
	state.SetField ( -1, "DEFAULT_WBITS", ZLDeflateWriter::DEFAULT_WBITS );
}

//----------------------------------------------------------------//
void MOAIStreamAdapter::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "close",					_close },
		{ "openBase64Reader",		_openBase64Reader },
		{ "openBase64Writer",		_openBase64Writer },
		{ "openDeflateReader",		_openDeflateReader },
		{ "openDeflateWriter",		_openDeflateWriter },
		{ "openHex",				_openHex },
		{ "openRing",				_openRing },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

