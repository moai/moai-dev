//----------------------------------------------------------------//
// Copyright (c) 2010-2017 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef MOAI_SIM_HOST_H
#define MOAI_SIM_HOST_H

#include <moai-core/host.h>
#include <moai-sim/MOAIKeyCodeEnum.h>

enum {
	AKU_ORIENTATION_PORTRAIT,
	AKU_ORIENTATION_LANDSCAPE,
};

enum {
	AKU_DISPLAY_LIST_DRAWING,
	AKU_DISPLAY_LIST_LOADING,
};

enum {
	AKU_DISPLAY_LIST_LOGIC_PHASE,
	AKU_DISPLAY_LIST_LOADING_PHASE,
	AKU_DISPLAY_LIST_DRAWING_PHASE,
};

// Callbacks
typedef void ( *AKUEnterFullscreenModeFunc )	( void* userdata );
typedef void ( *AKUExitFullscreenModeFunc )		( void* userdata );
typedef void ( *AKUHideCursorFunc )				( void* userdata );
typedef void ( *AKUOpenWindowFunc )				( const char* title, int width, int height, void* userdata );
typedef void ( *AKUSetSimStepFunc )				( double step, void* userdata );
typedef void ( *AKUSetTextInputRectFunc )		( int xMin, int yMin, int xMax, int yMax, void* userdata );
typedef void ( *AKUShowCursorFunc )				( void* userdata );

// setup
AKU_API void			AKUSimAppFinalize				();
AKU_API void			AKUSimAppInitialize				();
AKU_API void			AKUSimContextInitialize			( AKUContextID contextID );

// management api
AKU_API double			AKUGetSimStep					( AKUContextID contextID );
AKU_API void			AKUPause						( AKUContextID contextID, bool pause );
AKU_API void			AKURender						( AKUContextID contextID );
AKU_API void			AKUSetOrientation				( AKUContextID contextID, int orientation );
AKU_API void			AKUSetScreenDpi					( AKUContextID contextID, int dpi );
AKU_API void			AKUSetScreenSize				( AKUContextID contextID, int width, int height );
AKU_API void			AKUSetViewSize					( AKUContextID contextID, int width, int height );
AKU_API void			AKUUpdate						( AKUContextID contextID );

// callback management
AKU_API void			AKUSetFunc_EnterFullscreenMode	( AKUContextID contextID, AKUEnterFullscreenModeFunc func, void* userdata );
AKU_API void			AKUSetFunc_ExitFullscreenMode	( AKUContextID contextID, AKUExitFullscreenModeFunc func, void* userdata );
AKU_API void			AKUSetFunc_ShowCursor			( AKUContextID contextID, AKUShowCursorFunc func, void* userdata );
AKU_API void			AKUSetFunc_HideCursor			( AKUContextID contextID, AKUHideCursorFunc func, void* userdata );
AKU_API void			AKUSetFunc_OpenWindow			( AKUContextID contextID, AKUOpenWindowFunc func, void* userdata );
AKU_API void			AKUSetFunc_SetSimStep			( AKUContextID contextID, AKUSetSimStepFunc func, void* userdata );
AKU_API void			AKUSetFunc_SetTextInputRect		( AKUContextID contextID, AKUSetTextInputRectFunc func, void* userdata );

// input device api
AKU_API void			AKUReserveInputDevices			( AKUContextID contextID, int total );
AKU_API void			AKUReserveInputDeviceSensors	( AKUContextID contextID, int deviceID, int total );
AKU_API void			AKUSetInputAutoTimestamp		( AKUContextID contextID, bool autotimestamp ); // optional: causes the input manager to automatically assign timestamps
AKU_API void			AKUSetInputConfigurationName	( AKUContextID contextID, char const* name );
AKU_API void			AKUSetInputDevice				( AKUContextID contextID, int deviceID, char const* name );
AKU_API void			AKUSetInputDeviceHardwareInfo	( AKUContextID contextID, int deviceID, char const* hardwareInfo );
AKU_API void			AKUSetInputDeviceActive			( AKUContextID contextID, int deviceID, bool active );
AKU_API void			AKUSetInputDeviceButton			( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceCompass		( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceKeyboard		( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceJoystick		( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceLevel			( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceLocation		( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDevicePointer		( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceTouch			( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceVector			( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceWheel			( AKUContextID contextID, int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputTimebase				( AKUContextID contextID, double timebase ); // optional: sets timebase event timestamps
AKU_API void			AKUSetInputTimestamp			( AKUContextID contextID, double timestamp ); // optional: sets timestamp for next input event

// input events api
AKU_API void			AKUEnqueueButtonEvent			( AKUContextID contextID, int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueCompassEvent			( AKUContextID contextID, int deviceID, int sensorID, float heading );
AKU_API void			AKUEnqueueJoystickEvent			( AKUContextID contextID, int deviceID, int sensorID, float x, float y );
AKU_API void			AKUEnqueueKeyboardCharEvent		( AKUContextID contextID, int deviceID, int sensorID, int unicodeChar );
AKU_API void			AKUEnqueueKeyboardEditEvent		( AKUContextID contextID, int deviceID, int sensorID, char const* text, int start, int editLength, int maxLength);
AKU_API void			AKUEnqueueKeyboardKeyEvent		( AKUContextID contextID, int deviceID, int sensorID, int keyID, bool down );
AKU_API void			AKUEnqueueKeyboardTextEvent		( AKUContextID contextID, int deviceID, int sensorID, const char* text );
AKU_API void			AKUEnqueueLevelEvent			( AKUContextID contextID, int deviceID, int sensorID, float x, float y, float z );
AKU_API void			AKUEnqueueLocationEvent			( AKUContextID contextID, int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
AKU_API void			AKUEnqueuePointerEvent			( AKUContextID contextID, int deviceID, int sensorID, int x, int y );
AKU_API void			AKUEnqueueTouchEvent			( AKUContextID contextID, int deviceID, int sensorID, int touchID, bool down, float x, float y );
AKU_API void			AKUEnqueueTouchEventCancel		( AKUContextID contextID, int deviceID, int sensorID );
AKU_API void			AKUEnqueueVectorEvent			( AKUContextID contextID, int deviceID, int sensorID, float x, float y, float z );
AKU_API void			AKUEnqueueWheelEvent			( AKUContextID contextID, int deviceID, int sensorID, float value );

#endif
