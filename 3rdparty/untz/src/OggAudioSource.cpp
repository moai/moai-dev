//
//  OggAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "OggAudioSource.h"


OggAudioSource::OggAudioSource() : mpOggInfo(NULL)
{ }

OggAudioSource::~OggAudioSource()
{
	close();
}

bool OggAudioSource::init(const RString& path, bool loadIntoMemory)
{
	if(mLoadedInMemory && loadIntoMemory)
		return true;

	const int status = ov_fopen(path.c_str(), &mOggFile);

	if( status != 0 )
	{
		printError("Cannot open %s for reading, error: %s \n", path.c_str(), getErrorDescription( status) );
		return false;
	}

	// Get some information about the OGG file
	mpOggInfo = ov_info(&mOggFile, -1);

	return BufferedAudioSource::init(path, loadIntoMemory);
}

void OggAudioSource::close()
{
    BufferedAudioSource::close();
	ov_clear(&mOggFile);
}

void OggAudioSource::setDecoderPosition(Int64 startFrame)
{
	RScopedLock l(&mDecodeLock);

	const int status = ov_pcm_seek(&mOggFile, startFrame * getNumChannels());

	if ( status == 0 ) {
		if ( startFrame < ( getLength() * getSampleRate() ) ) {
			mEOF = false;
		}
	 } else {
		 printError("Failed ov_pcm_seek: %s\n", getErrorDescription(status) );
	 }
}

const char* OggAudioSource::getErrorDescription(const int status) const
{
	switch ( status ) {
		case OV_ENOSEEK		: return "Bitstream is not seekable.";
		case OV_EINVAL		: return "Invalid argument value.";
		case OV_EREAD		: return "A read from media returned an error.";
		case OV_EFAULT		: return "Internal logic fault; indicates a bug or heap/stack corruption.";
		case OV_EBADLINK	: return "Invalid stream section supplied.";
		case OV_ENOTVORBIS	: return "The given file/data was not recognized as Ogg Vorbis data.";
		case OV_EBADHEADER	: return "The file/data is apparently an Ogg Vorbis stream, \
										but contains a corrupted or undecipherable header.";
		case OV_EVERSION	: return "The bitstream format revision of the given stream is not supported.";
		default				: return "Unknown error description.";
	}
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
