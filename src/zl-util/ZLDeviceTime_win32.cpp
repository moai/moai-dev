// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <zl-util/ZLDeviceTime.h>
#include <windows.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
LONGLONG _getTimerFrequencyRaw () {

	LARGE_INTEGER frequency;
	if ( QueryPerformanceFrequency ( &frequency ) ) {
		return frequency.QuadPart;
	}
	return 0;
}

double _getTimerFrequency () {

	LONGLONG frequency = _getTimerFrequencyRaw();
	return ( double )frequency;
}

//================================================================//
// ZLDeviceTime
//================================================================//

	//----------------------------------------------------------------//
	const u32 ZLDeviceTime::GetDurationInMicroSeconds (const TimeStamp& duration) {

		return (u32)GetTimeInMicroSeconds(duration);
	}

	//----------------------------------------------------------------//
	const u64 ZLDeviceTime::GetTimeInMicroSeconds (const TimeStamp& timeStamp) {

		static LONGLONG sFrequency = _getTimerFrequencyRaw ();
		return ( u64 )(( double )timeStamp / (( double )sFrequency * 1000000.0 ));
	}

	//----------------------------------------------------------------//
	double ZLDeviceTime::GetTimeInSeconds () {

		static double sFrequency = _getTimerFrequency ();
	
		if ( sFrequency > 0.0 ) {
			
			LARGE_INTEGER retTime;
			if ( QueryPerformanceCounter ( &retTime )) {
				return ( double )retTime.QuadPart / sFrequency;
			}
		}
		return 0.0;
	}
	
	//----------------------------------------------------------------//
	void ZLDeviceTime::GetTimeStamp (ZLDeviceTime::TimeStamp& timeStamp) {
		
		timeStamp = 0;

		LARGE_INTEGER retTime;
		if ( QueryPerformanceCounter ( &retTime )) {
			timeStamp = retTime.QuadPart;
		}
	}

#endif
