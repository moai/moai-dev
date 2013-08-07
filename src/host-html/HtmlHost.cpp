// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <moai_config.h>
#include <lua-headers/moai_lua.h>
#include <host-html/HtmlHost.h>
#include <string.h>

#define UNUSED(p) (( void )p)

#include <moai-sim/host.h>
#include <moai-util/host.h>

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


namespace HtmlInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace HtmlInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}
namespace HtmlMouseButton {
	enum {
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT
	};
}
namespace HtmlMouseButtonState {
	enum {
		MOUSE_DOWN,
		MOUSE_UP
	};
}
static bool sHasWindow = false;
static bool sExitFullscreen = false;
// static bool sDynamicallyReevaluateLuaFiles = false;

static int sWinX;
static int sWinY;
static int sWinWidth;
static int sWinHeight;
static int sModifiers;

//================================================================//
// html callbacks
//================================================================//

//----------------------------------------------------------------//
void onKeyDown ( unsigned char key) {
	AKUEnqueueKeyboardEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD, key, true );
}

//----------------------------------------------------------------//
void onKeyUp ( unsigned char key ) {
	AKUEnqueueKeyboardEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD, key, false );
}


//----------------------------------------------------------------//
void onMouseButton ( int button, int state  ) {
	switch ( button ) {
		case HtmlMouseButton::MOUSE_LEFT:
			AKUEnqueueButtonEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_LEFT, ( state == HtmlMouseButtonState::MOUSE_DOWN));
		break;
	}
}

//----------------------------------------------------------------//
void onMouseDrag ( int x, int y ) {
	AKUEnqueuePointerEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::POINTER, x, y );
}

//----------------------------------------------------------------//
void onMouseMove ( int x, int y ) {
	AKUEnqueuePointerEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::POINTER, x, y );
}

//----------------------------------------------------------------//
void onPaint () {
	AKURender ();
}

//----------------------------------------------------------------//
void onReshape( int w, int h ) {

	if ( sExitFullscreen ) {
	
		w = sWinWidth;
		h = sWinHeight;
		
		sExitFullscreen = false;
	}

	AKUSetScreenSize ( w, h );
	AKUSetViewSize ( w, h );
}

//----------------------------------------------------------------//
void onTimer ( ) {

	double fSimStep = AKUGetSimStep ();
	int timerInterval = ( int )( fSimStep * 1000.0 );
	
	AKUUpdate ();
	
	#if MOAI_HOST_USE_FMOD_EX
		AKUFmodUpdate ();
	#endif
	
	#if MOAI_HOST_USE_FMOD_DESIGNER
		AKUFmodDesignerUpdate (( float )fSimStep );
	#endif
}

//================================================================//
// aku callbacks
//================================================================//

//JS delegates


void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {
      EnterFullScreen();
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {
      ExitFullScreen();
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	OpenWindowFunc(title, width, height);
	AKUDetectGfxContext ();
	AKUSetScreenSize ( width, height );
}

//================================================================//
   //HtmlHost
//================================================================//

//----------------------------------------------------------------//
void Cleanup () {

	
	#if MOAI_WITH_BOX2D
		AKUFinalizeBox2D ();
	#endif
	
	#if MOAI_WITH_CHIPMUNK
		AKUFinalizeChipmunk ();
	#endif
	
	#if MOAI_WITH_HTTP_CLIENT
		AKUFinalizeHttpClient ();
	#endif

	AKUFinalizeUtil ();
	AKUFinalizeSim ();
	AKUFinalize ();
	
}

void Dummy() {
	RestoreFile("dummy",0);
}

void RefreshContext () {

	AKUContextID context = AKUGetContext ();
	if ( context ) {
		AKUDeleteContext ( context );
	}
	AKUCreateContext ();
	
	AKUInitializeUtil ();
	AKUInitializeSim ();
  
	#if MOAI_WITH_BOX2D
		AKUInitializeBox2D ();
	#endif
	
	#if MOAI_WITH_CHIPMUNK
		AKUInitializeChipmunk ();
	#endif

	#if MOAI_WITH_FMOD_EX
		AKUFmodLoad ();
	#endif
	
	#if MOAI_WITH_FMOD_DESIGNER
		AKUFmodDesignerInit ();
	#endif
	
	#if MOAI_WITH_LUAEXT
		AKUExtLoadLuacrypto ();
		AKUExtLoadLuacurl ();
		AKUExtLoadLuafilesystem ();
		AKUExtLoadLuasocket ();
		AKUExtLoadLuasql ();
	#endif
	
	#if MOAI_WITH_HARNESS
		AKUSetFunc_ErrorTraceback ( _debuggerTracebackFunc );
		AKUDebugHarnessInit ();
	#endif
	
	#if MOAI_WITH_HTTP_CLIENT
		AKUInitializeHttpClient ();
	#endif 

	#if MOAI_WITH_PARTICLE_PRESETS
		ParticlePresets ();
	#endif
	
	#if MOAI_WITH_UNTZ
		AKUInitializeUntz ();
	#endif

	AKUSetInputConfigurationName ( "AKUGlut" );

	AKUReserveInputDevices			( HtmlInputDeviceID::TOTAL );
	AKUSetInputDevice				( HtmlInputDeviceID::DEVICE, "device" );
	
	AKUReserveInputDeviceSensors	( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::TOTAL );
	AKUSetInputDeviceKeyboard		( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD,		"keyboard" );
	AKUSetInputDevicePointer		( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::POINTER,		"pointer" );
	AKUSetInputDeviceButton			( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_LEFT,	"mouseLeft" );
	AKUSetInputDeviceButton			( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_MIDDLE,	"mouseMiddle" );
	AKUSetInputDeviceButton			( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_RIGHT,	"mouseRight" );

	AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
	AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
	AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );

	AKURunData ( moai_lua, moai_lua_SIZE,  AKU_DATA_STRING, AKU_DATA_ZIPPED);
}
