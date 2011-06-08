// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-untz/MOAIUntzSound.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
//int MOAIUntzSound::_load ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
//
//	bool streaming	= state.GetValue < bool >( 3, false );
//	bool async		= state.GetValue < bool >( 4, false );
//
//	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );
//
//	if ( data ) {
//
//		self->Load( *data, streaming );
//	}
//	else if ( state.IsType( 2, LUA_TSTRING ) ) {
//
//		cc8* filename	= state.GetValue < cc8* >( 2, "" );
//		self->Load ( filename, streaming, async );
//	}
//
//	return 0;
//}

//================================================================//
// MOAIUntzSound
//================================================================//

//----------------------------------------------------------------//
MOAIUntzSound::MOAIUntzSound () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIUntzSound::~MOAIUntzSound () {
}
//----------------------------------------------------------------//
void MOAIUntzSound::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIUntzSound::RegisterLuaFuncs ( USLuaState& state ) {

	//luaL_Reg regTable [] = {
	//	{ "load",			_load },
	//	{ "loadBGM",		_loadBGM },
	//	{ "loadSFX",		_loadSFX },
	//	{ "release",		_release },
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIUntzSound::ToString () {

	STLString repr;
	return repr;
}
