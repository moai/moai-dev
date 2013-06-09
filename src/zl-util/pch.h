#ifndef ZL_UTIL_PCH_H
#define	ZL_UTIL_PCH_H

#include <moai_config.h>

#include <zl-common/zl_platform.h>
#include <zl-common/zl_types.h>

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
#include <zl-vfs/pch.h>
#include <zl-vfs/headers.h>

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

#endif
