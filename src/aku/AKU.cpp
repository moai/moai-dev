// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <aku/AKU.h>

#include <cassert>
#include <moaicore/moaicore.h>

#define AKU_DEFINE_FUNC_CONTEXT(funcname) \
AKU##funcname##Func m##funcname;

#define AKU_DEFINE_FUNC_ACCESSORS(funcname,fallback) \
AKU##funcname##Func AKUGetFunc_##funcname () { if ( gContext ) return gContext->m##funcname; else return fallback;  } \
void AKUSetFunc_##funcname ( AKU##funcname##Func func ) { if ( gContext ) gContext->m##funcname = func; }

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
static void _cleanup () {

	AKUFinalize ();
}

//================================================================//
// AKUContext
//================================================================//
struct AKUContext {
	
	//----------------------------------------------------------------//
	AKU_DEFINE_FUNC_CONTEXT ( EnterFullscreenMode );
	AKU_DEFINE_FUNC_CONTEXT ( ExitFullscreenMode );
	AKU_DEFINE_FUNC_CONTEXT ( OpenWindow );
	AKU_DEFINE_FUNC_CONTEXT ( StartGameLoop );
	
	USGlobals*	mGlobals;
	void*		mUser;
};

typedef STLMap < AKUContextID, AKUContext* >::iterator ContextMapIt;
typedef STLMap < AKUContextID, AKUContext* > ContextMap;

static ContextMap*	gContextMap = 0;
static AKUContextID	gContextIDCounter = 0;
static AKUContextID	gContextID = 0;
static AKUContext*	gContext = 0;

//----------------------------------------------------------------//
static void _EnterFullscreenMode () {}
AKU_DEFINE_FUNC_ACCESSORS ( EnterFullscreenMode, _EnterFullscreenMode )

//----------------------------------------------------------------//
static void _ExitFullscreenMode () {}
AKU_DEFINE_FUNC_ACCESSORS ( ExitFullscreenMode, _ExitFullscreenMode )

//----------------------------------------------------------------//
static void _OpenWindow ( const char* title, int width, int height ) {
	UNUSED ( title );
	UNUSED ( width );
	UNUSED ( height );
}
AKU_DEFINE_FUNC_ACCESSORS ( OpenWindow, _OpenWindow )

//----------------------------------------------------------------//
static void _StartGameLoop () {}
AKU_DEFINE_FUNC_ACCESSORS ( StartGameLoop, _StartGameLoop )

//----------------------------------------------------------------//
static void _deleteContext ( AKUContext* context ) {
	
	if ( context->mGlobals ) {
		USGlobals::Delete ( context->mGlobals );
	}
	free ( context );
}

//================================================================//
// AKU
//================================================================//

//----------------------------------------------------------------//
AKUContextID AKUCreateContext ( void* user ) {
	
	static bool sysInit = true;
	if ( sysInit ) {
		gContextMap = new ContextMap;
	}

	gContext = ( AKUContext* )calloc ( 1, sizeof ( AKUContext ));
	
	gContextID = ++gContextIDCounter;
	( *gContextMap )[ gContextID ] = gContext;
	
	gContext->mUser = user;
	
	gContext->mGlobals = USGlobals::Create ();
	moaicore::InitGlobals ( gContext->mGlobals );

	if ( sysInit ) {
		atexit ( _cleanup );
		sysInit = false;
	}
	return gContextIDCounter;
}

//----------------------------------------------------------------//
void AKUDeleteContext ( AKUContextID contextID ) {
	
	AKUContext* context = gContextMap->value_for_key ( contextID );
	if ( !context ) return;
	
	_deleteContext ( context );
	gContextMap->erase ( contextID );
	
	AKUSetContext ( 0 );
}

//----------------------------------------------------------------//
void AKUEnqueueButtonEvent ( int deviceID, int sensorID, bool down ) {

	MOAIInputMgr::Get ().EnqueueButtonEvent (( u8 )deviceID, ( u8 )sensorID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueCompassEvent ( int deviceID, int sensorID, float heading ) {

	MOAIInputMgr::Get ().EnqueueCompassEvent (( u8 )deviceID, ( u8 )sensorID, heading );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEvent ( int deviceID, int sensorID, int keyID, bool down ) {

	MOAIInputMgr::Get ().EnqueueKeyboardEvent (( u8 )deviceID, ( u8 )sensorID, keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueLevelEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIInputMgr::Get ().EnqueueLevelEvent (( u8 )deviceID, ( u8 )sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueLocationEvent ( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAIInputMgr::Get ().EnqueueLocationEvent (( u8 )deviceID, ( u8 )sensorID, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
}

//----------------------------------------------------------------//
void AKUEnqueuePointerEvent ( int deviceID, int sensorID, int x, int y ) {

	MOAIInputMgr::Get ().EnqueuePointerEvent (( u8 )deviceID, ( u8 )sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y, int tapCount ) {

	MOAIInputMgr::Get ().EnqueueTouchEvent (( u8 )deviceID, ( u8 )sensorID, ( u32 )touchID, down, x, y, ( u32 )tapCount );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( int deviceID, int sensorID ) {

	MOAIInputMgr::Get ().EnqueueTouchEventCancel (( u8 )deviceID, ( u8 )sensorID );
}

//----------------------------------------------------------------//
void AKUFinalize () {

	if ( gContextMap ) {

		ContextMapIt contextMapIt = gContextMap->begin ();
		for ( ; contextMapIt != gContextMap->end (); ++contextMapIt ) {
			AKUContext* context = contextMapIt->second;
			_deleteContext ( context );
		}
		
		delete gContextMap;
		gContextMap = 0;
	}
}

//----------------------------------------------------------------//
AKUContextID AKUGetContext () {
	
	return gContextID;
}

//----------------------------------------------------------------//
void* AKUGetContextUser () {
	
	if ( gContext ) {
		return gContext->mUser;
	}
	return 0;
}

//----------------------------------------------------------------//
lua_State* AKUGetLuaState () {

	lua_State* lua_state = NULL;
	lua_state = USLuaRuntime::Get ().State ();

	return lua_state;
}

//----------------------------------------------------------------//
void AKUPause ( bool pause ) {

	if ( pause ) {
		MOAISim::Get().PauseMOAI ();
	}
	else {
		MOAISim::Get().ResumeMOAI ();
	}
}

//----------------------------------------------------------------//
void AKURender () {

	MOAISim::Get ().Render ();
}

//----------------------------------------------------------------//
void AKUReserveInputDevices ( int total ) {

	MOAIInputMgr::Get ().ReserveDevices (( u8 )total );
}

//----------------------------------------------------------------//
void AKUReserveInputDeviceSensors ( int deviceID, int total ) {

	MOAIInputMgr::Get ().ReserveSensors (( u8 )deviceID, ( u8 )total );
}

//----------------------------------------------------------------//
void AKUResize ( int width, int height ) {

	USGfxDevice::Get ().SetSize ( width, height );
}

//----------------------------------------------------------------//
void AKURunScript ( const char* filename ) {

	MOAISim::Get ().RunFile ( filename );
}

//----------------------------------------------------------------//
void AKUSetContext ( AKUContextID contextID ) {
	
	if ( gContextID != contextID ) {
		
		gContextID = contextID;
		gContext = gContextMap->value_for_key ( contextID );
		
		if ( gContext ) {
			USGlobals::Set ( gContext->mGlobals );
		}
		else {
			USGlobals::Set ( 0 );
		}
	}
}

//----------------------------------------------------------------//
void AKUSetContextUser ( void* user ) {

	if ( gContext ) {
		gContext->mUser = user;
	}
}

//----------------------------------------------------------------//
void AKUSetInputConfigurationName ( char const* name ) {

	MOAIInputMgr::Get ().SetConfigurationName ( name );
}

//----------------------------------------------------------------//
void AKUSetInputDevice ( int deviceID, char const* name ) {

	MOAIInputMgr::Get ().SetDevice (( u8 )deviceID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceActive ( int deviceID, bool active ) {

	MOAIInputMgr::Get ().SetDeviceActive (( u8 )deviceID, active );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceButton ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::BUTTON );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceCompass ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::COMPASS );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceKeyboard ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::KEYBOARD );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLevel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::LEVEL );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLocation ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::LOCATION );
}

//----------------------------------------------------------------//
void AKUSetInputDevicePointer ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::POINTER );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceTouch ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::TOUCH );
}

//----------------------------------------------------------------//
void AKUUpdate () {

	MOAISim::Get ().Update ();
}

