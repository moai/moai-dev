#ifndef USLSCORE_PCH_H
#define	USLSCORE_PCH_H

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
	#include <io.h>
	#include <direct.h>

	#ifndef PATH_MAX
		#define PATH_MAX 1024
	#endif

#endif

#ifdef MOAI_OS_LINUX
	#include <cstdlib>
	#include <cstring>
#endif

#ifdef MOAI_COMPILER_MSVC
	#define SUPPRESS_EMPTY_FILE_WARNING namespace { char gDummy##__LINE__; }
#else
	#define SUPPRESS_EMPTY_FILE_WARNING
#endif

#include <assert.h>

// stl
#include <memory>
#include <limits>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

using namespace std;

// stream
#include <fstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// c std
#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// lua
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

//----------------------------------------------------------------//
typedef unsigned int			uint;
typedef unsigned long			uintptr;
typedef long					sintptr;

typedef const char				cc8;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned long long		u64;

typedef signed char				s8;
typedef signed short			s16;
typedef signed int				s32;
typedef signed long long		s64;

//----------------------------------------------------------------//

#define UNUSED(p) (( void )p)

#endif
