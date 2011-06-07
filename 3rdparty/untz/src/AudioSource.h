#pragma once

#include "Untz.h"

class UNTZ::SoundData;

class AudioSource
{
public:
	AudioSource() :  mLooping(false) {};
	virtual ~AudioSource() {};
	
	virtual UInt32 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames) = 0;
	virtual void start() {};
	virtual void stop() {};
	virtual void setPosition(double position) {};
	virtual double getPosition() { return 0; }
	virtual UInt32 getBitsPerSample() { return 0; }
	virtual UInt32 getSampleRate() { return 0; }
	virtual UInt32 getNumChannels() { return 0; }
	virtual double getLength() { return 0; }
	
	void setLooping(bool loop) { mLooping = loop; };
	bool getLooping() const { return mLooping; };
	double convertSamplesToSeconds(Int64 samples) 
	{
		double time = (double)samples;
		return time / getSampleRate();
	}
	Int64 convertSecondsToSamples(float seconds)
	{
		return (Int64)(seconds * getSampleRate());
	}

	void setSound(UNTZ::Sound *sound) { mpSound = sound; }
protected:
	UNTZ::Sound *mpSound;
	bool mLooping;
};