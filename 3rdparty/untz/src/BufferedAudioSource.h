#pragma once

#include "AudioSource.h"
#include <Threading/Threading.h>

#define SECONDS_TO_BUFFER 2

class BufferedAudioSourceThread;

class BufferedAudioSource : public AudioSource
{
	friend class BufferedAudioSourceThread;
public:
	BufferedAudioSource();
	virtual ~BufferedAudioSource();

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames);
	virtual void setPosition(double seconds);
	virtual double getPosition();

	// BufferedAudioSource
	virtual bool open(const RString& path, bool loadIntoMemory) = 0;
	virtual Int64 decodeData(float* buffer, UInt32 numFrames) = 0;
	virtual void setDecoderPosition(Int64 startFrame) = 0;
	virtual bool isEOF() { return mEOF; }
	virtual bool isLoadedInMemory() { return mLoadedInMemory; }

protected:
	bool mLoadedInMemory;
	bool mEOF;
	Int64 mCurrentFrame;
	std::vector<float> mBuffer;
	RCriticalSection mLock;
};
