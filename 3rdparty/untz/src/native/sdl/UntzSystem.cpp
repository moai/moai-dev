#ifdef __SDL__

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

#include <SDL.h>
#include <SDL_audio.h>
#include <pthread.h>
#include <math.h>

using namespace UNTZ;

#define CHANNELS 2

class SDLSystemData : public UNTZ::SystemData
{
public:
	SDLSystemData() : SystemData() {}
  ~SDLSystemData() {}
  UInt32 getNumFrames();
  UInt32 getNumOutputChannels();

  pthread_t mAudioThread;

  UInt32 mSampleRate;
  UInt32 mFramesPerBuffer;
  UInt32 mOptions;
};


UInt32 SDLSystemData::getNumFrames()
{
	return mFramesPerBuffer;
}
UInt32 SDLSystemData::getNumOutputChannels()
{
    return CHANNELS;
}

static inline SInt16 limit_float_conv_SInt16(float inValue)
{
    //return (SInt16)((1-2*signbit(inValue)) * atanf(fabs(inValue) * 2.0f) * ((2.0f / 3.14f) * 32767));
		return (SInt16)(inValue * 32767);
}

void SDL_callback (void *unused, Uint8 *_stream, int _length) {

    SDLSystemData* alsd = (SDLSystemData*) System::get ()->getData ();

	const int length = _length / CHANNELS;
	const int length_by_channels = length / CHANNELS;
	const float volume = alsd->mMixer.getVolume();
	float float_buffer[ length ];

	alsd->mMixer.process ( 0, NULL, CHANNELS, float_buffer, length_by_channels);

	// Convert left-right buffers to short samples and interleave.
	SInt16 intBuffer[ length ];
//	SInt16 *outbuf = &intBuffer[0];
	SInt16 *outbuf= (SInt16*) _stream;

    for(int i=0; i<length_by_channels; i++)
    {
        for(int j=0; j<CHANNELS; j++)
        {
        	*(outbuf++) = limit_float_conv_SInt16(volume * float_buffer[j*length_by_channels+i]); // HBS
        }
    }


//    SDL_MixAudio(_stream, (UInt8*)&intBuffer[0], length, SDL_MIX_MAXVOLUME);
}

void *audio_loop (void *ptr) {
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
	}
	fprintf(stdout, "Audio Thread running:\n");

	SDLSystemData* alsd = (SDLSystemData*) System::get ()->getData ();

	SDL_AudioSpec fmt;
	UInt32 framesPerBuffer = alsd->getNumFrames ();

	/* Set 16-bit stereo audio at 22Khz */
	fmt.freq = alsd->mSampleRate;
	fmt.format = AUDIO_S16;
	fmt.channels = CHANNELS;
	fmt.samples = framesPerBuffer;
	fmt.callback = SDL_callback;
	fmt.userdata = NULL;

	/* Open the audio device and start playing sound! */
	if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
	}
	
    SDL_PauseAudio(0);

	return 0;
}

System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	SDLSystemData *alsd = new SDLSystemData();
	alsd->mSampleRate = sampleRate;
	alsd->mFramesPerBuffer = numFrames;
	alsd->mOptions = options;
	mpData = alsd;

	int iret1 = pthread_create( &alsd->mAudioThread, NULL, audio_loop, NULL);
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
	// TODO: Implement
}

unsigned int System::getSampleRate()
{
	SDLSystemData* alsd = (SDLSystemData*)mpData;
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
