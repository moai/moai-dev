/*	All contents Copyright (c) 2007-10 by Patrick Meehan
    
    Permission is hereby granted irrevocably and in perpetuity
    to Groundspeak Inc., which, having obtained a copy of this
    software and associated documentation files (the "Software"),
    is free to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the
    Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include "pch.h"
#ifdef __linux

#include <uslscore/USDeviceTime.h>
#include <time.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static long _getTimerInfo () {
	
	struct timespec ts;
	clock_gettime ( CLOCK_MONOTONIC, &ts );
	
	return ts.tv_nsec;
}

//================================================================//
// USDeviceTime
//================================================================//

	//----------------------------------------------------------------//
	
	double USDeviceTime::GetTimeInSeconds () {
			
		#ifndef ANDROID
			
			static long last_time = _getTimerInfo (); // in nanoseconds
		
			long this_time = _getTimerInfo ();

			double time = static_cast < double >( this_time - last_time ) * ( 1e-9 );

			last_time = this_time;

			return time;

		#else
			
			struct timespec timer;
			timer.tv_nsec = 0;
			clock_gettime ( CLOCK_MONOTONIC, &timer );
			static double last_time = timer.tv_sec + ( double )( timer.tv_nsec * 1e-9 );// in nanoseconds
		
			
			double time = ( timer.tv_sec + ( double )( timer.tv_nsec*1e-9 )) - last_time;

			return time;

		#endif
	}
#endif
