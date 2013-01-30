// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moaiext-fmod-ex/MOAIFmodEx.h>
#include <moaiext-fmod-ex/MOAIFmodExChannel.h>
#include <moaiext-fmod-ex/MOAIFmodExSound.h>
#include <fmod.hpp>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVolume
 @text	Returns the current volume of the channel.
 
 @in	MOAIFmodExChannel self
 @out	float Volume - the volume currently set in this channel.
 */
int MOAIFmodExChannel::_getVolume ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "U" )
	
	lua_pushnumber ( state, self->GetVolume ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
 @text	Returns true if channel is playing.
 
 @in	MOAIFmodExChannel self
 @out	boolean.
 */
int MOAIFmodExChannel::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "U" )
	
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

	@in		MOAIFmodExChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodExChannel::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodExChannelAttr::Pack ( ATTR_VOLUME ), delta, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Plays the specified sound, looping it if desired.

	@in		MOAIFmodExChannel self
	@in		MOAIFmodExSound sound		The sound to play.
	@in		boolean loop			Whether the sound should be looped.
	@out	nil
*/
int MOAIFmodExChannel::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "UU" )

	self->mPlayState = PLAYING;
	MOAIFmodExSound* sound = state.GetLuaObject < MOAIFmodExSound >( 2, true );
	if ( !sound ) return 0;

	int loopCount = state.GetValue < int >( 3, 0 );

	self->Play ( sound, loopCount );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodExChannel self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
*/
int MOAIFmodExChannel::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "UNN" )

	MOAIEaseDriver* action = new MOAIEaseDriver ();
	action->ReserveLinks ( 1 );
	
	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	u32 mode		= state.GetValue < u32 >( 4, USInterpolate::kSmooth );
	
	action->SetLink ( 0, self, MOAIFmodExChannelAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );

	action->SetSpan ( length );
	action->Start ();
	action->PushLuaUserdata ( state );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this channel.

	@in		MOAIFmodExChannel self
	@in		number volume			The volume of this channel.
	@out	nil
*/
int MOAIFmodExChannel::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "UN" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPaused
	@text	Sets whether this channel is paused and hence does not play any sounds.

	@in		MOAIFmodExChannel self
	@in		boolean paused			Whether this channel is paused.
	@out	nil
*/
int MOAIFmodExChannel::_setPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "UB" )

	self->mPlayState = PAUSED;
	bool paused = state.GetValue < bool >( 2, false );
	self->SetPaused ( paused );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLooping
	@text	Immediately sets looping for this channel.

	@in		MOAIFmodExChannel self
	@in		boolean looping	  True if channel should loop.
	@out	nil
*/
int MOAIFmodExChannel::_setLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "U" )

	float looping = state.GetValue < bool >( 2, false );
	self->mLooping = looping;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this channel.

	@in		MOAIFmodExChannel self
	@out	nil
*/
int MOAIFmodExChannel::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodExChannel, "U" )
	
	self->mPlayState = STOPPED;
	self->Stop ();

	return 0;
}

//================================================================//
// MOAIFmodExChannel
//================================================================//

//----------------------------------------------------------------//
bool MOAIFmodExChannel::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIFmodExChannelAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_VOLUME ) {
			this->mVolume = attrOp.Apply ( this->mVolume, op, MOAIAttrOp::ATTR_READ_WRITE );
			this->SetVolume ( this->mVolume );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
float MOAIFmodExChannel::GetVolume () {
	return this->mVolume;
}

//----------------------------------------------------------------//
MOAIFmodExChannel::MOAIFmodExChannel () :
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) ,
	mLooping ( false ) {
	
	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodExChannel::~MOAIFmodExChannel () {

	this->Stop ();
}

//----------------------------------------------------------------//
void MOAIFmodExChannel::Play ( MOAIFmodExSound* sound, int loopCount ) {

	this->Stop ();
	this->mSound = sound;
	if ( !sound ) return;
	if ( !sound->mSound ) return;
	
	FMOD::System* soundSys = MOAIFmodEx::Get ().GetSoundSys ();
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
		this->mChannel->setLoopCount ( loopCount );
	}
	
	this->SetVolume ( this->mVolume );
	this->SetPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodExChannel::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodExChannelAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodExChannel::RegisterLuaFuncs ( MOAILuaState& state ) {

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
void MOAIFmodExChannel::SetPaused ( bool paused ) {

	this->mPaused = paused;
	if ( !this->mChannel ) return;
	this->mChannel->setPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodExChannel::SetVolume ( float volume ) {

	this->mVolume = volume;
	if ( !this->mChannel ) return;
	this->mChannel->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
void MOAIFmodExChannel::Stop () {

	if ( !this->mChannel ) return;
	this->mChannel->stop();
}
