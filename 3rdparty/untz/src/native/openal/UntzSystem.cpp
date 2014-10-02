#ifdef __OPENAL__

//
//  UntzSystem.cpp
//  Part of UNTZ
//
//  Created by Francisco Tufro. (francisco@ziplinegames.com) on 02/18/2013.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSystem.h"
#include "SystemData.h"
#include "AudioMixer.h"

#include <stdio.h>
#include <string.h>

#include <AL/al.h>
#include <AL/alut.h>
#include <pthread.h>
#include <math.h>

using namespace UNTZ;

#define CHANNELS 2

class ALSystemData : public UNTZ::SystemData
{
public:
    ALSystemData() : SystemData() {}
    ~ALSystemData() {}
    UInt32 getNumFrames();
    UInt32 getNumOutputChannels();

    pthread_t mAudioThread;

    UInt32 mSampleRate;
    UInt32 mFramesPerBuffer;
    UInt32 mOptions;
};


UInt32 ALSystemData::getNumFrames()
{
	return mFramesPerBuffer;
}
UInt32 ALSystemData::getNumOutputChannels()
{
    return CHANNELS;
}

static inline SInt16 limit_float_conv_SInt16(float inValue)
{
    return (SInt16)((1-2*signbit(inValue)) * atanf(fabs(inValue) * 2.0f) * ((2.0f / 3.14f) * 32767));
}

void *audio_loop (void *ptr) {
	fprintf(stdout, "Audio Thread running\n:");

	alutInit(0, NULL);
	alGetError();

	ALuint source;
	alGenSources(1, &source);

	ALuint buffer;
	alGenBuffers(1, &buffer);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error generating\n");
    }


	ALSystemData* alsd = (ALSystemData*) System::get ()->getData ();
	UInt32 framesPerBuffer = alsd->getNumFrames ();

	ALenum error;

	alSourceQueueBuffers(source, 1, &buffer);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error queuing\n");
    }
	alSourcePlay(source);
    if(alGetError() != AL_NO_ERROR)
    {
        fprintf(stderr, "Error playing\n");
    }


	while(true){
		ALint val;

        alGetSourcei(source, AL_BUFFERS_PROCESSED, &val);
		if ((error = alGetError()) != AL_NO_ERROR)
		{
			printf("alGetSourcei: %d\n", error);
		}

        if(val <= 0)
            continue;

        alSourceUnqueueBuffers(source, 1, &buffer);

		float float_buffer[ CHANNELS * framesPerBuffer ];
		alsd->mMixer.process ( 0, NULL, CHANNELS, float_buffer, framesPerBuffer );

		float volume = alsd->mMixer.getVolume();

		// Convert left-right buffers to short samples and interleave.
		SInt16 int_buffer[ CHANNELS * framesPerBuffer ];
		SInt16 *outbuf = &int_buffer[0];

	    for(int i=0; i<framesPerBuffer ; i++)
	    {
	        for(int j=0; j<CHANNELS; j++)
	        {
	        	*(outbuf++) = limit_float_conv_SInt16(volume * float_buffer[j*framesPerBuffer+i]); // HBS
	        }
	    }

		alBufferData ( buffer, AL_FORMAT_STEREO16, &int_buffer[0], sizeof(SInt16) * CHANNELS * framesPerBuffer , alsd->mSampleRate);

        if(alGetError() != AL_NO_ERROR)
         {
             fprintf(stderr, "Error buffering data:(\n");
         }
		alSourceQueueBuffers ( source, 1, &buffer );
        if(alGetError() != AL_NO_ERROR)
         {
             fprintf(stderr, "Error buffering :(\n");
         }

		alGetSourcei ( source, AL_SOURCE_STATE, &val );
		if ( val != AL_PLAYING )
			alSourcePlay(source);

	}


}

System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	ALSystemData *alsd = new ALSystemData();
	alsd->mSampleRate = sampleRate;
	alsd->mFramesPerBuffer = numFrames;
	alsd->mOptions = options;
	mpData = alsd;

	int iret1 = pthread_create( &alsd->mAudioThread, NULL, audio_loop, NULL);
}

System::~System()
{
	alutExit();

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
	// TODO: Implement
}

unsigned int System::getSampleRate()
{
	ALSystemData* alsd = (ALSystemData*)mpData;
	return alsd->mSampleRate;
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


#endif
