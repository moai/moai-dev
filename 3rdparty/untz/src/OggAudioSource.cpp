//
//  OggAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "OggAudioSource.h"


OggAudioSource::OggAudioSource()
{
    mpOggInfo = NULL;
	mInFile = 0;
}

OggAudioSource::~OggAudioSource()
{
	close();
}

bool OggAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;

	mPath = path;
	mInFile = fopen(mPath.c_str(), "rb");

	if(mInFile == NULL)
	{
		std::cerr << "Cannot open " << mPath.c_str() << " for reading..." << std::endl;
		return false;
	}

	// Try opening the given file
	if(ov_open(mInFile, &mOggFile, NULL, 0) != 0)
	{
		std::cerr << "Error opening " << mPath.c_str() << " for decoding..." << std::endl;
		return false;
	}

	// Get some information about the OGG file
	mpOggInfo = ov_info(&mOggFile, -1);

	return BufferedAudioSource::init(path, loadIntoMemory);
}

void OggAudioSource::close()
{
    BufferedAudioSource::close();
    
	if(mInFile)
	{
		ov_clear(&mOggFile);
		mInFile = 0;
	}
}

void OggAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	int status = ov_pcm_seek(&mOggFile, startFrame * getNumChannels());
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
}

double OggAudioSource::getLength() 
{ 
	return ov_time_total(&mOggFile, -1);
}

double OggAudioSource::getSampleRate() 
{
	if(mpOggInfo)
		return mpOggInfo->rate;
	return 0; 
}

UInt32 OggAudioSource::getNumChannels()
{
	if(mpOggInfo)
		return mpOggInfo->channels;
	return 0; 
}

UInt32 OggAudioSource::getBitsPerSample()
{
	if(mpOggInfo)
		return mpOggInfo->bitrate_upper;
	return 0;
}

Int64 OggAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
	RScopedLock l(&mDecodeLock);

	int bitStream;
	float** data = 0;

	long framesRead = ov_read_float(&mOggFile, &data, numFrames, &bitStream);	
	if(framesRead > 0)
	{
		for(int i = 0; i < getNumChannels(); ++i)
		{
			int channels = getNumChannels();
			float *pTemp = &buffer[i];
			for(int j = 0; j < framesRead; ++j)
			{
				*pTemp = data[i][j];
				pTemp += channels;
			}
		}
	}
	else
		mEOF = true;

	return framesRead;
}
