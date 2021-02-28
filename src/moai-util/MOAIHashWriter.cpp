// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIHashWriter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getChecksum
	@text	Return the checksum (if supported).
	
	@in		MOAIHashWriter self
	@out	number checksum
*/
int MOAIHashWriter::_getChecksum ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	ZLHashWriter* hashWriter = self->GetHashWriter ();

	if ( hashWriter ) {
		u32 checksum = hashWriter->GetChecksum ();
		lua_pushnumber ( state, checksum );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getHash
	@text	Return the hash (if supported).
	
	@in		MOAIHashWriter self
	@out	string hash
*/
int MOAIHashWriter::_getHash ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	ZLHashWriter* hashWriter = self->GetHashWriter ();

	if (hashWriter ) {
		u8* hash = ( u8* )self->mHashWriter->GetHash ();
		lua_pushlstring ( state, ( cc8* )hash, hashWriter->GetHashSize ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getHashBase64
	@text	Return the hash, converted to base64.
	
	@in		MOAIHashWriter self
	@out	string hash64
*/
int MOAIHashWriter::_getHashBase64 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	ZLHashWriter* hashWriter = self->GetHashWriter ();

	if ( hashWriter ) {
		STLString hash;
		hash.base_64_encode ( hashWriter->GetHash (), hashWriter->GetHashSize ());
		lua_pushstring ( state, hash );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getHashBase64
	@text	Return the hash, converted to hexadecimal.
	
	@in		MOAIHashWriter self
	@out	string hashHex
*/
int MOAIHashWriter::_getHashHex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	ZLHashWriter* hashWriter = self->GetHashWriter ();

	if ( hashWriter ) {
	
		STLString hash;
		hash.hex_encode ( hashWriter->GetHash (), hashWriter->GetHashSize ());
		lua_pushstring ( state, hash );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openAdler32
	@text	Open a Adler32 hash stream for writing. (i.e. compute Adler32
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openAdler32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	self->mHashWriter = new ZLHashWriterAdler32 ();
	return self->Open ( state, 2, self->mHashWriter );
}

//----------------------------------------------------------------//
/**	@lua	openCRC32
	@text	Open a CRC32 hash stream for writing. (i.e. compute CRC32
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openCRC32 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	ZLHashWriterCRC32* writer = new ZLHashWriterCRC32 ();
	writer->SetAlgorithm ( ZLHashWriterCRC32::CRC32 );
	self->mHashWriter = writer;
	
	return self->Open ( state, 2, writer );
}

//----------------------------------------------------------------//
/**	@lua	openCRC32b
	@text	Open a CRC32b hash stream for writing. (i.e. compute CRC32b
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openCRC32b ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	ZLHashWriterCRC32* writer = new ZLHashWriterCRC32 ();
	writer->SetAlgorithm ( ZLHashWriterCRC32::CRC32B );
	self->mHashWriter = writer;
	
	return self->Open ( state, 2, writer );
}

//----------------------------------------------------------------//
/**	@lua	openWhirlpool
	@text	Open a Whirlpool hash stream for writing. (i.e. compute Whirlpool
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openWhirlpool ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	self->Close ();

	#ifndef MOAI_OS_NACL
		self->mHashWriter = new ZLHashWriterWhirlpool ();
		return self->Open ( state, 2, self->mHashWriter );
	#else
		state.Push ( false );
		return 1;
	#endif
}

//----------------------------------------------------------------//
/**	@lua	setHMACKey
	@text	Set the HMAC key (if supported).
	
	@in		MOAIHashWriter self
	@in		string hmac
	@out	nil
*/
int MOAIHashWriter::_setHMACKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "US" );
	
	ZLHashWriter* hashWriter = self->GetHashWriter ();
	
	if ( hashWriter ) {
		void* hmacKey;
		size_t hmacKeySize = 0;
		
		hmacKey = ( void* )lua_tolstring ( state, 2, &hmacKeySize );
		hashWriter->SetHMACKey ( hmacKey, hmacKeySize );
	}
	return 0;
}

//================================================================//
// MOAIStreamWriter
//================================================================//

//----------------------------------------------------------------//
ZLHashWriter* MOAIHashWriter::GetHashWriter () {

	return this->mAdapter ? this->mHashWriter : 0;
}

//----------------------------------------------------------------//
MOAIHashWriter::MOAIHashWriter ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIStream ( context ),
	MOAIStreamAdapter ( context ),
	mHashWriter ( 0 ) {
	
	RTTI_BEGIN ( MOAIHashWriter )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIHashWriter >)
		RTTI_EXTEND ( MOAIStreamAdapter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHashWriter::~MOAIHashWriter () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHashWriter::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIHashWriter::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "close",				_close },
		{ "getChecksum",		_getChecksum },
		{ "getHash",			_getHash },
		{ "getHashBase64",		_getHashBase64 },
		{ "getHashHex",			_getHashHex },
		{ "openAdler32",		_openAdler32 },
		{ "openCRC32",			_openCRC32 },
		{ "openCRC32b",			_openCRC32b },
		{ "openWhirlpool",		_openWhirlpool },
		{ "setHMACKey",			_setHMACKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
