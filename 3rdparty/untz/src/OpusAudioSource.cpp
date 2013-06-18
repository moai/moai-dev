//
//  OggAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "OpusAudioSource.h"


OpusAudioSource::OpusAudioSource()
{
    opusFile = NULL;
	mInFile = 0;
}

OpusAudioSource::~OpusAudioSource()
{
	if(opusFile != NULL)
	{
		op_free(opusFile);
		opusFile = NULL;
	}
	close();
}

bool OpusAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;

	int error = 0;
	// Try opening the given file
	opusFile = op_open_file(path.c_str(), &error);
	
	if(error != 0)
	{
		std::cerr << "Error opening " << path.c_str() << " for decoding..." << std::endl;
		return false;
	}

	// Get some information about the Opus file
	total_samples = op_pcm_total(opusFile, -1);
	channels = op_channel_count(opusFile, -1);
	
	return BufferedAudioSource::init(path, loadIntoMemory);
}

void OpusAudioSource::close()
{
    BufferedAudioSource::close();
    
	if(opusFile != NULL)
	{
		op_free(opusFile);
		opusFile = NULL;
	}
}

Int64 OpusAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	if (opusFile == NULL)
		return startFrame;
	
	int status = op_pcm_seek(opusFile, startFrame);
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
	return startFrame;
}

double OpusAudioSource::getLength() 
{ 
	return total_samples / 48000.0f;
}

double OpusAudioSource::getSampleRate() 
{
	// Opus streams are *always* 48K
	return 48000;
}

UInt32 OpusAudioSource::getNumChannels()
{
	return channels;
}

UInt32 OpusAudioSource::getBitsPerSample()
{
	// Opus samples are always floating point.
	return 32;
}

Int64 OpusAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	RScopedLock l(&mDecodeLock);

	if (opusFile == NULL)
		return numFrames;
		
	float* data = (float*)malloc(sizeof(float) * numFrames * channels);

	int framesRead = op_read_float(opusFile, data, numFrames * channels, NULL);
	if(framesRead > 0)
	{
		for(int i = 0; i < channels; ++i)
		{
			float *pTemp = &buffer[i];
			for(int j = 0; j < framesRead; ++j)
			{
				*pTemp = data[j * channels + i];
				pTemp += channels;
			}
		}
	}
	else
		mEOF = true;

	return framesRead;
}
