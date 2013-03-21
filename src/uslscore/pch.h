#ifndef USLSCORE_PCH_H
#define	USLSCORE_PCH_H

#include <moaicore-config.h>

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

// zl
#include <zlcore/zgl.h>
#include <zlcore/zl_types.h>

#endif
