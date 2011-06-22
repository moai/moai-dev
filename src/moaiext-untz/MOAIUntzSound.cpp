// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-untz/MOAIUntzSound.h>
#include <moaiext-untz/MOAIUntzSound.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_isLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		lua_pushboolean ( state, self->mSound->isLooping ());
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_getPosition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		lua_pushnumber ( state, self->mSound->getPosition ());
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		lua_pushnumber ( state, self->mSound->getVolume ());
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		lua_pushboolean ( state, self->mSound->isPaused ());
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_isPlaying ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		lua_pushboolean ( state, self->mSound->isPlaying ());
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "US" )
	
	cc8* filename = state.GetValue < cc8* >( 2, "" );
	
	if ( self->mSound ) {
		//delete self->mSound;
		self->mSound = 0;
	}
	
	self->mSound = UNTZ::Sound::create ( filename );
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		self->mSound->pause ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		self->mSound->play ();
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_setLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		bool loop = state.GetValue < bool >( 2, false );
		self->mSound->setLooping ( loop );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_setPosition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		double position = state.GetValue < double >( 2, 0.0 );
		self->mSound->setPosition ( position );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		float volume = state.GetValue < float >( 2, 0.0f );
		self->mSound->setVolume ( volume );
	}
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIUntzSound::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSound, "U" )
	
	if ( self->mSound ) {
		self->mSound->stop ();
	}
	return 0;
}

//================================================================//
// MOAIUntzSound
//================================================================//

//----------------------------------------------------------------//
MOAIUntzSound::MOAIUntzSound () {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIUntzSound::~MOAIUntzSound () {

	if ( this->mSound ) {
		//delete this->mSound;
	}
}
//----------------------------------------------------------------//
void MOAIUntzSound::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIUntzSound::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "isLooping",			_isLooping },
		{ "getPosition",		_getPosition },
		{ "getVolume",			_getVolume },
		{ "isPaused",			_isPaused },
		{ "isPlaying",			_isPlaying },
		{ "load",				_load },
		{ "pause",				_pause },
		{ "play",				_play },
		{ "setLooping",			_setLooping },
		{ "setPosition",		_setPosition },
		{ "setVolume",			_setVolume },
		{ "stop",				_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIUntzSound::ToString () {

	STLString repr;
	return repr;
}
