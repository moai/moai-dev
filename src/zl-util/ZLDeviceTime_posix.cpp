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
#if defined ( __linux ) | defined ( __FLASCC__ ) | defined ( __QNX__ ) | defined ( __EMSCRIPTEN__ )

#include <zl-util/ZLDeviceTime.h>
#include <time.h>

#if defined( __EMSCRIPTEN__ )
#include <emscripten.h>
#include <math.h>
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static long _getTimerInfo () {
	
	struct timespec ts;
	ts.tv_nsec = 0;
	clock_gettime ( CLOCK_MONOTONIC, &ts );
	return ts.tv_sec*1000000000LL + ts.tv_nsec;
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
		
		return timeStamp / 1000;
	}

	//----------------------------------------------------------------//
	double ZLDeviceTime::GetTimeInSeconds () {
			
		#if defined ( ANDROID ) | defined ( __linux ) | defined ( __QNX__ )
			     
      struct timespec timer;
      timer.tv_nsec = 0;
      clock_gettime ( CLOCK_MONOTONIC, &timer );
      static double last_time = timer.tv_sec + ( double )( timer.tv_nsec * 1e-9 );// in nanoseconds
      
      double time = ( timer.tv_sec + ( double )( timer.tv_nsec*1e-9 )) - last_time;

      return time;

		#elif defined ( __EMSCRIPTEN__ )
		   //emscripten has no clock_gettime
		   double browsertime = emscripten_get_now()/1000;
		   static double last_time = browsertime;
		   double time = browsertime - last_time;
			return time;			

		#else

      static long last_time = _getTimerInfo (); // in nanoseconds
    
      long this_time = _getTimerInfo ();

      double time = static_cast < double >( this_time - last_time ) * ( 1e-9 );

      // of: Why would we override last_time when we want to get the accumulated time
      //last_time = this_time;

      return time;

		#endif
	}
	
	//----------------------------------------------------------------//
	void ZLDeviceTime::GetTimeStamp ( ZLDeviceTime::TimeStamp& timeStamp ) {
		
		#if defined ( ANDROID ) | defined ( __linux )

			struct timespec timer;
			timer.tv_nsec = 0;
			clock_gettime ( CLOCK_MONOTONIC, &timer );
			timeStamp = ( ZLDeviceTime::TimeStamp )(( timer.tv_sec * 1000000000LL ) + timer.tv_nsec );
		#elif defined ( __EMSCRIPTEN__ )
		   //emscripten has no clock_gettime
			
			
           double browsertime = emscripten_get_now();

		   timeStamp = ( ZLDeviceTime::TimeStamp )( static_cast<long long>(floor(browsertime * 1000))*1000);
		   

		#else

			// PCM: This func does the same thing as above. So why the #else? Leaving for now.
			timeStamp = _getTimerInfo ();

		#endif
	}

#endif
