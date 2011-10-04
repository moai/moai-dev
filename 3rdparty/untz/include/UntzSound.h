//
//  UntzSound.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#ifndef UNTZSOUND_H_
#define UNTZSOUND_H_

#include "Untz.h"

namespace UNTZ
{
	typedef UInt32 (StreamCallback)(float* buffers, UInt32 numChannels, UInt32 length, void* userdata); 

	typedef struct {
		UInt32 mBitsPerSample;
		UInt32 mChannels;
		double mSampleRate;
		double mLength;
	} SoundInfo;

	class SoundData;

    class Sound
    {         
    public:
		~Sound();
		
		// Create a sound object from a file path
		static Sound* create(const RString& path, bool loadIntoMemory = true);
		// Create a sound object from a memory buffer
		static Sound* create(UInt32 sampleRate, UInt32 channels, UInt32 samples, Int16* inverleavedData);
		// Create a sound object that streams audio data via a callback
		static Sound* create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata);

		void setLooping(bool loop);
		bool isLooping() const;
		void setLoopPoints(double startTime, double endTime);
		void getLoopPoints(double& startTime, double& endTime);
        void setVolume(float volume);
		float getVolume() const;
		void setPosition(double seconds);
		double getPosition();
		SoundInfo getInfo();
		
		void play();
		void pause();
		void stop();
		bool isPlaying();
		bool isPaused();

		SoundData* getData() const { return mpData; }
	private:
		Sound();
		SoundData* mpData;
    };
};

#endif