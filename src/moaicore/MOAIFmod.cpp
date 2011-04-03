// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if USE_FMOD

#include <moaicore/MOAIFmod.h>

#include <fmod.hpp>
#include <fmod_errors.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static int _deprecate ( lua_State* L ) {

	return 0;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes the sound system.

	@out	nil
*/
int MOAIFmod::_init ( lua_State* L ) {

	USLuaState state ( L );
	
	MOAIFmod::Get ().OpenSoundSystem ();

	return 0;
}

//================================================================//
// MOAIFmod
//================================================================//

//----------------------------------------------------------------//
void MOAIFmod::CloseSoundSystem () {

	if ( !this->mSoundSys ) return;
	
	this->mSoundSys->release ();
	this->mSoundSys = 0;
}

//----------------------------------------------------------------//
MOAIFmod::MOAIFmod () :
	mSoundSys ( 0 ) {
}

//----------------------------------------------------------------//
MOAIFmod::~MOAIFmod () {

	this->CloseSoundSystem ();
}

//----------------------------------------------------------------//
void MOAIFmod::OpenSoundSystem () {

	FMOD_RESULT result;

	result = FMOD::System_Create ( &this->mSoundSys ); // Create the main system object.
	if ( result != FMOD_OK ) return;
	
	result = this->mSoundSys->init ( 100, FMOD_INIT_NORMAL, 0 ); // Initialize FMOD.
	if ( result != FMOD_OK ) return;
}

//----------------------------------------------------------------//
void MOAIFmod::RegisterLuaClass ( USLuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIFmod::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "init",			_init },
		
		{ "loadBGM",		_deprecate },
		{ "loadSFX",		_deprecate },
		{ "openChannel",	_deprecate },
		{ "playSound",		_deprecate },
		{ "releaseSound",	_deprecate },
		{ "selectChannel",	_deprecate },
		{ "selectSound",	_deprecate },
		{ "stopSound",		_deprecate },
		
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmod::Update () {

	if ( this->mSoundSys ) {
		this->mSoundSys->update ();
	}
}

//----------------------------------------------------------------//
STLString MOAIFmod::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mSoundSys )

	return repr;
}

#endif
