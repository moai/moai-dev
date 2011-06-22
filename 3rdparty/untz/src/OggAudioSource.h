#pragma once

#include "BufferedAudioSource.h"
#include <Threading/Threading.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <cstdio>
#include <iostream>

class OggAudioSource : public BufferedAudioSource
{
public:
	OggAudioSource();
	~OggAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();

	// BufferedAudioSource
	virtual bool open(const RString& path, bool loadIntoMemory);
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual void setDecoderPosition(Int64 startFrame);
	
private:
	void setDecoderPosition(double position);

	RString mPath;
	FILE* mInFile;
	vorbis_info* mpOggInfo;
	OggVorbis_File mOggFile;
	std::vector<float> mBuffer;
};
