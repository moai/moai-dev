#ifndef UNTZ_H_
#define UNTZ_H_

#define NUM_CHANNELS 2

#include <stdio.h>
#include <string>
#include <vector>

typedef std::string RString;
typedef std::vector<RString> RStringArray;
typedef long long Int64;
typedef short Int16;
typedef unsigned long ULong;

#if defined(__APPLE__)
    #if defined(MACOSX)
        #include <Carbon/Carbon.h>
    #else
        #include <MacTypes.h>
    #endif
#else
    typedef unsigned int UInt32;
    typedef signed short SInt16;
    typedef unsigned short UInt16;
    typedef unsigned char UInt8;
#endif


#if defined(_DEBUG)
	#if defined(__ANDROID__)
		#define RPRINT(fmt, ...) printf(fmt)
	#else
		#define RPRINT(fmt, ...) printf(fmt, __VA_ARGS__)
	#endif
#else
	#define RPRINT(fmt, ...)
	
#endif

#include "UntzSystem.h"
#include "UntzSound.h"

#endif // UNTZ_H_