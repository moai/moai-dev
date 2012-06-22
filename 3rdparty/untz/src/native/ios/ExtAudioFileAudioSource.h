//
//  ExtAudioFileAudioSource.h
//  Part of UNTZ
//
//  Created by Zach Saul (zach@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef EXTAUDIOFILEAUDIOSOURCE_H_
#define EXTAUDIOFILEAUDIOSOURCE_H_

#include "BufferedAudioSource.h"
#include <AudioToolbox/AudioToolbox.h>

class ExtAudioFileAudioSource : public BufferedAudioSource
{
public:
	ExtAudioFileAudioSource();
    virtual ~ExtAudioFileAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
    
    // BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual void setDecoderPosition(Int64 startFrame);
	virtual void doneDecoding();
    
protected:
    void setUpBuffers(float *buffer, UInt32 numChannels, UInt32 numFrames);

    ExtAudioFileRef mAudioFile;
    AudioStreamBasicDescription mClientFormat;
    AudioStreamBasicDescription mFormat;
    AudioBufferList *mpBufferList;
    SInt64 mTotalFrames;
    std::vector<float>mReadBuffer;
	RCriticalSection mDecodeLock;
};

#endif