// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIHashWriter.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	close
	@text	Flush any remaining buffered data and detach the target stream.
			(This only detatches the target from the formatter; it does
			not also close the target stream). Return the hash.
	
	@in		MOAIStreamWriter self
	@out	string hash
*/
int MOAIHashWriter::_close ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	self->mWriter->Close ();
	u8* hash = self->mWriter->GetHash ();	
	
	// Convert to hex string
	char* hexStr = ( char* ) malloc ( 2 * MD5_DIGEST_LENGTH + 1 );
	char* hexPtr = hexStr;
	for ( int i = 0; i < MD5_DIGEST_LENGTH; ++i ) {
		hexPtr += sprintf ( hexPtr, "%02x", hash [ i ]);
	}
	*( hexPtr + 1 ) = '\0';
	
	lua_pushstring ( state, ( cc8* )hexStr );
	
	self->Close ();
	return 1;
}

//----------------------------------------------------------------//
/**	@name	openMD5
	@text	Open a MD5 hash stream for writing. (i.e. compute MD5
			hash of data while writing)
	
	@in		MOAIStreamWriter self
	@in		MOAIStream target
	@out	boolean success
*/
int MOAIHashWriter::_openMD5 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHashWriter, "U" );
	
	self->Close ();
	
	MOAIStream* stream = state.GetLuaObject < MOAIStream >( 2, true );
	if ( !stream ) return 0;
	
	USMD5Writer* writer = new USMD5Writer ();
	
	bool result = self->OpenMD5 ( stream, writer );
	
	state.Push ( result );
	return 1;
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
	
	this->SetUSStream ( 0 );
	this->mStream.Set ( *this, 0 );
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
bool MOAIHashWriter::OpenMD5 ( MOAIStream* stream, USMD5Writer* writer ) {

	this->Close ();
	this->mWriter = writer;

	if ( !stream ) return false;
	
	USStream* usStream = stream->GetUSStream ();
	if ( usStream ) {
		
		this->mStream.Set ( *this, stream );
		
		this->mWriter->Open ( *usStream );
		this->SetUSStream ( this->mWriter );
		
		return true;
	}
	return false;
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
		{ "openMD5",			_openMD5 },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

