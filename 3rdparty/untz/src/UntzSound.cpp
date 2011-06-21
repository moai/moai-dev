#include "UntzSound.h"
#include "SystemData.h"
#include "SoundData.h"
#include "AudioMixer.h"
// Audio sources
#include "OggAudioSource.h"
#include "UserAudioSource.h"
#include "MemoryAudioSource.h"
#if defined(WIN32)
#include <Native/Win/DShowAudioSource.h>
#else
#include "ExtAudioFileAudioSource.h"
#endif

using namespace UNTZ;

#define	OGG_FILE_EXT ".ogg"

Sound* Sound::create(const RString& path, bool loadIntoMemory)
{
	Sound* newSound = new Sound();

	if (path.find(OGG_FILE_EXT) != RString::npos)
	{
		OggAudioSource* source = new OggAudioSource();
		if(source->open(path, loadIntoMemory))
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
	}
	else
	{
#if defined(WIN32)
		DShowAudioSource* source = new DShowAudioSource();
//FIXME:		source->setSound(newSound);
		if(source->load(path))
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
		ExtAudioFileAudioSource *source = new ExtAudioFileAudioSource();
		if(source->open(path, loadIntoMemory))
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
	if(mpData)
	{
		if(mpData->mpSource)
		{
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
	    System::get()->getData()->mMixer.addSound(this);
	}
	else if(mpData->mPlayState == kPlayStatePaused)
		mpData->mPlayState = kPlayStatePaused;
}

void Sound::pause()
{
	mpData->mPlayState = kPlayStatePaused;
}

void Sound::stop()
{	
    printf("stopped sound.\n");
	mpData->mPlayState = kPlayStateStopped;
	UNTZ::System::get()->getData()->mMixer.removeSound(this);
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
