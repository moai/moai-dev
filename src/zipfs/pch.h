#ifndef ZIPFS_PCH_H
#define	ZIPFS_PCH_H

//http://predef.sourceforge.net/preos.html#sec19
#if defined( __APPLE__ ) && defined( __MACH__ )

	#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE
	
		#define MOAI_PHONE
		#define MOAI_OS_IPHONE

		#if TARGET_IPHONE_SIMULATOR
			#define MOAI_OS_IPHONE_SIMULATOR
		#else
			#define MOAI_OS_IPHONE_DEVICE
		#endif

	#else
		#define MOAI_OS_OSX
	#endif

#elif defined( _WIN32 )
	#define MOAI_OS_WINDOWS

#elif defined( __linux )

	#define MOAI_OS_LINUX

	#ifdef ANDROID
		#define MOAI_PHONE
		#define MOAI_OS_ANDROID
	#endif

#else
	#define MOAI_OS_UNKNOWN

#endif

#ifdef _MSC_VER
	#define MOAI_COMPILER_MSVC

#else
	#define MOAI_COMPILER_GCC

#endif

#ifdef MOAI_OS_WINDOWS

	#pragma warning ( disable : 4995 )
	#pragma warning ( disable : 4996 )

	#ifndef _CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
	#endif
	
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif

	#ifndef STRSAFE_NO_DEPRECATE
		#define STRSAFE_NO_DEPRECATE
	#endif

	#include <stdlib.h>
	#include <crtdbg.h>
	#include <ctype.h>
	#include <direct.h>
	#include <string.h>

	#ifndef PATH_MAX
		#define PATH_MAX 1024
	#endif

#endif

#ifdef MOAI_OS_LINUX
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
#endif

#ifdef MOAI_COMPILER_MSVC
	#define SUPPRESS_EMPTY_FILE_WARNING namespace { char gDummy##__LINE__; }
#else
	#define SUPPRESS_EMPTY_FILE_WARNING
#endif

#define _USE_MATH_DEFINES

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <zlib.h>

//----------------------------------------------------------------//

#define UNUSED(p) (( void )p)

#endif
