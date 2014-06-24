// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-untz/MOAIUntzSoundQueue.h>
#include <SoundData.h>

//================================================================//
// local
//================================================================//


//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Return the volume of the sound.

	@in		MOAIUntzSound self
	@out	number volume
*/
int MOAIUntzSoundQueue::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	lua_pushnumber ( state, self->GetVolume ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPaused
	@text	Return the pause status of the sound.

	@in		MOAIUntzSoundQueue self
	@out	boolean paused
*/
int MOAIUntzSoundQueue::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	if ( self->mSound ) {
		lua_pushboolean ( state, self->mSound->isPaused ());
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
	@text	Return the playing status of the sound.

	@in		MOAIUntzSoundQueue self
	@out	boolean playing
*/
int MOAIUntzSoundQueue::_isPlaying ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	if ( self->mSound ) {
		lua_pushboolean ( state, self->mSound->isPlaying ());
		return 1;
	}
	return 0;
}


//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Animation helper for volume attribute,

	@in		MOAITransform self
	@in		number vDelta		Delta to be added to v.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAIUntzSoundQueue::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "UNN" )

	float volume	= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );

	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );

		action->SetLink ( 0, self, MOAIUntzSoundQueueAttr::Pack ( ATTR_VOLUME ), volume, mode );

		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}

	self->SetVolume ( self->GetVolume () + volume );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	addSound
	@text	enqueue a MOAIUntzSound into the queue

	@in		MOAIUntzSoundQueue self
	@in		MOAIUntzSound the sound to queue up
	@out	nil
 */
int MOAIUntzSoundQueue::_addSound ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "UU" )

	MOAIUntzSound* sound = state.GetLuaObject < MOAIUntzSound >( 2, false );
	if ( sound ) {
		// better retain the sound until we're done with it
		sound->Retain();
		self->mQueue.push_back( sound);

		if ( !self->mCurrent ) {
			self->mCurrent = sound;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	pause
	@text	Pause the sound.

	@in		MOAIUntzSoundQueue self
	@out	nil
*/
int MOAIUntzSoundQueue::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	if ( self->mSound ) {
		self->mSound->pause ();
	}
	return 0;
}

Int64 MOAIUntzSoundQueue::StreamCallback(float* buffers, UInt32 numChannels, UInt32 numFrames, void* userdata)
{
	MOAIUntzSoundQueue *self = (MOAIUntzSoundQueue*)userdata;

	// empty buffers
	memset((float*)&buffers[0], 0, sizeof(float) * numFrames * numChannels);

	while ( self->mCurrent ) {
		UNTZ::Sound *s = self->mCurrent->mSound;
		s->getData()->getSource()->prime();
		Int64 framesRead = s->getData()->getSource()->readFrames((float*)&buffers[0],
																 numChannels,
																 numFrames,
																 s->getData()->mState);
		// apply individual sound level to what's been read
		if ( framesRead > 0 ) {
			for ( UInt32 k = 0; k < numChannels; ++k ) {
				float *in = &buffers[k*framesRead];
				for (UInt32 j = 0; j < framesRead; j++) {
					*(in) = *(in) * s->getData()->mVolume * self->GetVolume();
					in++;
				}
			}

			// return immediately what we've read, buffer is non interleaved
			// so we can't reuse it anyway to get more data
			return framesRead;
		}

		// EOF, go to next file
		if ( framesRead == 0 ) {
			// we read everything from the current sound, stop sound which will rewind it
			self->mCurrent->mSound->stop();

			// and go to next if any

                                 std::vector<MOAIUntzSound*>::const_iterator itr_end = self->mQueue.end();
                                 std::vector<MOAIUntzSound*>::const_iterator itr;
                                 for (itr = self->mQueue.begin(); itr != itr_end && *itr != self->mCurrent; ++itr);

			// std::vector<MOAIUntzSound*>::const_iterator itr = std::find(self->mQueue.begin(),
			// 															self->mQueue.end(),
			// 															self->mCurrent);
			if ( itr != self->mQueue.end() && ++itr != self->mQueue.end() ) {
				self->mCurrent = *(itr);
			} else {
				self->mCurrent = NULL;
			}
		} else {
			// When buffering, we can't sleep because we're getting called
			// on a system audio callback. Returning buffering will delay the playback
			// of this buffered audio, because its (unavailable) buffers won't be taken
			// in account in the mixer.
			// this not an issue for regular sounds - they will just be played a bit later.
			// On the other hand, for sounds in a queue, which need back to back playback, this
			// becomes an issue because there's a gap in the queue, either from the loop or from
			// going from one sound to the other. The gap is there because we didn't have the data
			// for the current sound in the queue.
			return ERR_BUFFERING;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Play the sound.

	@in		MOAIUntzSoundQueue self
	@out	nil
*/
int MOAIUntzSoundQueue::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	if ( self->mQueue.size() ) {
		if ( !self->mSound ) {
			// the mSound created is defined by the samplerate & channels of the
			// first sound in the queue. I'm guessing they all need to be the same
            if ( !self->mQueue[0]->mSound )
                return 0;
			UNTZ::SoundInfo firstSoundInfo = self->mQueue[0]->mSound->getInfo();
			self->mSound = UNTZ::Sound::create(firstSoundInfo.mSampleRate,
											   firstSoundInfo.mChannels,
											   &StreamCallback,
											   self);
	    }

		if (self->mSound->getData()->getState() == UNTZ::kPlayStateStopped) {
			self->mCurrent = *(self->mQueue.begin());
		}
	    self->mSound->play();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Animation helper for volume attribute,

	@in		MOAITransform self
	@in		number vGoal		Desired resulting value for v.
	@in		number length		Length of animation in seconds.
	@opt	number mode			The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
								MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
*/
int MOAIUntzSoundQueue::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "UNN" )

	float volume	= state.GetValue < float >( 2, 0.0f );
	float delay		= state.GetValue < float >( 3, 0.0f );

	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );

		action->SetLink ( 0, self, MOAIUntzSoundQueueAttr::Pack ( ATTR_VOLUME ), volume - self->GetVolume (), mode );

		action->SetSpan ( delay );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}

	self->SetVolume ( volume );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Sets the volume of the sound.

	@in		MOAIUntzSoundQueue self
	@opt	boolean volume			Default value is 0.
	@out	nil
*/
int MOAIUntzSoundQueue::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops the sound from playing.

	@in		MOAIUntzSoundQueue self
	@out	nil
*/
int MOAIUntzSoundQueue::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIUntzSoundQueue, "U" )

	if ( self->mSound ) {
		self->mSound->stop ();
	}

	if ( self->mCurrent && self->mCurrent->mSound ) {
		self->mCurrent->mSound->stop();
	}
	return 0;
}

//================================================================//
// MOAIUntzSoundQueue
//================================================================//

void MOAIUntzSoundQueue::SetVolume(float volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;
	mVolume = volume;
}

float MOAIUntzSoundQueue::GetVolume() const
{
	return mVolume;
}

//----------------------------------------------------------------//
bool MOAIUntzSoundQueue::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIUntzSoundQueueAttr::Check ( attrID ) && this->mSound ) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_VOLUME ) {
			this->SetVolume ( attrOp.Apply ( this->GetVolume (), op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT ));
			return true;
		}
	}
	return false;
}


//----------------------------------------------------------------//
MOAIUntzSoundQueue::MOAIUntzSoundQueue () :
	mSound ( 0 ),
	mCurrent( 0 ),
    mVolume ( 1.0 ) {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIUntzSoundQueue::~MOAIUntzSoundQueue () {

	if ( this->mSound ) {
		UNTZ::Sound::dispose ( this->mSound );
	}
	// release the objects we retained on addSound
	for (int i=0; i < mQueue.size(); i++) {
		mQueue[i]->Release();
	}
}
//----------------------------------------------------------------//
void MOAIUntzSoundQueue::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );

	state.SetField ( -1, "ATTR_VOLUME", MOAIUntzSoundQueueAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIUntzSoundQueue::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getVolume",			_getVolume },
		{ "moveVolume",			_moveVolume },
		{ "seekVolume",			_seekVolume },
		{ "setVolume",			_setVolume },
		{ "addSound",			_addSound },
		{ "isPaused",			_isPaused },
		{ "isPlaying",			_isPlaying },
		{ "pause",				_pause },
		{ "play",				_play },
		{ "stop",				_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

