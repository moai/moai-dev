//
//  UntzSound.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSound.h"
#include "SystemData.h"
#include "SoundData.h"
#include "AudioMixer.h"
// Audio sources
#include "UserAudioSource.h"
#include "MemoryAudioSource.h"
#include "OggAudioSource.h"
#if defined(WIN32)
	#include <Native/Win/DShowAudioSource.h>
#else
	#if defined(__APPLE__)
		#include "ExtAudioFileAudioSource.h"
	#else
		#include "WaveFileAudioSource.h"
	#endif
#endif

using namespace UNTZ;

#define	OGG_FILE_EXT ".ogg"

Sound* Sound::create(const RString& path, bool loadIntoMemory)
{
	Sound* prevSound = UNTZ::System::get()->getData()->getInMemorySound(path);
	Sound* newSound = new Sound();

	if (path.find(OGG_FILE_EXT) != RString::npos)
	{
		OggAudioSource* source;
		if(prevSound && loadIntoMemory && prevSound->getData()->getSource()->isLoadedInMemory())
			// Use the existing AudioSource
			source = (OggAudioSource*)prevSound->getData()->getSource().get();
		else
			// Create a new AudioSource
			source = new OggAudioSource();
		
		if(source->init(path, loadIntoMemory))
		{
			newSound->mpData = new UNTZ::SoundData();
			newSound->mpData->mPath = path;
			if(prevSound)
				// Share the audio source
				newSound->mpData->mpSource = prevSound->getData()->getSource();
			else
				// This is the first use of the audio soruce...set it explicitly
				newSound->mpData->mpSource = AudioSourcePtr(source);

			System::get()->getData()->mMixer.addSound(newSound);
		}
		else
		{
			delete source;
			delete newSound;
			return 0;
		}
	}
	else
	{
#if defined(WIN32)
		DShowAudioSource* source;
		if(prevSound && loadIntoMemory && prevSound->getData()->getSource()->isLoadedInMemory())
			source = (DShowAudioSource*)prevSound->getData()->getSource().get();
		else
			source = new DShowAudioSource();
#elif defined(__APPLE__)
		ExtAudioFileAudioSource *source = 0;
		if(prevSound && loadIntoMemory && prevSound->getData()->getSource()->isLoadedInMemory())
			source = (ExtAudioFileAudioSource*)prevSound->getData()->getSource().get();
		else
			source = new ExtAudioFileAudioSource();
#else
        WaveFileAudioSource *source = 0;
		if(prevSound && loadIntoMemory && prevSound->getData()->getSource()->isLoadedInMemory())
			source = (WaveFileAudioSource*)prevSound->getData()->getSource().get();
		else
			source = new WaveFileAudioSource();
#endif
		
		if(source->init(path, loadIntoMemory))
        {
            newSound->mpData = new UNTZ::SoundData();
			newSound->mpData->mPath = path;

			if(prevSound)
				// Share the audio source
				newSound->mpData->mpSource = prevSound->getData()->getSource();
			else
				// This is the first use of the audio soruce...set it explicitly
				newSound->mpData->mpSource = AudioSourcePtr(source);

			System::get()->getData()->mMixer.addSound(newSound);
        }
        else
        {
            delete source;
            delete newSound;
            return 0;
        }
	}
	return newSound;
}

Sound* Sound::create(SoundInfo info, float* interleavedData, bool ownsData)
{
	Sound* newSound = new Sound();
	newSound->mpData = new UNTZ::SoundData();
	newSound->mpData->mPath = "";

	MemoryAudioSource* source = new MemoryAudioSource(info, (float*)interleavedData, ownsData);
	newSound->mpData->mpSource = AudioSourcePtr(source);

	System::get()->getData()->mMixer.addSound(newSound);

	return newSound;
}

/*
Sound* Sound::create(UInt32 sampleRate, UInt32 channels, UInt32 samples, Int16* interleavedData)
{
	Sound* newSound = new Sound();

	MemoryAudioSource* source = new MemoryAudioSource(sampleRate, channels, samples, interleavedData);
    newSound->mpData = new UNTZ::SoundData();
	newSound->mpData->mpSource = AudioSourcePtr(source);

	return newSound;
}
*/

Sound* Sound::create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata)
{
	Sound* newSound = new Sound();
	newSound->mpData = new UNTZ::SoundData();
	newSound->mpData->mPath = "";

	UserAudioSource* source = new UserAudioSource(sampleRate, channels, proc, userdata);
	newSound->mpData->mpSource = AudioSourcePtr(source);

	System::get()->getData()->mMixer.addSound(newSound);

	return newSound;
}

// Decode
bool Sound::decode(const RString& path, SoundInfo& info, float** data)
{
	bool decoded = false;
	AudioSource* source = 0;
	if (path.find(OGG_FILE_EXT) != RString::npos)
	{
		OggAudioSource* as = new OggAudioSource();
		source = as;
		if(as->init(path, true))
			decoded = true;
	}
	else
	{
#if defined(__APPLE__)
		ExtAudioFileAudioSource *as = new ExtAudioFileAudioSource();
		source = as;
		if(as->init(path, true))
			decoded = true;
#elif defined(__ANDROID__)
        WaveFileAudioSource *as = new WaveFileAudioSource();
		source = as;
		if(as->init(path, true))
			decoded = true;
#else
		DShowAudioSource* as = new DShowAudioSource();
		source = as;
		if(as->init(path, true))
			decoded = true;
#endif
	}
	
	// Couldn't decode the file
	if(!decoded)
	{
		if(source)
			delete source;
		return false;
	}

	RAudioBuffer* buffer = source->getBuffer();

	UInt32 size = buffer->getDataSize();

	// Allocate space and copy the buffer
	*data = (float*)new char[size];
	buffer->copyInto(*data);

	info.mChannels = source->getNumChannels();
	info.mBitsPerSample = 32;
	info.mSampleRate = source->getSampleRate();
	info.mTotalFrames = size / 4 / source->getNumChannels();
	info.mLength = (double)size / 4.0 / source->getNumChannels() / source->getSampleRate();

	if(source)
		delete source;
	return true;
}

void Sound::dispose ( Sound* sound )
{
	delete sound;
}

Sound::Sound()
{
	mpData = 0;
}

Sound::~Sound()
{
	stop();
	
	UNTZ::System::get()->getData()->mMixer.removeSound(this);

	if(mpData)
		delete mpData;
	mpData = 0;
}

void Sound::setVolume(float volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;
	mpData->mVolume = volume;
}

float Sound::getVolume() const
{
	return mpData->mVolume;
}

void Sound::setLooping(bool loop) 
{ 
	mpData->mState.mLooping = loop; 
}

bool Sound::isLooping() const 
{ 
	return mpData->mState.mLooping; 
}

void Sound::setLoopPoints(double startTime, double endTime)
{
	double length = mpData->getSource()->getLength();

	startTime = startTime < 0 ? 0 : startTime;
	startTime = startTime > length ? length : startTime;
	endTime = endTime < 0 ? 0 : endTime;
	endTime = endTime > length ? length : endTime;
		
	if(startTime < endTime)
	{
		mpData->mState.mLoopStart = startTime;
		mpData->mState.mLoopEnd = endTime;
	}
	else if(startTime == endTime)
	{
		mpData->mState.mLoopStart = 0.0f;
		mpData->mState.mLoopEnd = 0.0f;
	}
}

void Sound::getLoopPoints(double& startTime, double& endTime)
{
	startTime = mpData->mState.mLoopStart;
	endTime = mpData->mState.mLoopEnd;
}

void Sound::setPosition(double seconds)
{
	seconds = seconds < 0 ? 0.0f : seconds;
	seconds = seconds > mpData->getSource()->getLength() ? mpData->getSource()->getLength() : seconds;
	mpData->mState.mCurrentFrame = (Int64)(seconds * mpData->getSource()->getSampleRate());   
	mpData->getSource()->setPosition(0);
}

double Sound::getPosition()
{
	double position = (double)mpData->mState.mCurrentFrame / mpData->getSource()->getSampleRate();
	return position;
}

void Sound::play()
{
	if(mpData->mPlayState == kPlayStateStopped)
	{
		mpData->mPlayState = kPlayStatePlaying;
	}
    else if(mpData->mPlayState == kPlayStatePlaying)
		setPosition(0);
	else if(mpData->mPlayState == kPlayStatePaused)
		mpData->mPlayState = kPlayStatePlaying;        
}

void Sound::pause()
{
	if(mpData->mPlayState == kPlayStatePlaying)
		mpData->mPlayState = kPlayStatePaused;
}

void Sound::stop()
{	
	if(mpData)
	{
		mpData->mPlayState = kPlayStateStopped;
		setPosition(0);
	}
}

bool Sound::isPlaying()
{
	return mpData->mPlayState == kPlayStatePlaying;
}

bool Sound::isPaused()
{
	return mpData->mPlayState == kPlayStatePaused;
}

SoundInfo Sound::getInfo()
{
	SoundInfo info;
	if(mpData->getSource().get())
	{
		info.mBitsPerSample = mpData->getSource()->getBitsPerSample();
		info.mChannels = mpData->getSource()->getNumChannels();
		info.mSampleRate = mpData->getSource()->getSampleRate();
		info.mLength = mpData->getSource()->getLength();
	}
	return info;
}