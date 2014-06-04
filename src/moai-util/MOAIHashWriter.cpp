// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-util/MOAIHashWriter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	close
	@text	Flush any remaining buffered data and detach the target stream.
			(This only detaches the target from the formatter; it does
			not also close the target stream). Return the hash as a hex
			string.
	
	@in		MOAIHashWriter self
	@out	string hash
*/
int MOAIHashWriter::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	self->mWriter->Close ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHashWriter::_getChecksum ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	if ( self->mWriter ) {
		u32 checksum = self->mWriter->GetChecksum ();
		lua_pushnumber ( state, checksum );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHashWriter::_getHash ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	if ( self->mWriter ) {
		u8* hash = ( u8* )self->mWriter->GetHash ();
		lua_pushlstring ( state, ( cc8* )hash, self->mWriter->GetHashSize ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHashWriter::_getHashBase64 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	if ( self->mWriter ) {
		STLString hash;
		hash.base_64_encode ( self->mWriter->GetHash (), self->mWriter->GetHashSize ());
		lua_pushstring ( state, hash );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHashWriter::_getHashHex ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );

	if ( self->mWriter ) {
	
		self->mWriter->Close ();
	
		u8* hash = ( u8* )self->mWriter->GetHash ();	
		size_t hashSize = self->mWriter->GetHashSize ();
		
		// Convert to hex string
		char* hexStr = ( char* )alloca (( hashSize * 2 ) + 1 );
		char* hexPtr = hexStr;
		for ( size_t i = 0; i < hashSize; ++i ) {
			hexPtr += sprintf ( hexPtr, "%02x", hash [ i ]);
		}
		
		lua_pushstring ( state, ( cc8* )hexStr );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	openAdler32
	@text	Open a Adler32 hash stream for writing. (i.e. compute Adler32
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openAdler32 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterAdler32 ());
}

//----------------------------------------------------------------//
/**	@name	openCRC32
	@text	Open a CRC32 hash stream for writing. (i.e. compute CRC32
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openCRC32 ( lua_State* L ) {
	ZLHashWriterCRC32* writer = new ZLHashWriterCRC32 ();
	writer->SetAlgorithm ( ZLHashWriterCRC32::CRC32 );
	return MOAIHashWriter::ImplementLuaHash ( L, writer );
}

//----------------------------------------------------------------//
/**	@name	openCRC32b
	@text	Open a CRC32b hash stream for writing. (i.e. compute CRC32b
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openCRC32b ( lua_State* L ) {
	ZLHashWriterCRC32* writer = new ZLHashWriterCRC32 ();
	writer->SetAlgorithm ( ZLHashWriterCRC32::CRC32B );
	return MOAIHashWriter::ImplementLuaHash ( L, writer );
}

//----------------------------------------------------------------//
/**	@name	openWhirlpool
	@text	Open a Whirlpool hash stream for writing. (i.e. compute Whirlpool
			hash of data while writing)
	
	@in		MOAIHashWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openWhirlpool ( lua_State* L ) {
	#ifndef MOAI_OS_NACL
		return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterWhirlpool ());
	#else
		return 0;
	#endif
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHashWriter::_setHMACKey ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "US" );
	
	if ( self->mWriter ) {
		void* hmacKey;
		size_t hmacKeySize = 0;
		
		hmacKey = ( void* )lua_tolstring ( state, 2, &hmacKeySize );
		self->mWriter->SetHMACKey ( hmacKey, hmacKeySize );
	}
	return 0;
}

//================================================================//
// MOAIStreamWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIHashWriter::Close () {
	
	if ( this->mWriter ) {
		delete this->mWriter;
		this->mWriter = 0;
	}
	
	this->SetZLStream ( 0 );
	this->mStream.Set ( *this, 0 );
}

//----------------------------------------------------------------//
int MOAIHashWriter::ImplementLuaHash ( lua_State* L, ZLHashWriter* writer ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	assert ( writer );
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	bool result = self->Open ( stream, writer );
	
	state.Push ( result );
	return 1;
}

//----------------------------------------------------------------//
MOAIHashWriter::MOAIHashWriter () :
	mWriter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHashWriter::~MOAIHashWriter () {

	this->Close ();
}

//----------------------------------------------------------------//
bool MOAIHashWriter::Open ( MOAIStream* stream, ZLHashWriter* writer ) {

	this->Close ();
	this->mWriter = writer;
	
	ZLStream* usStream = stream ? stream->GetZLStream () : 0;
	
	if ( usStream ) {
		this->mStream.Set ( *this, stream );
	}
	
	this->mWriter->Open ( *usStream );
	this->SetZLStream ( this->mWriter );
	
	return true;
}

//----------------------------------------------------------------//
void MOAIHashWriter::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHashWriter::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

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
