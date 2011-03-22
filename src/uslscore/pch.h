#ifndef USLSCORE_PCH_H
#define	USLSCORE_PCH_H

#ifdef _WIN32

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

#ifdef __linux
	#include <cstdlib>
	#include <cstring>
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

typedef struct luaL_Reg LuaReg;

//----------------------------------------------------------------//
typedef unsigned int			uint;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned long			u32;
typedef unsigned long long		u64;

typedef signed char				s8;
typedef signed short			s16;
typedef signed long				s32;
typedef signed long long		s64;

//----------------------------------------------------------------//
typedef const char				cc8;

typedef const unsigned char		cu8;
typedef const unsigned short	cu16;
typedef const unsigned long		cu32;

typedef const signed char		cs8;
typedef const signed short		cs16;
typedef const signed long		cs32;

//----------------------------------------------------------------//
typedef float					f32;
typedef double					f64;

//----------------------------------------------------------------//
typedef u32						addr;

#define UNUSED(p) (( void )p)

#ifdef _WIN32
	#define SUPPRESS_EMPTY_FILE_WARNING namespace { char gDummy##__LINE__; }
#else
	#define SUPPRESS_EMPTY_FILE_WARNING
#endif

#endif
