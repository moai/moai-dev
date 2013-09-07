#pragma once

#define SLEDGE_NAMESPACE Sledge

#define SLEDGE_HOST_USE_LUAEXT

// ARCHITECTURE ---------------------------------------------------------------
// VC
#if defined(_WIN64)
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif

// GCC
#if defined(__GNUC__)
#if defined(__x86_64__)
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// LIBRARIES ------------------------------------------------------------------
// C standard library
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#ifdef _DEBUG
#include <cassert>
#endif

// STL
#include <string>
#include <vector>
#include <list>

#include <sys/types.h>
#include <sys/stat.h> 
#include <errno.h>

// TinyXML
#include <tinyxml.h>

// MOAI
#if defined(_WIN32) || defined(_WIN64)
#include <kashmir/uuid.h>
#endif
#include <uslscore/pch.h>
#include <uslscore/USAccessors.h>
#include <uslscore/USBox.h>
#include <uslscore/USColor.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USLeanArray.h>
#include <uslscore/USStream.h>
#include <uslscore/USStreamReader.h>
#include <uslscore/USStreamWriter.h>
#include <uslscore/STLList.h>
#include <uslscore/STLSet.h>
#include <uslscore/STLMap.h>
#ifndef HAS_AKU
#define HAS_AKU
#include <aku/AKU.h>
#endif
#define SLEDGE_HOST_USE_LUAEXT
#ifdef SLEDGE_HOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif
#include <aku/AKU-untz.h>
#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
#include <aku/AKU-audiosampler.h>
#endif


// Folder-watcher stuff
#if defined(_WIN32) || defined(_WIN64)
#include "FolderWatcher-win.h"
#elif defined(__APPLE__)
#include <FolderWatcher-mac.h>
#endif

// libSDL
#include <SDL.h>
#define SDL_main main

// OpenGL
#if defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#endif

// OS headers
#if defined(_WIN32) || defined(_WIN64)
//#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#endif

// MACROS ---------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(p) (( void )p)
#endif

// ENUMS
// -----------------------------------------------------------------------
namespace SLEDGE_NAMESPACE {
	const u32 WINDOWTITLE_LENGTH = 255;
	enum SFSEnvVarMap {
		SFS_ENV_appDisplayName,
		SFS_ENV_appID,
		SFS_ENV_appVersion,
		SFS_ENV_buildNumber,
		SFS_ENV_cacheDirectory,
		SFS_ENV_carrierISOCountryCode,
		SFS_ENV_carrierMobileCountryCode,
		SFS_ENV_carrierMobileNetworkCode,
		SFS_ENV_carrierName,
		SFS_ENV_connectionType,
		SFS_ENV_countryCode,
		SFS_ENV_cpuabi,
		SFS_ENV_devBrand,
		SFS_ENV_devName,
		SFS_ENV_devUserName,
		SFS_ENV_devManufacturer,
		SFS_ENV_devModel,
		SFS_ENV_devPlatform,
		SFS_ENV_devProduct,
		SFS_ENV_documentDirectory,
		SFS_ENV_iosRetinaDisplay,
		SFS_ENV_languageCode,
		SFS_ENV_numProcessors,
		SFS_ENV_osBrand,
		SFS_ENV_osVersion,
		SFS_ENV_resourceDirectory,
		SFS_ENV_screenDpi,
		SFS_ENV_verticalResolution,
		SFS_ENV_horizontalResolution,
		SFS_ENV_udid,
		SFS_ENV_openUdid,

		SFS_ENV_MOAIMAX,

		SFS_ENV_screenCount = SFS_ENV_MOAIMAX,
		SFS_ENV_ramAmount,
		SFS_ENV_processorModel,
		SFS_ENV_processorFreq,
		SFS_ENV_desktopRes
	};
	const s32 SFS_ENV_ADDITIONAL_KEY_COUNT = 5;
	enum SFSStatusCodes
	{
		SFS_OK = -1,
		SFS_ERR_NOSCRIPT,

		SFS_MAX
	};
	/*
	enum SFSEventType {
	};
	*/
};

// CONSTANTS, DEFAULTS --------------------------------------------------------
#pragma region constants
const char* const SFSMOAIEnvDefaults[Sledge::SFS_ENV_MOAIMAX] = {
	"Moai Debug",
	"moai-test-debug",
	"UNKNOWN VERSION",
	"UNKNOWN BUILD",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};
const char* const SFSMOAIXMLElementNames[Sledge::SFS_ENV_MOAIMAX] = {
	"app_name",
	"identifier",
	"version",
	"build",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	""
};

namespace SLEDGE_NAMESPACE
{
	const u32 DefaultScriptLocationCount = 5;
	char* const DefaultScriptLocations[] =
	{
		"test/main.lua",
		"main.lua",
		"main.lb",
		"Resources/main.lua",
		"Resources/main.lb"
	};
}
#pragma endregion

// TYPEDEFS, STRUCTS ----------------------------------------------------------
#ifndef f32
typedef float		f32;
#endif
#ifndef f64
typedef double		f64;
#endif

template<typename T>
struct vec2
{
	T x;
	T y;
};