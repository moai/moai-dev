// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod-ex/MOAIFmodEx.h>
#include <moaiext-fmod-ex/MOAIFmodExSound.h>
#include <fmod.hpp>

#ifdef MOAI_OS_IPHONE
	#include <fmodiphone.h>
#endif

#ifdef MOAI_OS_NACL
#include <NaClFileSystem.h>
#include <moai_nacl.h>
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads the specified sound from file, or from a MOAIDataBuffer.

	@overload
	
		@in		MOAIFmodExSound self
		@in		string filename			The path to the sound to load from file.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@in		boolean	async			Whether the sound file should be loaded asynchronously.
		@out	nil

	@overload

		@in		MOAIFmodExSound self
		@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@out	nil
*/
int MOAIFmodExSound::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExSound, "U" )

	bool streaming	= state.GetValue < bool >( 3, false );
	bool async		= state.GetValue < bool >( 4, false );

	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, streaming, async );
	}
	else {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, streaming );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadBGM
	@text	Loads the specified BGM sound from file, or from a MOAIDataBuffer.

	@in		MOAIFmodExSound self
	@opt	string filename			The path to the sound to load from file.
	@opt	MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
	@out	nil
*/
int	MOAIFmodExSound::_loadBGM ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExSound, "U" )

	
	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, true, false );
	}
	else {
	
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, true );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadSFX
	@text	Loads the specified SFX sound from file, or from a MOAIDataBuffer.

	@in		MOAIFmodExSound self
	@opt	string filename			The path to the sound to load from file.
	@opt	MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
	@out	nil
*/
int	MOAIFmodExSound::_loadSFX ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExSound, "U" )

	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, false, true );
	}
	else {
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, false );
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases the sound data from memory.

	@in		MOAIFmodExSound self
	@out	nil
*/
int MOAIFmodExSound::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExSound, "U" )

	self->Release ();

	return 0;
}

//================================================================//
// MOAIFmodExSound
//================================================================//

//----------------------------------------------------------------//
MOAIFmodExSound::MOAIFmodExSound () :
	mSound ( 0 ),
	mLoopCount ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )

	memset ( mFileName, 0, 128 );
}

//----------------------------------------------------------------//
MOAIFmodExSound::~MOAIFmodExSound () {

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIFmodExSound::Load ( MOAIDataBuffer& data, bool streaming ) {

	if ( this->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmodEx::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	void* bytes;
	size_t size;
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
void MOAIFmodExSound::Load ( cc8* filename, bool streaming, bool async ) {

	async = false;
	if ( this->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmodEx::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	FMOD_MODE mode = 0;
	mode = streaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
	mode |= async ? FMOD_NONBLOCKING : 0;
	
	FMOD_RESULT result;
	FMOD::Sound* sound = 0;
	FMOD_CREATESOUNDEXINFO info;
	memset ( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ));
	
	info.cbsize = sizeof ( FMOD_CREATESOUNDEXINFO );
	
	#ifdef MOAI_OS_IPHONE
		info.audioqueuepolicy = FMOD_AUDIOQUEUE_CODECPOLICY_SOFTWAREONLY;
	#endif

#ifdef MOAI_OS_NACL

	mode = FMOD_OPENMEMORY | FMOD_CREATESAMPLE;

	memset( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	info.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );

	NaClFile *file = g_FileSystem->fopen ( filename, "r" );

	if ( file ) {
		
		info.length = file->mSize;

		result = soundSys->createSound (( cc8* )file->mData, FMOD_SOFTWARE | mode, &info, &sound );

		g_FileSystem->fclose ( file );
	}
	else {
		info.length = 0;
		result = soundSys->createSound (( cc8* )NULL, FMOD_SOFTWARE | mode, &info, &sound );
	}
#else
	if ( streaming ) {
		result = soundSys->createStream ( filename, FMOD_SOFTWARE | mode, &info, &sound );
	}
	else {
		result = soundSys->createSound ( filename, FMOD_SOFTWARE | mode, &info, &sound );
	}
#endif
	
	if ( result != FMOD_OK ) {
		return;
	}

	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodExSound::Release () {

	if ( !this->mSound ) return;
	
	if ( MOAIFmodEx::IsValid ()) {
		this->mSound->release ();
	}
	this->mSound = 0;
}

//----------------------------------------------------------------//
void MOAIFmodExSound::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodExSound::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "load",			_load },
		{ "loadBGM",		_loadBGM },
		{ "loadSFX",		_loadSFX },
		{ "release",		_release },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

