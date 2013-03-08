// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	USDEVICETIME_H
#define	USDEVICETIME_H

//================================================================//
// USDeviceTime
//================================================================//
namespace USDeviceTime {

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


