// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING
#if USE_FMOD

#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIFmodSound.h>
#include <moaicore/MOAIFmodChannel.h>
#include <moaicore/MOAIFmod.h>

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
int MOAIFmodChannel::_moveVolume ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "UNN" )

	MOAIEaseDriver* action = USSafeNew < MOAIEaseDriver >();
	action->Init ( self, 1, 0.0f );
	
	action->SetForce ( 0, MOAIFmodChannel::ATTR_VOLUME, state.GetValue < float >( 2, 0.0f ));
	
	action->SetDelay (state.GetValue < float >( 3, 0.0f ));
	action->SetMode ( state.GetValue < u32 >( 4, USInterpolate::kSmooth ));
	
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodChannel::_play ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "UU" )

	MOAIFmodSound* sound = state.GetLuaObject < MOAIFmodSound >( 2 );
	if ( !sound ) return 0;

	int loopCount = state.GetValue < int >( 3, 0 );

	self->Play ( sound, loopCount );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodChannel::_seekVolume ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "UNN" )

	MOAIEaseDriver* action = USSafeNew < MOAIEaseDriver >();
	action->Init ( self, 1, 0.0f );
	
	action->SetForce ( 0, MOAIFmodChannel::ATTR_VOLUME, state.GetValue < float >( 2, 0.0f ) - self->mVolume );
	
	action->SetDelay ( state.GetValue < float >( 3, 0.0f ));
	action->SetMode ( state.GetValue < u32 >( 4, USInterpolate::kSmooth ));
	
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodChannel::_setPaused ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "UB" )

	bool paused = state.GetValue < bool >( 2, false );
	self->SetPaused ( paused );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodChannel::_setVolume ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "UN" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );

	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
*/
int MOAIFmodChannel::_stop ( lua_State* L ) {
	LUA_SETUP ( MOAIFmodChannel, "U" )
	
	self->Stop ();

	return 0;
}

//================================================================//
// MOAIFmodChannel
//================================================================//

//----------------------------------------------------------------//
void MOAIFmodChannel::ApplyAttrOp ( u32 attrID, USAttrOp& attrOp ) {

	switch ( attrID ) {
		case ATTR_VOLUME:
			this->mVolume = attrOp.Op ( this->mVolume );
			this->SetVolume ( this->mVolume );
			return;
	}
}

//----------------------------------------------------------------//
MOAIFmodChannel::MOAIFmodChannel () :
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodChannel::~MOAIFmodChannel () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIFmodChannel::Play ( MOAIFmodSound* sound, int loopCount ) {

	this->Stop ();
	this->mSound = sound;
	if ( !sound ) return;
	if ( !sound->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmod::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	FMOD_RESULT result;
	FMOD::Channel* channel = 0;
	
	result = soundSys->playSound ( FMOD_CHANNEL_FREE, sound->mSound, true, &channel );
	if ( result != FMOD_OK ) return;
	
	this->mChannel = channel;
	this->mChannel->setMode ( FMOD_LOOP_NORMAL );
	this->mChannel->setLoopCount ( loopCount );
	
	this->SetVolume ( this->mVolume );
	this->SetPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodChannel::RegisterLuaClass ( USLuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIFmodChannel::RegisterLuaFuncs ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "moveVolume",		_moveVolume },
		{ "play",			_play },
		{ "seekVolume",		_seekVolume },
		{ "setPaused",		_setPaused },
		{ "setVolume",		_setVolume },
		{ "stop",			_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodChannel::SetPaused ( bool paused ) {

	this->mPaused = paused;
	if ( !this->mChannel ) return;
	this->mChannel->setPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodChannel::SetVolume ( float volume ) {

	this->mVolume = volume;
	if ( !this->mChannel ) return;
	this->mChannel->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
void MOAIFmodChannel::Stop () {

	if ( !this->mChannel ) return;
	this->mChannel->stop();
}

//----------------------------------------------------------------//
STLString MOAIFmodChannel::ToString () {

	STLString repr;

	PRETTY_PRINT ( repr, mVolume )
	PRETTY_PRINT ( repr, mPaused )
	PRETTY_PRINT ( repr, mChannel )
	PRETTY_PRINT ( repr, mSound )

	return repr;
}

#endif