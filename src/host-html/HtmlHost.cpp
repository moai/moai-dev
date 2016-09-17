// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <host-html/HtmlHost.h>
#include <string.h>
#include <host-modules/aku_modules.h>
#include <moai-core/headers.h>
#define UNUSED(p) (( void )p)



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
void onKeyDown ( int key) {
	AKUEnqueueKeyboardKeyEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD, key, true );
}

//----------------------------------------------------------------//
void onKeyUp ( int key ) {
	AKUEnqueueKeyboardKeyEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD, key, false );
}

//----------------------------------------------------------------//
void onChar ( int unicodeChar ) {
	AKUEnqueueKeyboardCharEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::KEYBOARD, unicodeChar );
}


//----------------------------------------------------------------//
void onMouseButton ( int button, int state  ) {
	switch ( button ) {
		case HtmlMouseButton::MOUSE_LEFT:
			AKUEnqueueButtonEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_LEFT, ( state == HtmlMouseButtonState::MOUSE_DOWN));
			break;
		case HtmlMouseButton::MOUSE_RIGHT:
			AKUEnqueueButtonEvent ( HtmlInputDeviceID::DEVICE, HtmlInputDeviceSensorID::MOUSE_RIGHT, ( state == HtmlMouseButtonState::MOUSE_DOWN));
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
	
	AKUModulesUpdate ();
	
	
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

	
	AKUModulesAppFinalize();
	AKUAppFinalize ();
	
}

void Dummy() {
	RestoreFile("dummy",0);
}

void RefreshContext () {

	AKUAppInitialize ();
	AKUModulesAppInitialize ();

	AKUCreateContext ();

    AKUModulesContextInitialize ();
	AKUModulesRunLuaAPIWrapper ();
	
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

	//AKUModulesParseArgs ( argc, argv );
}

const char *CallStringFunc(char *func) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	lua_getglobal ( state, "loadstring" );
	if ( !state.IsType ( -1, LUA_TFUNCTION )) {
		ZLLog_ErrorF ( ZLLog::CONSOLE, "Missing global Lua function 'loadstring'\n" );
	}

	state.Push ( func, strlen(func) );

	int status = state.DebugCall ( state.GetLocalTop () - 1, 2 );
	if ( state.LogErrors ( ZLLog::LOG_ERROR, ZLLog::CONSOLE, status )) return NULL;

	if ( state.IsType ( -1, LUA_TSTRING )) {

		ZLLog_ErrorF ( ZLLog::CONSOLE, "Error loading script:\n" );
		ZLLog_ErrorF ( ZLLog::CONSOLE, "%s\n", state.GetValue < cc8* >( -1, "" ));
		return NULL;
		
	}
	state.Pop(); //leaving function at top of stack
	status = state.DebugCall(0, 1);
	if (state.LogErrors(ZLLog::LOG_ERROR, ZLLog::CONSOLE, status)) return NULL;

	cc8* result = state.GetValue<cc8*>(-1,"null");
	//assign to stlstring to get a copy before pop
  char * res = strdup(result);
	state.Pop();

	return res;
}