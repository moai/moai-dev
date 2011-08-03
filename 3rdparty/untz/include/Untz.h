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

#ifndef WIN32
#ifdef MACOSX
	#include <Carbon/Carbon.h>
#else
	#include <MacTypes.h>
#endif
#else
typedef unsigned int UInt32;
typedef unsigned char UInt8;
#endif

#if defined(_DEBUG)
	#ifdef __GNUC__
		#define RPRINT(fmt...) printf(fmt)
	#else
		#define RPRINT(fmt, ...) printf(fmt, __VA_ARGS__)
	#endif
#else
	#define RPRINT(fmt, ...)
	
#endif

#include "UntzSystem.h"
#include "UntzSound.h"

#endif // UNTZ_H_