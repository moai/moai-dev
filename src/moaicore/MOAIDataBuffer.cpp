// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAIDataBuffer.h>

//================================================================//
// MOAIDataBuffer
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBuffer::_base64Decode ( lua_State* L ) {
	USLuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Decode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaData < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Base64Decode ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBuffer::_base64Encode ( lua_State* L ) {
	USLuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Base64Encode ( 1 ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaData < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Base64Encode ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIDataBuffer::_deflate ( lua_State* L ) {
	USLuaState state ( L );

	int level = state.GetValue < int >( 2, USDeflater::DEFAULT_LEVEL );
	int windowBits = state.GetValue < int >( 3, USDeflater::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Deflate ( 1, level, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaData < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Deflate ( level, windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>getSize ()</tt>\n
\n
	Returns the number of bytes in this object.
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
// TODO: doxygen
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
// TODO: doxygen
int MOAIDataBuffer::_inflate ( lua_State* L ) {
	USLuaState state ( L );

	int windowBits = state.GetValue < int >( 2, USDeflater::DEFAULT_WBITS );

	if ( state.IsType ( 1, LUA_TSTRING )) {
		return state.Inflate ( 1, windowBits ) ? 1 : 0;
	}
	
	MOAIDataBuffer* self = state.GetLuaData < MOAIDataBuffer >( 1 );
	if ( self ) {
		self->Inflate ( windowBits );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>load (filename)</tt>\n
\n
	Copies the data from the given file into this object.  Method call blocks until operation is complete.
	@param filename source to read from
	@return boolean value indicating success or failure
*/
int MOAIDataBuffer::_load ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	bool success = self->Load ( filename );
	lua_pushboolean ( state, success );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>loadAsync (filename)</tt>\n
\n
	Asynchronously copies the data from the given file into this object.  Method call returns immediately.
	@see setCallback
	@param filename source to read from
	@return an action that determines state of the task.
*/
int MOAIDataBuffer::_loadAsync ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	MOAIDataIOAction* action = new MOAIDataIOAction ();
	action->Init ( filename, self );
	action->StartLoad ();
	action->PushLuaInstance( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>save (filename)</tt>\n
\n
	Saves this object's data into the given file.  Method call blocks until operation is complete.
	@param filename destination to write to
	@return boolean value indicating success or failure
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
// TODO: doxygen
int MOAIDataBuffer::_saveAsync ( lua_State* L ) {
	LUA_SETUP ( MOAIDataBuffer, "US" );

	cc8* filename = lua_tostring ( state, 2 );

	MOAIDataIOAction* action = new MOAIDataIOAction ();
	action->Init ( filename, self );
	action->StartSave ();
	action->PushLuaInstance( state );

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
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
	
	RTTI_SINGLE ( USLuaData )
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
