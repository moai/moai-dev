// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAIDataBuffer.h>

//================================================================//
// MOAIDataBuffer
//================================================================//

//----------------------------------------------------------------//
/**	@name base64Decode
	@param1 self|data @type userdata|string @text The string to decode, or a reference to the current object (self).
	@text If a string is provided, decodes it as a base64 encoded string.  Otherwise, decodes the current data stored in this object as a base64 encoded sequence of characters.
	@return If passed a string, returns either a string or nil depending on whether it could be decoded.  Otherwise the decoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_base64Decode ( lua_State* L ) {
	USLuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Decode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Base64Decode ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name base64Encode
	@param1 self|data @type userdata|string @text The string to encode, or a reference to the current object (self).
	@text If a string is provided, encodes it in base64.  Otherwise, encodes the current data stored in this object as a base64 encoded sequence of characters.
	@return If passed a string, returns either a string or nil depending on whether it could be encoded.  Otherwise the encoding occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_base64Encode ( lua_State* L ) {
	USLuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Encode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Base64Encode ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name deflate
	@param1 self|data @type userdata|string @text The string to deflate, or a reference to the current object (self).
	@param2 level @type number @text The level used in the DEFLATE algorithm.  Pass nil to use the default value.
	@param3 windowBits @type number @text The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@text Compresses the string or the current data stored in this object using the DEFLATE algorithm.
	@return If passed a string, returns either a string or nil depending on whether it could be compressed.  Otherwise the compression occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_deflate ( lua_State* L ) {
	USLuaState state ( L );

	int level = state.GetValue < int >( 2, USDeflater::DEFAULT_LEVEL );
	int windowBits = state.GetValue < int >( 3, USDeflater::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Deflate ( 1, level, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Deflate ( level, windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name getSize
	@param1 self @type userdata
	@text Returns the number of bytes in this data buffer object.
	@return The number of bytes in this data buffer object. @type number
*/
int MOAIDataBuffer::_getSize ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "U" );

	void* bytes;
	u32 size;
	self->Lock ( &bytes, &size );
	
	lua_pushnumber ( state, size );

	self->Unlock ();

	return 1;
}

//----------------------------------------------------------------//
/**	@name getString
	@param1 self @type userdata
	@text Returns the contents of the data buffer object as a string value.
	@return The data buffer object as a string. @type string
*/
int MOAIDataBuffer::_getString ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "U" );

	u32 size;
	void* buffer;
	
	self->Lock ( &buffer, &size );
	lua_pushlstring ( state, ( cc8* )buffer, size );
	self->Unlock ();

	return 1;
}

//----------------------------------------------------------------//
/**	@name inflate
	@param1 self|data @type userdata|string @text The string to inflate, or a reference to the current object (self).
	@param2 windowBits @type number @text The window bits used in the DEFLATE algorithm.  Pass nil to use the default value.
	@text Decompresses the string or the current data stored in this object using the DEFLATE algorithm.
	@return If passed a string, returns either a string or nil depending on whether it could be decompressed.  Otherwise the decompression occurs inline on the existing data buffer in this object, and nil is returned.
*/
int MOAIDataBuffer::_inflate ( lua_State* L ) {
	USLuaState state ( L );

	int windowBits = state.GetValue < int >( 2, USDeflater::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Inflate ( 1, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaObject < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Inflate ( windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name load
	@param1 self @type userdata
	@param2 filename @type string @text The path to the file that the data should be loaded from.
	@text Copies the data from the given file into this object.  This method is a synchronous operation and will block until the file is loaded.
	@return Whether the file could be loaded into the object. @type boolean
*/
int MOAIDataBuffer::_load ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	bool success = self->Load ( filename );
	lua_pushboolean ( state, success );

	return 1;
}

//----------------------------------------------------------------//
/**	@name loadAsync
	@param1 self @type userdata
	@param2 filename @type string @text The path to the file that the data should be loaded from.
	@text Asynchronously copies the data from the given file into this object.  This method is an asynchronous operation and will return immediately; the callback for completion should be set using setCallback.
	@return A new MOAIDataIOAction which indicates the status of the task. @type userdata
*/
int MOAIDataBuffer::_loadAsync ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	MOAIDataIOAction* action = new MOAIDataIOAction ();
	action->Init ( filename, self );
	action->StartLoad ();
	action->PushLuaUserdata( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name save
	@param1 self @type userdata
	@param2 filename @type string @text The path to the file that the data should be saved to.
	@text Saves the data in this object to the given file.  This method is a synchronous operation and will block until the data is saved.
	@return Whether the data could be saved to the file. @type boolean
*/
int MOAIDataBuffer::_save ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );
	bool affirm_path = state.GetValue < bool >( 3, true );

	bool success = self->Save ( filename, affirm_path );
	lua_pushboolean ( state, success );

	return 0;
}

//----------------------------------------------------------------//
/**	@name saveAsync
	@param1 self @type userdata
	@param2 filename @type string @text The path to the file that the data should be saved to.
	@text Asynchronously saves the data in this object to the given file.  This method is an asynchronous operation and will return immediately; the callback for completion should be set using setCallback.
	@return A new MOAIDataIOAction which indicates the status of the task. @type userdata
*/
int MOAIDataBuffer::_saveAsync ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	MOAIDataIOAction* action = new MOAIDataIOAction ();
	action->Init ( filename, self );
	action->StartSave ();
	action->PushLuaUserdata( state );

	return 0;
}

//----------------------------------------------------------------//
/**	@name setString
	@param1 self @type userdata
	@param2 data @type string @text The string data to replace the contents of this object with.
	@text Replaces the contents of this object with the string specified.
	@return nil
*/
int MOAIDataBuffer::_setString ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	size_t len;
	cc8* str = lua_tolstring ( state, 2, &len );
	
	self->Load (( void* )str, len );

	return 0;
}

//================================================================//
// MOAIDataBuffer
//================================================================//

//----------------------------------------------------------------//
MOAIDataBuffer::MOAIDataBuffer () {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIDataBuffer::~MOAIDataBuffer () {
}

//----------------------------------------------------------------//
void MOAIDataBuffer::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "base64Decode",	_base64Decode },
		{ "base64Encode",	_base64Encode },
		{ "deflate",		_deflate },
		{ "inflate",		_inflate },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIDataBuffer::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "base64Decode",	_base64Decode },
		{ "base64Encode",	_base64Encode },
		{ "deflate",		_deflate },
		{ "getSize",		_getSize },
		{ "getString",		_getString },
		{ "inflate",		_inflate },
		{ "load",			_load },
		{ "loadAsync",		_loadAsync },
		{ "save",			_save },
		{ "saveAsync",		_saveAsync },
		{ "setString",		_setString },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIDataBuffer::ToString () {

	STLString repr;

	void* bytes;
	u32 size;
	this->Lock ( &bytes, &size );

	PrettyPrint ( repr, "size", size );

	this->Unlock ();

	return repr;
}
