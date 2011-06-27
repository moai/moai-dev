
#pragma once;

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
		static Sound* create(const RString& path, bool loadIntoMemory = false);
		// Create a sound object from a memory buffer
		static Sound* create(UInt32 sampleRate, UInt32 channels, UInt32 samples, Int16* inverleavedData);
		// Create a sound object that streams audio data via a callback
		static Sound* create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata);

		void setLooping(bool loop);
		bool isLooping() const;
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