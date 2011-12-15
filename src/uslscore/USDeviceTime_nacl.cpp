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
#ifdef MOAI_OS_NACL

#include <uslscore/USDeviceTime.h>
#include <time.h>
#include "moai_nacl.h"

namespace {
	int g_CLOCKS_PER_SECOND = 1000000;
	double g_startClock;
	double g_prevClock;
	int g_USDeviceTimeInit = 0;
}

//================================================================//
// USDeviceTime
//================================================================//
	
	//----------------------------------------------------------------//
	
	double USDeviceTime::GetTimeInSeconds () {
		
		if ( !g_USDeviceTimeInit ) {

			///AJV extremely ugly hack to 'detect' unix systems or any other system where the default clock is wrong
			//check out "clock() issues with beta SDK, chrome 15 vs 16" in the native client discuss Google group
			double beforeClock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );
			double beforeTime = g_core->GetTime ();

			while (( g_core->GetTime () - beforeTime ) < 1.0f ) {
				sleep ( 0.1f );
			}

			double afterClock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );

			if (( afterClock - beforeClock ) < 0.1f ) {
				//SetClocksPerSecond ( 1000 );
				g_CLOCKS_PER_SECOND = 1000;
			}
			else {
				//SetClocksPerSecond ( 1000000 );
				g_CLOCKS_PER_SECOND = 1000000;
			}

			g_startClock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );
			g_prevClock = g_startClock;
			//AJV End extremely ugly hack

			g_USDeviceTimeInit = 1;
		}

		double curclock = ( clock () / ( double ) ( g_CLOCKS_PER_SECOND ) );
		
		double clockDelta = curclock - g_prevClock;

		if ( clockDelta > 0.0 ) {
			g_startClock += clockDelta;
		}

		g_prevClock = curclock;

		return  g_startClock;
	}
#endif
