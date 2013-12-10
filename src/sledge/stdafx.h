#pragma once

#define SLEDGE_NAMESPACE Sledge

#define SLEDGE_HOST_USE_LUAEXT

typedef unsigned int			uint;
typedef unsigned long			uintptr;
typedef long				    sintptr;

typedef const char				cc8;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;
typedef unsigned long long		u64;

typedef signed char				s8;
typedef signed short			s16;
typedef signed int				s32;
typedef signed long long			s64;


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
//#include <cstdint>
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
//#include <tinyxml.h>

// MOAI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <moai_config.h>
#include <lua.h>
#include <lua-headers/moai_lua.h>
#include <string.h>

#if LUA_VERSION_NUM >= 502
#ifdef MOAI_WITH_LUAEXT
#undef MOAI_WITH_LUAEXT
#define MOAI_WITH_LUAEXT 0
#endif
#endif

#define UNUSED(p) (( void )p)

#include <moai-http-client/host.h>
#include <moai-sim/host.h>
#include <moai-util/host.h>
#include <zl-util/headers.h>
#include <lauxlib.h>


#if MOAI_WITH_BOX2D
#include <moai-box2d/host.h>
#endif

#if MOAI_WITH_CHIPMUNK
#include <moai-chipmunk/host.h>
#endif

#if MOAI_WITH_FMOD_DESIGNER
#include <moai-fmod-designer/host.h>
#endif

#if MOAI_WITH_FMOD_EX
#include <moai-fmod-ex/host.h>
#endif

#if MOAI_WITH_HARNESS
#include <moai-harness/host.h>
#endif

#if MOAI_WITH_HTTP_CLIENT
#include <moai-http-client/host.h>
#endif

#if MOAI_WITH_LUAEXT
#include <moai-luaext/host.h>
#endif

#if MOAI_WITH_PARTICLE_PRESETS
#include <ParticlePresets.h>
#endif

#if MOAI_WITH_UNTZ
#include <moai-untz/host.h>
#endif

#ifdef _WIN32

#include <glut.h> // wtf

#if MOAI_WITH_FOLDER_WATCHER
#include <FolderWatcher-win.h>
#endif
#else
#ifdef MOAI_OS_LINUX
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#endif

#ifdef __APPLE__

#include <OpenGL/OpenGL.h>

#if MOAI_WITH_FOLDER_WATCHER
#include <FolderWatcher-mac.h>
#endif
#endif


#if defined(_WIN32) || defined(_WIN64)
#include <kashmir/uuid.h>
#endif
/*#include <uslscore/pch.h>
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
#include <uslscore/STLMap.h>*/
#define SLEDGE_HOST_USE_LUAEXT
#ifdef SLEDGE_HOST_USE_LUAEXT
//#include <aku/AKU-luaext.h>
#endif
//#include <aku/AKU-untz.h>
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
#if defined(_WIN32) || defined(_WIN64)
#include <SDL.h>
#elif defined (__APPLE__)
#include <SDL2/SDL.h>
#endif
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
	enum InputDeviceType_ID {
		IDT_DEVICE,
		IDT_PAD,
		IDT_JOY
	};
	enum InputDevice_ID {
		ID_DEVICE,
		ID_PAD_0,
		ID_PAD_1,
		ID_PAD_2,
		ID_PAD_3,
		ID_JOY_0,
		ID_JOY_1,
		ID_JOY_2,
		ID_JOY_3,
		ID_TOTAL
	};

	// "Device" sensors, i.e. keyboard and mouse.
	enum DeviceSensor_ID {
		IDS_KEYBOARD,
		IDS_POINTER,
		IDS_MOUSE_LEFT,
		IDS_MOUSE_MIDDLE,
		IDS_MOUSE_RIGHT,

		IDS_TOTAL
	};
	enum PadSensor_ID {
		PS_STICK_LEFT,
		PS_STICK_RIGHT,
		PS_TRIGGERS,
		PS_BUTTONS,
		PS_TOTAL
	};
	enum JoySensor_ID {
		JS_STICK,
		JS_BUTTONS,
		JS_TOTAL
	};
	enum PadSensorButton_ID {
		PSB_BUTTON_A,
		PSB_BUTTON_B,
		PSB_BUTTON_X,
		PSB_BUTTON_Y,
		PSB_BUTTON_START,
		PSB_BUTTON_BACK,

		PSB_BUTTON_LEFTSTICK,
		PSB_BUTTON_RIGHTSTICK,

		PSB_BUTTON_LEFTSHOULDER,
		PSB_BUTTON_RIGHTSHOULDER,

		PSB_BUTTON_DPAD_UP,
		PSB_BUTTON_DPAD_DOWN,
		PSB_BUTTON_DPAD_LEFT,
		PSB_BUTTON_DPAD_RIGHT
	};
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

	const u32 DEADZONE_THUMB_LEFT = 7849;
	const u32 DEADZONE_THUMB_RIGHT = 8689;
	const u32 DEADZONE_JOY = 8269;
	const u32 THRESHOLD_TRIGGER = 3855;
	const u32 MAX_AXIS = 32767;

	char* const INPUTCONFIGNAME = "AKUSDL2";
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

struct ScreenEnvInfo
{
	s32		screenDim[2];
	s32		screenCount;
	bool	retina;
};

struct buttonState
{
	bool state[SDL_CONTROLLER_BUTTON_MAX];
};

struct pingpongState
{
	buttonState pp[2];
};

struct keybState
{
	bool state[SDL_NUM_SCANCODES];
};

struct pingpongState_keyb
{
	keybState pp[2];
};

struct NormalizedController
{
	vec2<f32> stick_left;
	vec2<f32> stick_right;
	vec2<f32> triggers;
	//buttonState lastButtonState;
};

struct NormalizedJoystick
{
	std::vector<vec2 <f32> > sticks;
	std::vector<bool> buttons;
};

struct SledgeDevice
{
	SLEDGE_NAMESPACE::InputDevice_ID device_id;
	char*				name;
};

struct SledgeController
{
	SLEDGE_NAMESPACE::InputDevice_ID device_id;

	SDL_GameController* controller;
	int					index;
	int					index_controller;
	char*				name;

	bool				connected;

	vec2<f32>				stick_left;
	vec2<f32>				stick_right;
	vec2<f32>				triggers;
	std::vector<bool>	buttons;
};

struct SledgeJoystick
{
	SLEDGE_NAMESPACE::InputDevice_ID device_id;

	SDL_Joystick*		joystick;
	int					index;
	int					index_joystick;
	char*				name;

	bool				connected;

	std::vector<vec2 <f32> > 	sticks;
	std::vector<bool>	buttons;
};


#ifndef SDLINPUTTYPES
#define SDLINPUTTYPES

namespace SLEDGE_NAMESPACE {
	namespace InputDevice {
		const static char* DeviceName[] = {
			"device",
			"pad0",
			"pad1",
			"pad2",
			"pad3",
			"joy0",
			"joy1",
			"joy2",
			"joy3"
		};
		const static InputDeviceType_ID DeviceType[] = {		
			IDT_DEVICE,
			IDT_PAD,
			IDT_PAD,
			IDT_PAD,
			IDT_PAD,
			IDT_JOY,
			IDT_JOY,
			IDT_JOY,
			IDT_JOY
		};
	}
	namespace DeviceSensor {
		const static char* SensorName[] = {
			"keyboard",
			"pointer",
			"mouseLeft",
			"mouseMiddle",
			"mouseRight"
		};
	}
	namespace PadSensorAxes
	{
		const static char* SensorName[] = {
			"stickLeft",
			"stickRight",
			"triggers",
			"buttons"
		};
	}
	namespace JoySensor
	{
		const static char* SensorName[] = {
			"stick",
			"buttons"
		};
	}
	namespace PadSensor
	{
		const static char* ButtonName[] = {
			"btnA",
			"btnB",
			"btnX",
			"btnY",
			"start",
			"back",
			"stickLeft",
			"stickRight",
			"shoulderLeft",
			"shoulderRight",
			"dpadUp",
			"dpadDown",
			"dpadLeft",
			"dpadRight"
		};
	}
}



#endif
