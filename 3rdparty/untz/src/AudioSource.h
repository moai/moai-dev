#pragma once

#include "Untz.h"

class AudioSource
{
protected:
	AudioSource() :  mLooping(false) {};
public:
	virtual ~AudioSource() {};

    virtual void close() {};
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames) = 0;
	virtual void setPosition(double position) {};
	virtual double getPosition() { return 0; }
	virtual UInt32 getBitsPerSample() { return 0; }
	virtual double getSampleRate() { return 0; }
	virtual UInt32 getNumChannels() { return 0; }
	virtual double getLength() { return 0; }
    
    void setLooping(bool loop) { mLooping = loop; };
    bool isLooping() const { return mLooping; };
	double convertSamplesToSeconds(Int64 samples) 
	{
		double time = (double)samples;
		return time / getSampleRate();
	}
	Int64 convertSecondsToSamples(float seconds)
	{
		return (Int64)(seconds * getSampleRate());
	}

	Int64 convertSecondsToBytes(float seconds)
	{
		return (Int64)(seconds * getSampleRate() * getBitsPerSample() / 8);
	}

protected:
	bool mLooping;
};