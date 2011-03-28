// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if USE_FMOD

#include <moaicore/MOAIFmodSound.h>
#include <moaicore/MOAIFmod.h>
#include <moaicore/MOAIDataBuffer.h>

#include <fmod.hpp>
#include <fmod_errors.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodSound::_load ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodSound, "U" )

	bool streaming	= state.GetValue < bool >( 3, false );
	bool async		= state.GetValue < bool >( 4, false );

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );

	if ( data ) {

		self->Load( *data, streaming );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		self->Load ( filename, streaming, async );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIFmodSound::_loadBGM ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodSound, "U" )

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );

	if ( data ) {

		self->Load( *data, true );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		self->Load ( filename, true, false );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int	MOAIFmodSound::_loadSFX ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodSound, "U" )

	MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2 );

	if ( data ) {

		self->Load( *data, false );
	}
	else if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		self->Load ( filename, false, true );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodSound::_release ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodSound, "U" )

	self->Release ();

	return 0;
}

//================================================================//
// MOAIFmodSound
//================================================================//

//----------------------------------------------------------------//
MOAIFmodSound::MOAIFmodSound () :
	mSound ( 0 ),
	mLoopCount ( 0 ) {

	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIFmodSound::~MOAIFmodSound () {

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIFmodSound::Load ( MOAIDataBuffer& data, bool streaming ) {

	if ( this->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmod::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	void* bytes;
	u32 size;
	data.Lock ( &bytes, &size );

	FMOD_MODE mode = FMOD_OPENMEMORY;
	mode |= streaming ? FMOD_CREATESTREAM : FMOD_DEFAULT;

	FMOD_CREATESOUNDEXINFO info;
	memset( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	info.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
	info.length = size;

	FMOD_RESULT result;
	FMOD::Sound* sound = 0;

	result = soundSys->createSound (( cc8* )bytes, mode, &info, &sound );
	
	data.Unlock ();
	
	if ( result != FMOD_OK ) return;
	
	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Load ( cc8* filename, bool streaming, bool async ) {
	
	if ( this->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmod::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	STLString fullpath = USResourceMgr::Get ().GetPath ( filename );
	if ( !fullpath.size ()) return;
	
	FMOD_MODE mode = 0;
	mode = streaming ? FMOD_CREATESTREAM : FMOD_DEFAULT;
	mode |= async ? FMOD_NONBLOCKING : 0;
	
	FMOD_RESULT result;
	FMOD::Sound* sound = 0;

	result = soundSys->createSound ( fullpath.c_str (), mode, 0, &sound );
	if ( result != FMOD_OK ) return;
	
	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Release () {

	if ( !this->mSound ) return;
	
	if ( MOAIFmod::IsValid ()) {
		this->mSound->release ();
	}
	this->mSound = 0;
}

//----------------------------------------------------------------//
void MOAIFmodSound::RegisterLuaClass ( USLuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIFmodSound::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "load",			_load },
		{ "loadBGM",		_loadBGM },
		{ "loadSFX",		_loadSFX },
		{ "release",		_release },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAIFmodSound::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mSound )
	PRETTY_PRINT ( repr, mLoopCount )

	return repr;
}

#endif