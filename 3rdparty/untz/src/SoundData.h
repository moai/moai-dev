//
//  SoundData.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include "Untz.h"
#include "AudioSource.h"

namespace UNTZ
{
	typedef enum {
		kPlayStateStopped = 0,
		kPlayStatePlaying,
		kPlayStatePaused,
	} PlayState;

	class SoundData
	{
		friend class Sound;
	public:
		SoundData() : mPlayState(kPlayStateStopped), mVolume(1.0f) 
		{
			mState.mCurrentFrame = 0;
			mState.mLooping = false;
			mState.mLoopStart = 0.0;
			mState.mLoopEnd = 0.0;

			mPlayState == kPlayStateStopped;
		};

		PlayState getState() const { return mPlayState; }
		void setState(PlayState state) { mPlayState = state; }
		AudioSourcePtr getSource() const { return mpSource; }

		float mVolume;
		AudioSourceState mState;
		RString mPath;
	protected:
		AudioSourcePtr mpSource;
		PlayState mPlayState;
	};
};

#endif