//
//  UntzSound.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSound.h"


//externs
extern "C" int untzsound_createfromfile(char const *path, bool loadIntoMemory);
extern "C" void untzsound_destroy(int handle);
extern "C" void untzsound_setVolume(int handle, float volume);
extern "C" float untzsound_getVolume(int handle);
extern "C" void untzsound_setLooping(int handle, bool loop);
extern "C" bool untzsound_isLooping(int handle);
extern "C" void untzsound_setLoopPoints(int handle, double startTime, double endTime);
extern "C" double untzsound_getLoopStart(int handle);
extern "C" double untzsound_getLoopEnd(int handle);
extern "C" void untzsound_setPosition(int handle, double seconds);
extern "C" double untzsound_getPosition(int handle);

extern "C" void untzsound_play(int handle);
extern "C" void untzsound_pause(int handle);
extern "C" void untzsound_stop(int handle);
extern "C" bool untzsound_isPlaying(int handle);
extern "C" bool untzsound_isPaused(int handle);

extern "C" UInt32 untzsound_getNumChannels(int handle);
extern "C" double untzsound_getSampleRate(int handle);
extern "C" double untzsound_getLength(int handle);

      struct AudioSourceState
      {
      	Int64 mCurrentFrame;
      	double mLoopStart;
      	double mLoopEnd;
      	bool mLooping;
      };


namespace UNTZ {
        class SoundData {
          	  	friend class Sound;
              	public:
              	  SoundData(int id) {
                      mId = id;
              	  };
              	  int mId;
              	  SoundInfo mInfo;
       	};
};


using namespace UNTZ;








Sound* Sound::create(const RString& path, bool loadIntoMemory)
{
  int id = untzsound_createfromfile(path.c_str(),loadIntoMemory);
	Sound* newSound = new Sound();
	newSound->mpData = new SoundData(id);
	return newSound;
}

Sound* Sound::create(SoundInfo info, float* interleavedData, bool ownsData)
{
	Sound* newSound = new Sound();
	newSound->mpData = new SoundData(0);
	newSound->mpData->mInfo = info;
	//we will implement this if needed later on
	return newSound;
}


Sound* Sound::create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata)
{
	return NULL; //die hard
}

// Decode
bool Sound::decode(const RString& path, SoundInfo& info, float** data)
{
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

	if(mpData)  {
	    if (mpData->mId > 0)
        untzsound_destroy(mpData->mId);
			delete mpData;
	}

	mpData = 0;
}

void Sound::setVolume(float volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;
  untzsound_setVolume(mpData->mId,volume);
}

float Sound::getVolume() const
{
   return  untzsound_getVolume(mpData->mId);
}

void Sound::setLooping(bool loop) 
{ 
	 untzsound_setLooping(mpData->mId, loop);
}

bool Sound::isLooping() const 
{ 
	 return untzsound_isLooping(mpData->mId);
}

void Sound::setLoopPoints(double startTime, double endTime)
{
	double length = untzsound_getLength(mpData->mId);

	startTime = startTime < 0 ? 0 : startTime;
	startTime = startTime > length ? length : startTime;
	endTime = endTime < 0 ? 0 : endTime;
	endTime = endTime > length ? length : endTime;
		
	if(startTime < endTime)
	{
		untzsound_setLoopPoints(mpData->mId,startTime,endTime);
	}
	else if(startTime == endTime)
	{
	   untzsound_setLoopPoints(mpData->mId,0.0f,0.0f);
	}
}

void Sound::getLoopPoints(double& startTime, double& endTime)
{
	startTime = untzsound_getLoopStart(mpData->mId);
	endTime = untzsound_getLoopEnd(mpData->mId);
}

void Sound::setPosition(double seconds)
{
    untzsound_setPosition(mpData->mId,seconds);
}

double Sound::getPosition()
{
	return untzsound_getPosition(mpData->mId);
}

void Sound::play()
{
  untzsound_play(mpData->mId);
}

void Sound::pause()
{
  untzsound_pause(mpData->mId);
}

void Sound::stop()
{	
  untzsound_stop(mpData->mId);
}

bool Sound::isPlaying()
{
  return untzsound_isPlaying(mpData->mId);
}

bool Sound::isPaused()
{
  return untzsound_isPaused(mpData->mId);
}

SoundInfo Sound::getInfo()
{
	SoundInfo info;

		info.mBitsPerSample = 32;
		info.mChannels = untzsound_getNumChannels(mpData->mId);
		info.mSampleRate = untzsound_getSampleRate(mpData->mId);
		info.mLength = untzsound_getLength(mpData->mId);

	return info;
}
