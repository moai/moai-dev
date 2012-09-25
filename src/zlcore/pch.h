#ifndef ZL_PCH_H
#define	ZL_PCH_H

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

	#ifdef ANDROID
		#define MOAI_PHONE
		#define MOAI_OS_ANDROID
	#else
		#define MOAI_OS_LINUX
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

	#pragma warning ( disable : 4290 )
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

	#include <crtdbg.h>
	#include <direct.h>

	#ifndef PATH_MAX
		#define PATH_MAX 1024
	#endif

#endif

#ifdef MOAI_OS_NACL
	#ifndef PATH_MAX
		#define PATH_MAX 1024
	#endif
#endif

#ifdef MOAI_OS_LINUX
	#include <unistd.h>
#endif

#ifdef MOAI_COMPILER_MSVC
	#define SUPPRESS_EMPTY_FILE_WARNING namespace { char gDummy##__LINE__; }
#else
	#define SUPPRESS_EMPTY_FILE_WARNING
#endif

#define _USE_MATH_DEFINES

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
	#include <cstdio>
	#include <cstdlib>
	#include <string>
 	#include <iostream>
	#include <sstream>
#endif

//----------------------------------------------------------------//

#define UNUSED(p) (( void )p)
#define ALLOCA_MAX 1024

#endif
