// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod/MOAIFmod.h>
#include <moaiext-fmod/MOAIFmodChannel.h>
#include <moaiext-fmod/MOAIFmodSound.h>
#include <fmod.hpp>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVolume
 @text	Returns the current volume of the channel.
 
 @in	MOAIFmodChannel self
 @out	float Volume - the volume currently set in this channel.
 */
int MOAIFmodChannel::_getVolume ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodChannel, "U" )
	
	lua_pushnumber ( state, self->GetVolume ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getVolume
 @text	Returns the current volume of the channel.
 
 @in	MOAIFmodChannel self
 @out	float Volume - the volume currently set in this channel.
 */
int MOAIFmodChannel::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodChannel, "U" )
	
	bool isPlaying = self->mPlayState == PLAYING;

	if ( self->mSound ) {
		lua_pushboolean ( state, isPlaying );
		return 1; 
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodChannel::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodChannelAttr::Pack ( ATTR_VOLUME ), delta, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Plays the specified sound, looping it if desired.

	@in		MOAIFmodChannel self
	@in		MOAIFmodSound sound		The sound to play.
	@in		boolean loop			Whether the sound should be looped.
	@out	nil
*/
int MOAIFmodChannel::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UU" )

	self->mPlayState = PLAYING;
	MOAIFmodSound* sound = state.GetLuaObject < MOAIFmodSound >( 2 );
	if ( !sound ) return 0;

	int loopCount = state.GetValue < int >( 3, 0 );

	self->Play ( sound, loopCount );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodChannel::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodChannelAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setLooping
	@text	Immediately sets the volume of this channel.

	@in		MOAIFmodChannel self
	@in		number volume			The volume of this channel.
	@out	nil
*/
int MOAIFmodChannel::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UN" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPaused
	@text	Sets whether this channel is paused and hence does not play any sounds.

	@in		MOAIFmodChannel self
	@in		boolean paused			Whether this channel is paused.
	@out	nil
*/
int MOAIFmodChannel::_setPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UB" )

	self->mPlayState = PAUSED;
	bool paused = state.GetValue < bool >( 2, false );
	self->SetPaused ( paused );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this channel.

	@in		MOAIFmodChannel self
	@in		number volume			The volume of this channel.
	@out	nil
*/
int MOAIFmodChannel::_setLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "UB" )

	float looping = state.GetValue < bool >( 2, false );

	self->mLooping = looping;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this channel.

	@in		MOAIFmodChannel self
	@out	nil
*/
int MOAIFmodChannel::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodChannel, "U" )
	
	self->mPlayState = STOPPED;
	self->Stop ();

	return 0;
}

//================================================================//
// MOAIFmodChannel
//================================================================//

//----------------------------------------------------------------//
bool MOAIFmodChannel::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIFmodChannelAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_VOLUME ) {
			this->mVolume = attrOp.Apply ( this->mVolume, op, MOAINode::ATTR_READ_WRITE );
			this->SetVolume ( this->mVolume );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
float MOAIFmodChannel::GetVolume () {
	return this->mVolume;
}

//----------------------------------------------------------------//
MOAIFmodChannel::MOAIFmodChannel () :
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) ,
	mLooping ( false ) {
	
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
	
	//printf ( "PLAY SOUND %s, @ %f\n", sound->GetFileName (), USDeviceTime::GetTimeInSeconds () );
	result = soundSys->playSound ( FMOD_CHANNEL_FREE, sound->mSound, true, &channel );
	if ( result != FMOD_OK ) {
		printf (" FMOD ERROR: Sound did not play\n" );
		return;
	}
	
	this->mChannel = channel;
	this->mChannel->setMode ( FMOD_LOOP_NORMAL );

	if ( mLooping ) {
		this->mChannel->setLoopCount ( -1 );
	}
	else {
		this->mChannel->setLoopCount ( 0 );
	}
	
	this->SetVolume ( this->mVolume );
	this->SetPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodChannel::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodChannelAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodChannel::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getVolume",		_getVolume },
		{ "isPlaying",		_isPlaying },
		{ "moveVolume",		_moveVolume },
		{ "play",			_play },
		{ "seekVolume",		_seekVolume },
		{ "setPaused",		_setPaused },
		{ "setLooping",		_setLooping },
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
