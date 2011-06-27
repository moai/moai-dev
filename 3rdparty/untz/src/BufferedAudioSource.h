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
    ~BufferedAudioSource();

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames);
	virtual void setPosition(double seconds);
	virtual double getPosition();

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
	virtual bool init(float* interleavedData, Int64 numSamples);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 numFrames) = 0;
	virtual void setDecoderPosition(Int64 startFrame) = 0;

protected:
    bool isEOF() { return mEOF; }
    bool isLoadedInMemory() { return mLoadedInMemory; }
    
	bool mLoadedInMemory;
	bool mEOF;
	Int64 mCurrentFrame;
	std::vector<float> mBuffer;
	RCriticalSection mLock;
};
