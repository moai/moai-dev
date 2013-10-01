// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/headers.h>
#include <moai-crypto/MOAIHashWriterCrypto.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	openMD5
	@text	Open a MD5 hash stream for writing. (i.e. compute MD5
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openMD5 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterMD5 ());
}

//----------------------------------------------------------------//
/**	@name	openSHA1
	@text	Open a SHA1 hash stream for writing. (i.e. compute SHA1
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openSHA1 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterSHA1 ());
}

//----------------------------------------------------------------//
/**	@name	openSHA224
	@text	Open a SHA224 hash stream for writing. (i.e. compute SHA256
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openSHA224 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterSHA224 ());
}

//----------------------------------------------------------------//
/**	@name	openSHA256
	@text	Open a SHA256 hash stream for writing. (i.e. compute SHA256
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openSHA256 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterSHA256 ());
}

//----------------------------------------------------------------//
/**	@name	openSHA384
	@text	Open a SHA384 hash stream for writing. (i.e. compute SHA256
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openSHA384 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterSHA384 ());
}

//----------------------------------------------------------------//
/**	@name	openSHA512
	@text	Open a SHA512 hash stream for writing. (i.e. compute SHA256
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@opt	MOAIStream target
	@out	boolean success
*/
int MOAIHashWriterCrypto::_openSHA512 ( lua_State* L ) {
	return MOAIHashWriter::ImplementLuaHash ( L, new ZLHashWriterSHA512 ());
}

//================================================================//
// MOAIHashWriterCrypto
//================================================================//


//----------------------------------------------------------------//
MOAIHashWriterCrypto::MOAIHashWriterCrypto () :
	mWriter ( 0 ) {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIHashWriter )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHashWriterCrypto::~MOAIHashWriterCrypto () {
}

//----------------------------------------------------------------//
void MOAIHashWriterCrypto::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIHashWriter::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIHashWriterCrypto::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIHashWriter::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "openMD5",			_openMD5 },
		{ "openSHA1",			_openSHA1 },
		{ "openSHA224",			_openSHA224 },
		{ "openSHA256",			_openSHA256 },
		{ "openSHA384",			_openSHA384 },
		{ "openSHA512",			_openSHA512 },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
