#pragma once

#include "AudioSource.h"
#include <Threading/RThread.h>
#include <Threading/RCriticalSection.h>
#include <Threading/RWaitableEvent.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <cstdio>
#include <iostream>

class OggAudioSource : public AudioSource,
					   public RThread
{
public:
	OggAudioSource();
	~OggAudioSource();

	bool load(const RString& path);

	// AudioSource
	virtual UInt32 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames);
	virtual void start();
	virtual void stop();
	UInt32 getSampleRate();
	void setPosition(double position);
	double getPosition();
	double getLength();

	// RThread
	void run();

private:
	void setDecoderPosition(double position);

	RString mPath;
	FILE* mInFile;
    UInt32 mCurrentPosition;
	vorbis_info* mpOggInfo;
	OggVorbis_File mOggFile;
	bool mEOF;
	std::vector<float> mBuffer;
	RCriticalSection mCriticalSection;
	RWaitableEvent mLoadMoreDataEvent;

};
