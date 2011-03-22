// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILuaScript.h>
#include <moaicore/MOAIData.h>

//================================================================//
// local
//================================================================//

static const char *TAG = "MOAILuaScript";

//----------------------------------------------------------------//
/**	@brief <tt> execute()</tt>\n
\n
	Execute the loaded script in the context of the calling Lua state/thread.
*/
int MOAILuaScript::_execute ( lua_State* L ) {

	USLuaState state ( L );

	if ( !state.CheckParams ( 1, "U" ) ) return 0;

	MOAILuaScript* self = state.GetLuaData < MOAILuaScript > ( 1 );
	if ( !self ) return 0;

	if ( self->mData == NULL ) return 0;

	lua_pushboolean ( state, self->Execute ( state ) );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt> load( filename|data )</tt>\n
\n
	Loads external script data into this object.
*/
int MOAILuaScript::_load ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" ) ) return 0;

	MOAILuaScript* self = state.GetLuaData < MOAILuaScript > ( 1 );
	if ( !self ) return 0;

	if ( state.CheckParams ( 2, "S" ) ) {

		const char *filename = lua_tostring ( state, 2 );

		lua_pushboolean ( state, self->Load ( filename ) );
	}
	else {

		USData* data = state.GetLuaData < MOAIData > ( 2 );

		if ( data != NULL ) {

			lua_pushboolean ( state, self->Load( *data ) );
		}
		else {

			lua_pushboolean ( state, false );
		}
	}

	return 1;
}

//================================================================//
// MOAILuaScript
//================================================================//

//----------------------------------------------------------------//
MOAILuaScript::MOAILuaScript ()
:	mData ( NULL ),
	mDataLength ( 0 ) {

	RTTI_SINGLE ( USLuaData )
}

//----------------------------------------------------------------//
MOAILuaScript::~MOAILuaScript () {

	Clear ();
}

//----------------------------------------------------------------//
void MOAILuaScript::Clear () {

	if ( mData ) {

		delete [] mData;
		mData = NULL;
	}

	mDataLength = 0;
}

//----------------------------------------------------------------//
bool MOAILuaScript::Execute ( USLuaState& state ) {

	int result = luaL_loadbuffer ( state, mData, mDataLength, TAG );

	if ( result == 0 ) {

		 // TODO Determine if we should allow parameters, return values
		result = state.DebugCall ( 0, 0 );
	}

	return result == 0;
}

//----------------------------------------------------------------//
bool MOAILuaScript::Load ( const char *filename ) {

	USData data;

	if ( data.Load ( filename ) ) {

		return Load ( data );
	}

	return false;
}

//----------------------------------------------------------------//
bool MOAILuaScript::Load ( USData& data ) {

	Clear ();
	
	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );
	
	this->mDataLength = size;

	char *dest = new char [ mDataLength ];
	this->mData = dest;

	u8* src = ( u8* )bytes;

	// FIXME Replace with copy method
	for ( int i = 0; i < mDataLength; ++i) {
		*dest++ = *src++;
	}

	data.Unlock ();

	return true;
}

//----------------------------------------------------------------//
void MOAILuaScript::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAILuaScript::RegisterLuaFuncs ( USLuaState& state ) {

		LuaReg regTable [] = {
		{ "execute",		_execute },
		{ "load",			_load },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString  MOAILuaScript::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mData );
	PRETTY_PRINT ( repr, mDataLength );

	return repr;
}
