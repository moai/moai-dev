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
#ifdef NACL

#include <uslscore/USDeviceTime.h>
#include <time.h>
#include "moai_nacl.h"

namespace {
	int g_CLOCKS_PER_SECOND = CLOCKS_PER_SEC;
	double g_startClock;
	double g_prevClock;
	int g_USDeviceTimeInit = 0;
	bool useTimeOfDay = false;
	double getTimeOfDay () {

		timeval _time;
		gettimeofday ( &_time, NULL );

		return ( _time.tv_sec + ( _time.tv_usec / 1000000.0 ));
	}

	double getClock () {
		if ( useTimeOfDay ) {
			return getTimeOfDay ();
		}
		else {
			return ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );
		}
	}
}

//================================================================//
// USDeviceTime
//================================================================//
	
	//----------------------------------------------------------------//
	
	double USDeviceTime::GetTimeInSeconds () {
		
		if ( !g_USDeviceTimeInit ) {

			//AJV extremely ugly hack to 'detect' unix systems or any other system where the default clock is wrong
			//check out "clock() issues with beta SDK, chrome 15 vs 16" in the native client discuss Google group
			//TODO: gettimeofday will produce high precision results in Chrom 17, switch to this in pepper_17
			for ( int i = 0; i < 2; ++i ) {

				double beforeTime = getTimeOfDay ();
				double currentTime = beforeTime;

				double beforeClock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );

				while (( currentTime - beforeTime ) < 0.25f ) {
					
					currentTime = getTimeOfDay ();
					usleep ( 1000 );
				}


				double afterClock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );
				double diff = afterClock - beforeClock;

				if ( diff < .23 || diff > 0.27 ) {
					useTimeOfDay = true;
				}
				else {
					useTimeOfDay = false;
				}

			}

			g_startClock = getClock ();
			g_prevClock = g_startClock;
			//AJV End extremely ugly hack

			g_USDeviceTimeInit = 1;
		}

		double curclock = getClock ();
		
		
		double clockDelta = curclock - g_prevClock;

		if ( clockDelta > 0.0 ) {
			g_startClock += clockDelta;
		}

		g_prevClock = curclock;

		return  g_startClock;
	}
#endif
