#ifndef ZL_PLATFORM_H
#define	ZL_PLATFORM_H

#if !(( defined ( DEBUG ) && defined ( _DEBUG )) || defined ( NDEBUG ))
	#error DEBUG and _DEBUG or NDEBUG *must* be defined!
#endif

//http://predef.sourceforge.net/preos.html#sec19
#if defined( __APPLE__ ) && defined( __MACH__ )

	#include "TargetConditionals.h"

	#if TARGET_OS_IPHONE

		#define MOAI_OS_IPHONE

//		#if TARGET_IPHONE_SIMULATOR
//			#define MOAI_OS_IPHONE_SIMULATOR
//		#else
//			#define MOAI_OS_IPHONE_DEVICE
//		#endif

	#else
		#define MOAI_OS_OSX
	#endif

#elif defined( _WIN32 )
	#define MOAI_OS_WINDOWS

#elif defined( EMSCRIPTEN )
	#define MOAI_OS_HTML

#elif defined( __linux )
	#ifdef ANDROID
		#define MOAI_OS_ANDROID
	#else
		#define MOAI_OS_LINUX
	#endif
#else
	#define MOAI_OS_UNKNOWN
#endif

#ifdef __QNX__
  #define MOAI_PHONE
  #define MOAI_OS_BLACKBERRY
#endif

#ifdef _MSC_VER
	#define MOAI_COMPILER_MSVC
  //  #ifndef va_copy
  //  	#define va_copy(d,s) ((d) = (s))
	//  #endif
	#define __func__ __FUNCTION__
#else
	#define MOAI_COMPILER_GCC

#endif


#ifdef MOAI_OS_WINDOWS

	#ifdef MOAI_COMPILER_MSVC
		#pragma warning ( disable : 4250 )
		#pragma warning ( disable : 4290 )
		#pragma warning ( disable : 4995 )
		#pragma warning ( disable : 4996 )
	#endif

	#ifndef _CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
	#endif
	
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif

	#ifndef STRSAFE_NO_DEPRECATE
		#define STRSAFE_NO_DEPRECATE
	#endif

	#ifdef MOAI_COMPILER_GCC
		#include <malloc.h>
	#endif
	
	#ifdef MOAI_COMPILER_MSVC
		#include <crtdbg.h>
    #endif
	
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

#if defined ( MOAI_OS_LINUX ) || defined ( MOAI_OS_OSX )
	#define MOAI_HAS_UNISTD
	#include <unistd.h>
#endif

#ifdef _ARM_ARCH_7
	#define MOAI_ARM7
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
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>

#ifdef __cplusplus
	#include <algorithm>
	#include <cstdio>
	#include <cstdlib>
	#include <map>
	#include <set>
	#include <string>
 	#include <iostream>
	#include <sstream>
	#include <vector>
#endif

//----------------------------------------------------------------//

#define UNUSED(p) (( void )p)
#define ALLOCA_MAX 1024

#endif
