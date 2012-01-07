/*	Useless Technologies - Version 1.0
	All contents Copyright (c) 2009 by Patrick Meehan
	
	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

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

#ifndef _WIN32

#include <uslscore/USAdapterInfo.h>


//================================================================//
// USAdapterInfo
//================================================================//

//----------------------------------------------------------------//
void USAdapterInfo::SetNameFromMACAddress ( u8* address, u32 length ) {
	UNUSED ( address );
	UNUSED ( length );

	this->mName = "Unimplemented - Do Not Use!";
}

//================================================================//
// USAdapterInfoList
//================================================================//

//----------------------------------------------------------------//
void USAdapterInfoList::EnumerateAdapters () {

	printf("Not implemented");
	assert( false );
}

#endif

