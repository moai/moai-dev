#ifndef SYSTEMDATA_H_
#define SYSTEMDATA_H_

#include "AudioMixer.h"

namespace UNTZ
{
	class SystemData
	{
	public:
		SystemData() {};

		virtual UInt32 getNumFrames() = 0;
		virtual UInt32 getNumOutputChannels() = 0;

		AudioMixer mMixer;
	};
};

#endif