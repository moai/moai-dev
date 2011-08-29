#include "UntzSound.h"
#include "SystemData.h"
#include "SoundData.h"
#include "AudioMixer.h"
// Audio sources
#include "UserAudioSource.h"
#include "MemoryAudioSource.h"
#ifndef __ANDROID__
	#include "OggAudioSource.h"
#endif
#if defined(WIN32)
	#include <Native/Win/DShowAudioSource.h>
#else
#ifdef __APPLE__
	#include "ExtAudioFileAudioSource.h"
#endif
#ifdef __ANDROID__
	#include "WaveFileAudioSource.h"
#endif
#endif

using namespace UNTZ;

#define	OGG_FILE_EXT ".ogg"

Sound* Sound::create(const RString& path, bool loadIntoMemory)
{
	Sound* newSound = new Sound();

	if (path.find(OGG_FILE_EXT) != RString::npos)
	{
#ifndef __ANDROID__
		OggAudioSource* source = new OggAudioSource();
		if(source->init(path, loadIntoMemory))
		{
			newSound->mpData = new UNTZ::SoundData();
			newSound->mpData->mpSource = source;
		}
		else
		{
			delete source;
			delete newSound;
			return 0;
		}
#else
		return 0;
#endif
	}
	else
	{
#if defined(WIN32)
		DShowAudioSource* source = new DShowAudioSource();
		if(source->init(path, loadIntoMemory))
		{
			newSound->mpData = new UNTZ::SoundData();
			newSound->mpData->mpSource = source;
		}
		else
		{
			printf("could not load file, %s\n", path.c_str());
			delete source;
			delete newSound;
			return 0;
		}
#else
#ifdef __APPLE__
		ExtAudioFileAudioSource *source = new ExtAudioFileAudioSource();
#endif
#ifdef __ANDROID__
        WaveFileAudioSource *source = new WaveFileAudioSource();
#endif
		if(source->init(path, loadIntoMemory))
        {
            newSound->mpData = new UNTZ::SoundData();
            newSound->mpData->mpSource = source;
        }
        else
        {
            delete source;
            delete newSound;
            return 0;
        }
#endif
	}

	return newSound;
}

Sound* Sound::create(UInt32 sampleRate, UInt32 channels, UInt32 samples, Int16* interleavedData)
{
	Sound* newSound = new Sound();

	MemoryAudioSource* source = new MemoryAudioSource(sampleRate, channels, samples, interleavedData);
    newSound->mpData = new UNTZ::SoundData();
	newSound->mpData->mpSource = source;

	return newSound;
}

Sound* Sound::create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata)
{
	Sound* newSound = new Sound();

	UserAudioSource* source = new UserAudioSource(sampleRate, channels, proc, userdata);
	newSound->mpData = new UNTZ::SoundData();
	newSound->mpData->mpSource = source;

	return newSound;
}

Sound::Sound()
{
	mpData = 0;
}

Sound::~Sound()
{
	UNTZ::System::get()->getData()->mMixer.removeSound(this);

	if(mpData)
	{
		if(mpData->mpSource)
		{
            mpData->mpSource->close();
			delete mpData->mpSource;
		}

		delete mpData;
	}
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
	mpData->mpSource->setLooping(loop);
}

bool Sound::isLooping() const
{
	return mpData->mpSource->isLooping();
}

void Sound::setLoopPoints(double startTime, double endTime)
{
	mpData->mpSource->setLoopPoints(startTime, endTime);
}

void Sound::getLoopPoints(double& startTime, double& endTime)
{
	mpData->mpSource->getLoopPoints(startTime, endTime);
}

void Sound::setPosition(double seconds)
{
	mpData->mpSource->setPosition(seconds);
}

double Sound::getPosition()
{
	return mpData->mpSource->getPosition();
}

void Sound::play()
{
	if(mpData->mPlayState == kPlayStateStopped)
	{
		mpData->mPlayState = kPlayStatePlaying;
		mpData->getSource()->setPosition(0);
	    System::get()->getData()->mMixer.addSound(this);
	}
    else if(mpData->mPlayState == kPlayStatePlaying)
        mpData->getSource()->setPosition(0);
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
	mpData->mPlayState = kPlayStateStopped;
//	UNTZ::System::get()->getData()->mMixer.removeSound(this);
	mpData->getSource()->setPosition(0);
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
	if(mpData->getSource())
	{
		info.mBitsPerSample = mpData->getSource()->getBitsPerSample();
		info.mChannels = mpData->getSource()->getNumChannels();
		info.mSampleRate = mpData->getSource()->getSampleRate();
		info.mLength = mpData->getSource()->getLength();
	}
	return info;
}
