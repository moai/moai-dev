//
//  UntzSystem.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UntzSystem.h"

#include <stdio.h>
#include <string.h>

using namespace UNTZ;



    extern "C" void untz_setVolume(float volume);

  extern "C" float untz_getVolume();

  extern "C" void untz_suspend();
  extern "C" void untz_resume();

/* The windows system data object
 *
 */
namespace UNTZ
{
	class SystemData
	{

	};

};


System* System::msInstance = 0;

System::System(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{

}

System::~System()
{
	if(mpData)
		delete mpData;
}

void System::shutdown()
{
	if(msInstance)
	{
		delete msInstance;
		msInstance = 0;
	}
}

System* System::initialize(UInt32 sampleRate, UInt32 numFrames, UInt32 options)
{
	if(!msInstance)
	{
		msInstance = new System(sampleRate, numFrames, options);
	}
	return msInstance;
}

System* System::get()
{
	return msInstance;
}

void System::setSampleRate(UInt32 sampleRate)
{
  //no-op just ignore
}

unsigned int System::getSampleRate()
{
	return 44100;
}

void System::setVolume(float volume)
{
	untz_setVolume(volume);
}

float System::getVolume() const
{
	return untz_getVolume();
}

void System::suspend()
{
	untz_suspend();
}

void System::resume()
{
	untz_resume();
}