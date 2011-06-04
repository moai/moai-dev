// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod/MOAIFmod.h>
#include <moaiext-fmod/MOAIFmodSound.h>
#include <fmod.hpp>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads the specified sound from file, or from a MOAIDataBuffer.

	@overload
	
		@in		MOAIFmodSound self
		@in		string filename			The path to the sound to load from file.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@in		boolean	async			Whether the sound file should be loaded asynchronously.
		@out	nil

	@overload

		@in		MOAIFmodSound self
		@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@out	nil
*/
int MOAIFmodSound::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

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
/**	@name	loadBGM
	@text	Loads the specified BGM sound from file, or from a MOAIDataBuffer.

	@in		MOAIFmodSound self
	@opt	string filename			The path to the sound to load from file.
	@opt	MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
	@out	nil
*/
int	MOAIFmodSound::_loadBGM ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

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
/**	@name	loadSFX
	@text	Loads the specified SFX sound from file, or from a MOAIDataBuffer.

	@in		MOAIFmodSound self
	@opt	string filename			The path to the sound to load from file.
	@opt	MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
	@out	nil
*/
int	MOAIFmodSound::_loadSFX ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

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
/**	@name	release
	@text	Releases the sound data from memory.

	@in		MOAIFmodSound self
	@out	nil
*/
int MOAIFmodSound::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

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
	
	FMOD_MODE mode = 0;
	mode = streaming ? FMOD_CREATESTREAM : FMOD_DEFAULT;
	mode |= async ? FMOD_NONBLOCKING : 0;
	
	FMOD_RESULT result;
	FMOD::Sound* sound = 0;

	result = soundSys->createSound ( filename, mode, 0, &sound );
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
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodSound::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
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
