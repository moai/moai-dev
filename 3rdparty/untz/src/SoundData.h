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
		SoundData() : mPlayState(kPlayStateStopped), mVolume(1.0f) {};
		PlayState getState() const { return mPlayState; }
		void setState(PlayState state) { mPlayState = state; }
		AudioSource* getSource() const { return mpSource; }

		float mVolume;
	protected:
		AudioSource* mpSource;
		PlayState mPlayState;
	};
};

#endif