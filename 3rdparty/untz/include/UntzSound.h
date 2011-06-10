
#pragma once;

#include "Untz.h"

namespace UNTZ
{
	typedef UInt32 (StreamCallback)(float* buffers, UInt32 numChannels, UInt32 length, void* userdata); 

	typedef struct {
		UInt32 mBitsPerSample;
		UInt32 mSampleRate;
		UInt32 mChannels;
	} SoundInfo;

	class SoundData;

    class Sound
    {         
    public:
		// Create a sound object from a file path
		static Sound* create(const RString& path);
		// Create a sound object from a memory buffer
		static Sound* create(UInt32 sampleRate, UInt32 channels, UInt32 samples, Int16* inverleavedData);
		// Create a sound object that streams audio data via a callback
		static Sound* create(UInt32 sampleRate, UInt32 channels, StreamCallback* proc, void* userdata);

		void setLooping(bool loop);
		bool getLooping() const;
        void setVolume(float volume);
		float getVolume() const;

		void setPosition(double seconds);
		double getPosition();
		double getLength();
		
		void play();
		void pause();
		void stop();
		bool isPlaying();
		bool isPaused();

		SoundData* getData() const { return mpData; }
	private:
		Sound();
		~Sound();
		SoundData* mpData;
    };
};