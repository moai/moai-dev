// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAIData.h>

//================================================================//
// MOAIData
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>getSize ()</tt>\n
\n
	Returns the number of bytes in this object.
*/
int MOAIData::_getSize ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;

	MOAIData* self = state.GetLuaData < MOAIData >( 1 );
	if ( !self ) return 0;

	void* bytes;
	u32 size;
	self->Lock ( &bytes, &size );
	
	lua_pushnumber ( state, size );

	self->Unlock ();

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>load (filename)</tt>\n
\n
	Copies the data from the given file into this object.  Method call blocks until operation is complete.
	@param filename source to read from
	@return boolean value indicating success or failure
*/
int MOAIData::_load ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "US" )) return 0;

	MOAIData* self = state.GetLuaData < MOAIData >( 1 );
	if ( !self ) return 0;

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
int MOAIData::_loadAsync ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "US" )) return 0;

	MOAIData* self = state.GetLuaData < MOAIData >( 1 );
	if ( !self ) return 0;

	cc8* filename = lua_tostring ( state, 2 );

	MOAIDataIOAction* action = new MOAIDataIOAction ();
	action->Init ( filename, self );
	action->Start ();
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
int MOAIData::_save ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "US" )) return 0;

	MOAIData* self = state.GetLuaData < MOAIData >( 1 );
	if ( !self ) return 0;

	cc8* filename = lua_tostring ( state, 2 );
	bool affirm_path = state.GetValue < bool >( 3, true );

	bool success = self->Save ( filename, affirm_path );
	lua_pushboolean ( state, success );

	return 1;
}

//================================================================//
// MOAIData
//================================================================//

//----------------------------------------------------------------//
MOAIData::MOAIData () {
	
	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAIData::~MOAIData () {
}

//----------------------------------------------------------------//
void MOAIData::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIData::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "getSize",		_getSize },
		{ "load",			_load },
		{ "loadAsync",		_loadAsync },
		{ "save",			_save },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIData::ToString () {

	STLString repr;

	void* bytes;
	u32 size;
	this->Lock ( &bytes, &size );

	PrettyPrint ( repr, "size", size );

	this->Unlock ();

	return repr;
}
