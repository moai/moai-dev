//
//  ExtAudioFileAudioSrc.h
//  MOAIAudio
//
//  Created by Zach Saul on 5/9/11.
//  Copyright 2011 Retronyms. All rights reserved.
//

#pragma once
#include "AudioSource.h"
#include <AudioToolbox/AudioToolbox.h>

class ExtAudioFileAudioSource : public AudioSource
{
public:
	ExtAudioFileAudioSource(const RString& path);
    ~ExtAudioFileAudioSource();
	virtual UInt32 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames);
private:
    void setUpBuffers(float *buffer, UInt32 numChannels, UInt32 numFrames);

    ExtAudioFileRef mAudioFile;
    AudioStreamBasicDescription mFormat;
    AudioBufferList *mpBufferList;
};