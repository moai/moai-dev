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
#ifdef __APPLE__

#include <uslscore/USDeviceTime.h>
#import <mach/mach_time.h>

//================================================================//
// local
//================================================================//

mach_timebase_info_data_t _getTimerInfo ();

//----------------------------------------------------------------//
mach_timebase_info_data_t _getTimerInfo () {
	
	mach_timebase_info_data_t info;
	mach_timebase_info ( &info );
	
	return info;
}

//================================================================//
// USDeviceTime
//================================================================//

	//----------------------------------------------------------------//
	double USDeviceTime::GetTimeInSeconds () {
		
		static mach_timebase_info_data_t sInfo = _getTimerInfo (); // frequency in nanoseconds
	
		if ( sInfo.denom > 0.0 ) {
			
			double time = ( double )mach_absolute_time ();
			return (( time * sInfo.numer ) / sInfo.denom ) * ( 1e-9 );
		}
		return 0.0;
	}

#endif
