//
//  UntzSystem.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSystem.h"
#include "SystemData.h"
#include "AudioMixer.h"
#include "RtAudio.h"
#include <stdio.h>
#include <string.h>

using namespace UNTZ;


int RtInOut( void* outputBuffer, void* inputBuffer, unsigned int framesPerBuffer, 
			double streamTime, RtAudioStreamStatus status, void *userdata )
{
	if(!UNTZ::System::get()->getData()->isActive())
	{
		memset(outputBuffer, 0, sizeof(float) * framesPerBuffer * UNTZ::System::get()->getData()->getNumOutputChannels());
		return 0;
	}

	if(status)
		std::cout << "Stream underflow detected!" << std::endl;	
	AudioMixer *mixer = (AudioMixer*)userdata;
	mixer->process(0, NULL, UNTZ::System::get()->getData()->getNumOutputChannels(), (float*)outputBuffer, framesPerBuffer);
	
    // volume & clipping
    // HBS
    UInt32 samples = UNTZ::System::get()->getData()->getNumOutputChannels() * framesPerBuffer;
	float volume = mixer->getVolume();
    // TODO: doing an extra read/write here is painful...
    float *outB = (float*)outputBuffer;
    for(UInt32 k = 0; k < samples; ++k)
    {
        float val = *outB * volume;
        val = val > 1.0 ? 1.0 : val;
        val = val < -1.0 ? -1.0 : val;
        *(outB)++ = val;
    }

	return 0;
}

/* The windows system data object
 *
 */
class WinSystemData : public UNTZ::SystemData
{
public:
	WinSystemData(int numFrames) : SystemData(), mNumFrames(numFrames), audioIO(RtAudio::WINDOWS_DS) {};

	// SystemData
	virtual unsigned int getNumFrames()
	{
		return mNumFrames;
	}
	virtual unsigned int getNumOutputChannels()
	{
		return 2;
	}

	RtAudio audioIO;
	UInt32 mNumFrames;
	UInt32 mOptions;
};


System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	WinSystemData *wsd = new WinSystemData(numFrames);
	wsd->mOptions = options;
	mpData = wsd;

	RtAudio::StreamParameters outParams;
	outParams.nChannels = 2;
	outParams.deviceId = wsd->audioIO.getDefaultOutputDevice();
	RtAudio::StreamOptions streamOptions;
	streamOptions.flags = 0 | RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_NONINTERLEAVED;
	try 
	{
		wsd->audioIO.openStream( &outParams, NULL, RTAUDIO_FLOAT32, sampleRate, &numFrames, &RtInOut, (void *)&mpData->mMixer, &streamOptions );
		wsd->audioIO.startStream();
	}
	catch(RtError& error)
	{
		wsd->setError(true);
		printf("!!!AudioIO Error: %s\n", error.getMessage());
	}
}

System::~System()
{
	if(mpData)
		delete mpData;
}

void System::shutdown()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = 0;
	}
}

System* System::initialize(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	if(!msInstance)
	{
		msInstance = new System(sampleRate, numFrames, options);
		if(msInstance->mpData->getError())
		{
			delete msInstance;
		}
		else
		{
			msInstance->mpData->mMixer.init();
		}
	}

	return msInstance;
}

System* System::get()
{
	return msInstance;
}

void System::setSampleRate(UInt32 sampleRate)
{
}

unsigned int System::getSampleRate()
{
	WinSystemData* wsd = (WinSystemData*)mpData;
	return wsd->audioIO.getStreamSampleRate();
}

void System::setVolume(float volume)
{
	return msInstance->mpData->mMixer.setVolume(volume);
}

float System::getVolume() const
{
	return msInstance->mpData->mMixer.getVolume();
}

void System::suspend()
{
	msInstance->mpData->setActive(false);
}

void System::resume()
{
	msInstance->mpData->setActive(true);
}