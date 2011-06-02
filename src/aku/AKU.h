//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#ifndef AKU_H
#define AKU_H

#ifdef WIN32

	#ifdef AKU_EXPORT
		#define AKU_API __declspec(dllexport)
	#endif

	#ifdef AKU_DLL
		#define AKU_API __declspec(dllimport)
	#else
		#define AKU_API
	#endif

	#define AKU_CALLBACK __stdcall

#else
	#define AKU_API
	#define AKU_CALLBACK
#endif

#define AKU_DECLARE_FUNC_ACCESSORS(funcname) \
	AKU_API AKU##funcname##Func	AKUGetFunc_##funcname (); \
	AKU_API void AKUSetFunc_##funcname ( AKU##funcname##Func func );

// Callback management
typedef void ( *AKUEnterFullscreenModeFunc )	();
typedef void ( *AKUExitFullscreenModeFunc )		();
typedef void ( *AKUHideLoadingScreenFunc )		();
typedef void ( *AKUShowLoadingScreenFunc )		();
typedef void ( *AKUOpenWindowFunc )				( const char* title, int width, int height );
typedef void ( *AKUShowSoftwareKeyboardFunc )	();
typedef void ( *AKUStartGameLoopFunc )			();

AKU_DECLARE_FUNC_ACCESSORS ( EnterFullscreenMode )
AKU_DECLARE_FUNC_ACCESSORS ( ExitFullscreenMode )
AKU_DECLARE_FUNC_ACCESSORS ( HideLoadingScreen )
AKU_DECLARE_FUNC_ACCESSORS ( OpenWindow )
AKU_DECLARE_FUNC_ACCESSORS ( ShowLoadingScreen )
AKU_DECLARE_FUNC_ACCESSORS ( ShowSoftwareKeyboard )
AKU_DECLARE_FUNC_ACCESSORS ( StartGameLoop )

struct lua_State;
typedef int AKUContextID;

// context api
AKU_API AKUContextID	AKUCreateContext	( void* user = 0 );
AKU_API void			AKUDeleteContext	( AKUContextID context );
AKU_API AKUContextID	AKUGetContext		();
AKU_API void*			AKUGetContextUser	();
AKU_API void			AKUFinalize			();
AKU_API void			AKUSetContext		( AKUContextID context );
AKU_API void			AKUSetContextUser	( void* user );


// management api
AKU_API lua_State*		AKUGetLuaState		();
AKU_API void			AKUPause			( bool pause );
AKU_API void			AKURender			();
AKU_API void			AKUResize			( int width, int height );
AKU_API void			AKURunScript		( const char* filename );
AKU_API void			AKUSetKey			( unsigned char key, bool isDown );
AKU_API void			AKUSetMouseButton	( bool left, bool middle, bool right, bool isDown );
AKU_API void			AKUSetMouseLoc		( int x, int y );
AKU_API void			AKUUpdate			();

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

// input events api
AKU_API void			AKUEnqueueButtonEvent			( int deviceID, int sensorID, bool down );
AKU_API void			AKUEnqueueCompassEvent			( int deviceID, int sensorID, float heading );
AKU_API void			AKUEnqueueKeyboardEvent			( int deviceID, int sensorID, int keyID, bool down );
AKU_API void			AKUEnqueueLevelEvent			( int deviceID, int sensorID, float x, float y, float z );
AKU_API void			AKUEnqueueLocationEvent			( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
AKU_API void			AKUEnqueuePointerEvent			( int deviceID, int sensorID, int x, int y );
AKU_API void			AKUEnqueueTouchEvent			( int deviceID, int sensorID, int touchID, bool down, float x, float y, int tapCount );
AKU_API void			AKUEnqueueTouchEventCancel		( int deviceID, int sensorID );

#endif
