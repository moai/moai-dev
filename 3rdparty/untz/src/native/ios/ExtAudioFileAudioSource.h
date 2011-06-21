
#pragma once

#include "BufferedAudioSource.h"
#include <AudioToolbox/AudioToolbox.h>

class ExtAudioFileAudioSource : public BufferedAudioSource
{
public:
	ExtAudioFileAudioSource();
    ~ExtAudioFileAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
    
    // BufferedAudioSource
	virtual bool open(const RString& path, bool loadIntoMemory);
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual void setDecoderPosition(Int64 startFrame);
    
protected:
    void setUpBuffers(float *buffer, UInt32 numChannels, UInt32 numFrames);

    ExtAudioFileRef mAudioFile;
    AudioStreamBasicDescription mClientFormat;
    AudioStreamBasicDescription mFormat;
    AudioBufferList *mpBufferList;
    SInt64 mTotalFrames;
    std::vector<float>mReadBuffer;
};