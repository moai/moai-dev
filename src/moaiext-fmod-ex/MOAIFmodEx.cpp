// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod-ex/MOAIFmodEx.h>
#include <fmod.hpp>

#ifdef MOAI_OS_NACL
#include <fmodnacl.h>
#include "moai_nacl.h"
#include "ppapi/c/ppb.h"
#include "ppapi/cpp/instance.h"
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMemoryStats
	@text	Get memory usage.

	@opt	boolean blocking - Default value is 'false.'
	@out	number currentAlloc
	@out	number maxAlloc
*/
int	MOAIFmodEx::_getMemoryStats( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool blocking = state.GetValue < bool >( 1, false );
	
	int currentAlloc;
	int maxAlloc;
	
	FMOD_Memory_GetStats ( &currentAlloc, &maxAlloc, blocking );
	
	lua_pushnumber ( state, currentAlloc );
	lua_pushnumber ( state, maxAlloc );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes the sound system.

	@out	nil
*/
int MOAIFmodEx::_init ( lua_State* L ) {

	MOAILuaState state ( L );
	
#ifdef MOAI_OS_NACL
	printf ( "Cannot initialize fmod on background thread\n" );
	return -1;
#endif

	MOAIFmodEx::Get ().OpenSoundSystem ();

	return 0;
}

int MOAIFmodEx::_mute ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );

	MOAIFmodEx::Get().MuteChannels ( mute );
	return 0;
}

//================================================================//
// MOAIFmodEx
//================================================================//

//----------------------------------------------------------------//
void MOAIFmodEx::CloseSoundSystem () {

	if ( !this->mSoundSys ) return;
	
	this->mSoundSys->close ();
	this->mSoundSys->release ();
	this->mSoundSys = 0;
}

//----------------------------------------------------------------//
MOAIFmodEx::MOAIFmodEx () :
	mSoundSys ( 0 ) {
}

//----------------------------------------------------------------//
MOAIFmodEx::~MOAIFmodEx () {

	this->CloseSoundSystem ();
}

//----------------------------------------------------------------//
void MOAIFmodEx::MuteChannels ( bool mute ) {

	this->mMainChannelGroup->setMute ( mute );
}

//----------------------------------------------------------------//
void MOAIFmodEx::OpenSoundSystem () {

	FMOD_RESULT result;

	//FMOD::Debug_SetLevel(FMOD_DEBUG_ALL);
	//FMOD::Debug_SetLevel(FMOD_DEBUG_LEVEL_ERROR);

	result = FMOD::System_Create ( &this->mSoundSys ); // Create the main system object.
	if ( result != FMOD_OK ) return;

#ifdef MOAI_OS_NACL

	FMOD_NACL_EXTRADRIVERDATA extraDriverData;
	memset(&extraDriverData, 0, sizeof(FMOD_NACL_EXTRADRIVERDATA)); 
	extraDriverData.instance = g_instance->pp_instance();

	this->mSoundSys->setDSPBufferSize( 1024, 4 );
	result = this->mSoundSys->init ( 100, FMOD_INIT_NORMAL, &extraDriverData );
#else
	result = this->mSoundSys->init ( 100, FMOD_INIT_NORMAL, 0 );
#endif

	if ( result != FMOD_OK ) return;
	
	result = this->mSoundSys->getMasterChannelGroup ( &this->mMainChannelGroup );
	if ( result != FMOD_OK ) return;
}

//----------------------------------------------------------------//
void MOAIFmodEx::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getMemoryStats",		_getMemoryStats },
		{ "init",				_init },
		{ "mute",				_mute },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodEx::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodEx::Update () {

	if ( this->mSoundSys ) {
		this->mSoundSys->update ();
	}
}

