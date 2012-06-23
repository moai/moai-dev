//
//  MemoryAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef MEMORYAUDIOSOURCE_H_
#define MEMORYAUDIOSOURCE_H_

#include "Untz.h"
#include "BufferedAudioSource.h"

class MemoryAudioSource : public BufferedAudioSource
{
public:
	MemoryAudioSource(UNTZ::SoundInfo& info, float* interleavedData, bool ownsData);
    virtual ~MemoryAudioSource() {};
    
    // AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
	UInt32 getBitsPerSample();
        
private:
	UNTZ::SoundInfo mInfo;
};

#endif