// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#ifdef _WIN32

#include <uslscore/USDeviceTime.h>
#include <windows.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
double _getTimerFrequency () {

	LARGE_INTEGER frequency;
	if ( QueryPerformanceFrequency ( &frequency )) {
		return ( double )frequency.QuadPart;
	}
	return 0.0;
}

//================================================================//
// USDeviceTime
//================================================================//

	//----------------------------------------------------------------//
	double USDeviceTime::GetTimeInSeconds () {
		
		// TODO: check accuracy of timeGetTime vs. QueryPerformanceCounter
		//return ( double )timeGetTime () * 0.001f;

		static double sFrequency = _getTimerFrequency ();
	
		if ( sFrequency > 0.0 ) {
			
			LARGE_INTEGER retTime;
			if ( QueryPerformanceCounter ( &retTime )) {
				return ( double )retTime.QuadPart / sFrequency;
			}
		}
		return 0.0;
	}

#endif
