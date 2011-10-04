//
//  MemoryAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#ifndef MEMORYAUDIOSOURCE_H_
#define MEMORYAUDIOSOURCE_H_

#include "Untz.h"
#include "BufferedAudioSource.h"

class MemoryAudioSource : public BufferedAudioSource
{
public:
	MemoryAudioSource(UInt32 sampleRate, UInt32 numChannels, UInt32 numSamples, Int16 *interleavedData);
    ~MemoryAudioSource() {};
    
    // AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
    
	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
	virtual bool init(float* interleavedData, Int64 numSamples);
	virtual Int64 decodeData(float* buffer, UInt32 numFrames);
	virtual void setDecoderPosition(Int64 startFrame);
    
private:
    double mSampleRate;
    Int64 mNumFrames;
    UInt32 mNumChannels;
};

#endif