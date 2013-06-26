//
//  FLACAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef FLACAUDIOSOURCE_H
#define FLACAUDIOSOURCE_H

#include "BufferedAudioSource.h"
#include <threading/Threading.h>
#ifdef __APPLE__
#include <libFLAC/stream_decoder.h>
#else
extern "C" {
	#include <libFLAC/stream_decoder.h>
}
#endif
#include <vector>
#include <cstdio>
#include <iostream>

class FLACAudioSource : public BufferedAudioSource
{
public:
	FLACAudioSource();
	~FLACAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
	UInt32 getBitsPerSample();

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual Int64 setDecoderPosition(Int64 startFrame);

	FLAC__StreamDecoderWriteStatus flac_write(const FLAC__Frame *frame, const FLAC__int32 * const buffer[]);
	void flac_metadata(const FLAC__StreamMetadata *metadata);
	void flac_error(FLAC__StreamDecoderErrorStatus status);
	
private:
	void setDecoderPosition(double position);
	
	RCriticalSection mDecodeLock;
	FLAC__StreamDecoder *decoder;
	bool gotMetadata;
	FLAC__uint64 total_samples;
	unsigned int sample_rate;
	unsigned int channels;
	unsigned int bps;
	float *flac_buffer;
	unsigned int bufferSize;
	unsigned int bufferFrames;
	unsigned int bufferIndex;
};


FLAC__StreamDecoderWriteStatus FLAC_write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data);

void FLAC_metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data);

void FLAC_error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data);


#endif