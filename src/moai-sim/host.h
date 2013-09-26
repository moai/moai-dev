//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_SIM_H
#define AKU_SIM_H

#include <moai-core/host.h>

enum {
	AKU_ORIENTATION_PORTRAIT,
	AKU_ORIENTATION_LANDSCAPE,
};

// Callbacks
typedef void ( *AKUEnterFullscreenModeFunc )	();
typedef void ( *AKUExitFullscreenModeFunc )		();
typedef void ( *AKUOpenWindowFunc )				( const char* title, int width, int height );
typedef void ( *AKUSetSimStepFunc )				( double step );

// setup
AKU_API void			AKUSimAppFinalize				();
AKU_API void			AKUSimAppInitialize				();
AKU_API void			AKUSimContextInitialize			();

// management api
AKU_API void			AKUDetectGfxContext				();
AKU_API double			AKUGetSimStep					();
AKU_API void			AKUPause						( bool pause );
AKU_API void			AKUReleaseGfxContext			();
AKU_API void			AKURender						();
AKU_API void			AKUSetOrientation				( int orientation );
AKU_API void			AKUSetScreenDpi					( int dpi );
AKU_API void			AKUSetScreenSize				( int width, int height );
AKU_API void			AKUSetViewSize					( int width, int height );
AKU_API void			AKUSoftReleaseGfxResources		( int age );
AKU_API void			AKUUpdate						();

// callback management
AKU_API void			AKUSetFunc_EnterFullscreenMode	( AKUEnterFullscreenModeFunc func );
AKU_API void			AKUSetFunc_ExitFullscreenMode	( AKUExitFullscreenModeFunc func );
AKU_API void			AKUSetFunc_OpenWindow			( AKUOpenWindowFunc func );
AKU_API void			AKUSetFunc_SetSimStep			( AKUSetSimStepFunc func );

// input device api
AKU_API void			AKUReserveInputDevices			( int total );
AKU_API void			AKUReserveInputDeviceSensors	( int deviceID, int total );
AKU_API void			AKUSetInputConfigurationName	( char const* name );
AKU_API void			AKUSetInputDevice				( int deviceID, char const* name );
AKU_API void			AKUSetInputDeviceActive			( int deviceID, bool active );
AKU_API void			AKUSetInputDeviceButton			( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceCompass		( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceKeyboard		( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceLevel			( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceLocation		( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDevicePointer		( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceTouch			( int deviceID, int sensorID, char const* name );
AKU_API void			AKUSetInputDeviceWheel			( int deviceID, int sensorID, char const* name );

// input events api
AKU_API void			AKUEnqueueButtonEvent			( int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueCompassEvent			( int deviceID, int sensorID, float heading );
AKU_API void			AKUEnqueueKeyboardAltEvent		( int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueKeyboardControlEvent	( int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueKeyboardEvent			( int deviceID, int sensorID, int keyID, bool down );
AKU_API void			AKUEnqueueKeyboardShiftEvent	( int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueLevelEvent			( int deviceID, int sensorID, float x, float y, float z );
AKU_API void			AKUEnqueueLocationEvent			( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
AKU_API void			AKUEnqueuePointerEvent			( int deviceID, int sensorID, int x, int y );
AKU_API void			AKUEnqueueTouchEvent			( int deviceID, int sensorID, int touchID, bool down, float x, float y );
AKU_API void			AKUEnqueueTouchEventCancel		( int deviceID, int sensorID );
AKU_API void			AKUEnqueueWheelEvent			( int deviceID, int sensorID, float value );

#endif
