//
//  UntzSystem.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef UNTZSYSTEM_H_
#define UNTZSYSTEM_H_

#include "Untz.h"

namespace UNTZ
{
	typedef struct {
		RString name;
		int numInputChannels;
		int numOutputChannels;
	} DeviceInfo;

	typedef enum {
		RECORDABLE = 0x100 // bitwise options
	} SystemOption;

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

		void setVolume(float volume);
		float getVolume() const;

		void suspend();
		void resume();

		SystemData* getData() const { return mpData; }
	private:
        System(UInt32 sampleRate, UInt32 numFrames, UInt32 options);
		~System();
        static System* msInstance;
		SystemData* mpData;
    };
};

#endif