// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod/MOAIFmod.h>
#include <fmod.hpp>

//================================================================//
// local
//================================================================//

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

int	MOAIFmod::_getMemoryStats( lua_State* L ) {
	//MOAI_LUA_SETUP ( MOAIFmod, "U" )
	USLuaState state ( L );
	
	bool blocking	= state.GetValue < bool >( 2, false );
	int currentAlloc, maxAlloc;

	
	FMOD_Memory_GetStats(&currentAlloc, &maxAlloc, blocking);
	lua_pushnumber ( state, currentAlloc );
	lua_pushnumber ( state, maxAlloc );
	
	return 2;
}
//================================================================//
// MOAIFmod
//================================================================//

//----------------------------------------------------------------//
void MOAIFmod::CloseSoundSystem () {

	if ( !this->mSoundSys ) return;
	
	this->mSoundSys->close();
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
void MOAIFmod::MuteChannels ( bool mute ){
	

	this->mMainChannelGroup->setMute( mute );

}
//----------------------------------------------------------------//
void MOAIFmod::OpenSoundSystem () {

	FMOD_RESULT result;

	result = FMOD::System_Create ( &this->mSoundSys ); // Create the main system object.
	if ( result != FMOD_OK ) return;
	
	result = this->mSoundSys->init ( 100, FMOD_INIT_NORMAL, 0 ); // Initialize FMOD.
	if ( result != FMOD_OK ) return;
	
	result = this->mSoundSys->getMasterChannelGroup( &this->mMainChannelGroup);
	if ( result != FMOD_OK ) return;
}

//----------------------------------------------------------------//
void MOAIFmod::RegisterLuaClass ( USLuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "init",			_init },
		{ "getMemoryStats", _getMemoryStats },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmod::RegisterLuaFuncs ( USLuaState& state ) {

	UNUSED ( state );
	
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
