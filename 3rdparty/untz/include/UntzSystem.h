#pragma once

#include "Untz.h"

namespace UNTZ
{
	typedef struct {
		RString name;
		int numInputChannels;
		int numOutputChannels;
	} DeviceInfo;

	class SystemData;

    class System
    {
    public:
		static System* initialize(UInt32 sampleRate, UInt32 numFrames, UInt32 options);
		static System* get();
		static void shutdown();
        
		RStringArray& getSupportedFormats();
        void setSampleRate(UInt32 sampleRate);
        UInt32 getSampleRate();
		void setOptions(UInt32 options);
		UInt32 getOptions();
		UInt32 getDeviceCount();
		DeviceInfo getDeviceInfo(UInt32 deviceIndex);
		void setInputDevice(UInt32 deviceIndex);
		void setOutputDevice(UInt32 deviceIndex);

		SystemData* getData() const { return mpData; }
	private:
        System(UInt32 sampleRate, UInt32 numFrames, UInt32 options);
		~System();
        static System* msInstance;
		SystemData* mpData;
    };
};