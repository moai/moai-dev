#pragma once

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
		SoundData() : mPlayState(kPlayStateStopped), mVolume(1.0f) {};
		PlayState getState() const { return mPlayState; }
		AudioSource* getSource() const { return mpSource; }

		float mVolume;
		bool mLooping;
	protected:
		AudioSource* mpSource;
		PlayState mPlayState;
	};
};