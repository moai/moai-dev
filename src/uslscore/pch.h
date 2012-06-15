#ifndef USLSCORE_PCH_H
#define	USLSCORE_PCH_H

#ifndef TIXML_USE_STL
	#define TIXML_USE_STL
#endif

// lua
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

// vfs
#include <zlcore/pch.h>
#include <zlcore/zlcore.h>

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

#endif
