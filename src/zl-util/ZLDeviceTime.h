// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDEVICETIME_H
#define ZLDEVICETIME_H

//================================================================//
// ZLDeviceTime
//================================================================//
namespace ZLDeviceTime {

	#ifdef _WIN32
		typedef long long TimeStamp;
	#elif __APPLE__
		typedef uint64_t TimeStamp;
	#elif ANDROID
		typedef long TimeStamp;
	#elif __linux
		#ifndef ANDROID
			typedef long TimeStamp;
		#else
			typedef long long TimeStamp;
		#endif
	#elif NACL
		typedef long long TimeStamp;
	#elif __FLASCC__
		typedef long long TimeStamp;
	#elif __QNX__
		typedef long long TimeStamp;
    #elif __EMSCRIPTEN__
		typedef long long TimeStamp;
	#else
		#pragma error("Unsupported platform")
	#endif

	//----------------------------------------------------------------//
	const u32	GetDurationInMicroSeconds	(const TimeStamp& duration);
	const u64	GetTimeInMicroSeconds		(const TimeStamp& timeStamp);
	double		GetTimeInSeconds			();
	void		GetTimeStamp				(TimeStamp& timeStamp);
}

#endif


