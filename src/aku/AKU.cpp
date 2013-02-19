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
	AKU_DEFINE_FUNC_CONTEXT ( ErrorTraceback );
	AKU_DEFINE_FUNC_CONTEXT ( ExitFullscreenMode );
	AKU_DEFINE_FUNC_CONTEXT ( OpenWindow );
	AKU_DEFINE_FUNC_CONTEXT ( SetSimStep );
	
	MOAIGlobals*		mGlobals;
	void*				mUserdata;
};

typedef STLMap < AKUContextID, AKUContext* >::iterator ContextMapIt;
typedef STLMap < AKUContextID, AKUContext* > ContextMap;

static bool			gSysInit = true;
static ContextMap*	gContextMap = 0;
static AKUContextID	gContextIDCounter = 0;
static AKUContextID	gContextID = 0;
static AKUContext*	gContext = 0;

//----------------------------------------------------------------//
static void _EnterFullscreenMode () {}
AKU_DEFINE_FUNC_ACCESSORS ( EnterFullscreenMode, _EnterFullscreenMode )

//----------------------------------------------------------------//
static void _ErrorTraceback ( const char* message, struct lua_State* L, int level ) {
    USLog::Print ( "%s\n", message );
    MOAILuaStateHandle state ( L );
    state.PrintStackTrace ( USLog::CONSOLE, level );
}
AKU_DEFINE_FUNC_ACCESSORS ( ErrorTraceback, _ErrorTraceback );

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
static void _SetSimStep ( double step ) {
	UNUSED ( step );
}
AKU_DEFINE_FUNC_ACCESSORS ( SetSimStep, _SetSimStep )

//----------------------------------------------------------------//
static void _deleteContext ( AKUContext* context ) {
	
	if ( context->mGlobals ) {
		MOAIGlobalsMgr::Delete ( context->mGlobals );
	}
	free ( context );
}

//================================================================//
// AKU
//================================================================//

//----------------------------------------------------------------//
void AKUClearMemPool () {

	ZL_TLSF_POOL* pool = zl_tlsf_get_pool ();
	zl_tlsf_set_pool ( 0 );
	
	if ( pool ) {
		zl_tlsf_destroy_pool ( pool );
	}
}

//----------------------------------------------------------------//
AKUContextID AKUCreateContext () {
	
	if ( gSysInit ) {
		moaicore::SystemInit ();
		gContextMap = new ContextMap;
		atexit ( _cleanup );
		gSysInit = false;
	}

	gContext = ( AKUContext* )calloc ( 1, sizeof ( AKUContext ));
	
	gContextID = ++gContextIDCounter;
	( *gContextMap )[ gContextID ] = gContext;
	
	gContext->mUserdata = 0;
	
	gContext->mGlobals = MOAIGlobalsMgr::Create ();
	moaicore::InitGlobals ( gContext->mGlobals );

	// Always set our default error traceback
	AKUSetFunc_ErrorTraceback ( _ErrorTraceback );

	return gContextIDCounter;
}

//----------------------------------------------------------------//
void AKUDeleteContext ( AKUContextID contextID ) {
	
	AKUSetContext ( contextID );
	if ( !gContext ) return;
	
	_deleteContext ( gContext );
	gContextMap->erase ( contextID );
	
	AKUSetContext ( 0 );
}

//----------------------------------------------------------------//
void AKUDetectGfxContext () {

	MOAIGfxDevice::Get ().DetectContext ();
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
void AKUEnqueueKeyboardAltEvent ( int deviceID, int sensorID, bool down ) {

	MOAIInputMgr::Get ().EnqueueKeyboardEvent (( u8 )deviceID, ( u8 )sensorID, MOAIKeyCodes::ALT, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardControlEvent ( int deviceID, int sensorID, bool down ) {

	MOAIInputMgr::Get ().EnqueueKeyboardEvent (( u8 )deviceID, ( u8 )sensorID, MOAIKeyCodes::CONTROL, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEvent ( int deviceID, int sensorID, int keyID, bool down ) {

	MOAIInputMgr::Get ().EnqueueKeyboardEvent (( u8 )deviceID, ( u8 )sensorID, keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardShiftEvent ( int deviceID, int sensorID, bool down ) {

	MOAIInputMgr::Get ().EnqueueKeyboardEvent (( u8 )deviceID, ( u8 )sensorID, MOAIKeyCodes::SHIFT, down );
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
void AKUEnqueueTouchEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y ) {

	MOAIInputMgr::Get ().EnqueueTouchEvent (( u8 )deviceID, ( u8 )sensorID, ( u32 )touchID, down, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( int deviceID, int sensorID ) {

	MOAIInputMgr::Get ().EnqueueTouchEventCancel (( u8 )deviceID, ( u8 )sensorID );
}

//----------------------------------------------------------------//
void AKUEnqueueWheelEvent ( int deviceID, int sensorID, float value ) {

	MOAIInputMgr::Get ().EnqueueWheelEvent (( u8 )deviceID, ( u8 )sensorID, value );
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
	
	if ( !gSysInit ) {
		moaicore::SystemFinalize ();
		gSysInit = true;
	}
}

//----------------------------------------------------------------//
AKUContextID AKUGetContext () {
	
	return gContextID;
}

//----------------------------------------------------------------//
void* AKUGetUserdata () {
	
	if ( gContext ) {
		return gContext->mUserdata;
	}
	return 0;
}

//----------------------------------------------------------------//
lua_State* AKUGetLuaState () {

	lua_State* lua_state = NULL;
	lua_state = MOAILuaRuntime::Get ().State ();

	return lua_state;
}

//----------------------------------------------------------------//
double AKUGetSimStep () {

	return MOAISim::Get ().GetStep ();
}

//----------------------------------------------------------------//
char* AKUGetWorkingDirectory ( char* buffer, int length ) {

	return zl_getcwd ( buffer, length );
}

//----------------------------------------------------------------//
void AKUInitMemPool ( size_t bytes ) {

	assert ( !zl_tlsf_get_pool ());

	ZL_TLSF_POOL* pool = zl_tlsf_create_pool ( bytes );
	zl_tlsf_set_pool ( pool );
}

//----------------------------------------------------------------//
int AKUMountVirtualDirectory ( char const* virtualPath, char const* archive ) {

	return zl_mount_virtual ( virtualPath, archive );
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
void AKUReleaseGfxContext () {

	MOAIGfxDevice::Get ().ReleaseResources ();
}

//----------------------------------------------------------------//
void AKURender () {

	MOAIRenderMgr::Get ().Render ();
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
void AKURunBytecode ( void* data, size_t size ) {

	if ( size ) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		state.Run ( data, size, 0, 0 );
	}
}

//----------------------------------------------------------------//
void AKURunScript ( const char* filename ) {

	if ( !USFileSys::CheckFileExists ( filename )) return;

	int status;
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	status = luaL_loadfile ( state, filename );
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return;
	
	state.DebugCall ( 0, 0 );
}

//----------------------------------------------------------------//
void AKURunString ( const char* script ) {

	int status;
	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	
	status = luaL_loadstring ( state, script );
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return;
	
	state.DebugCall ( 0, 0 );
}

//----------------------------------------------------------------//
void AKUSetContext ( AKUContextID contextID ) {
	
	if ( gContextID != contextID ) {
		
		gContextID = contextID;
		gContext = gContextMap->value_for_key ( contextID );
		
		if ( gContext ) {
			MOAIGlobalsMgr::Set ( gContext->mGlobals );
		}
		else {
			MOAIGlobalsMgr::Set ( 0 );
		}
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
void AKUSetInputDeviceWheel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor (( u8 )deviceID, ( u8 )sensorID, name, MOAISensor::WHEEL );
}


//----------------------------------------------------------------//
void AKUSetUserdata ( void* userdata ) {

	if ( gContext ) {
		gContext->mUserdata = userdata;
	}
}

//----------------------------------------------------------------//
void AKUSetOrientation ( int orientation ) {

	MOAIGfxDevice::Get ().GetDefaultBuffer ()->SetLandscape ( orientation == AKU_ORIENTATION_LANDSCAPE );
}

//----------------------------------------------------------------//	
void AKUSetScreenDpi ( int dpi ) {

	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_screenDpi, dpi );
}

//----------------------------------------------------------------//
void AKUSetScreenSize ( int width, int height ) {

	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_horizontalResolution, width );
	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_verticalResolution, height );
}

//----------------------------------------------------------------//
void AKUSetViewSize ( int width, int height ) {
	
	MOAIGfxDevice& device = MOAIGfxDevice::Get ();
	
	u32 currentWidth = device.GetWidth ();
	u32 currentHeight = device.GetHeight ();
	
	if (( currentWidth != ( u32 )width ) || ( currentHeight != ( u32 )height )) {
	
		MOAIGfxDevice::Get ().SetBufferSize ( width, height );
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		if ( device.PushListener ( MOAIGfxDevice::EVENT_RESIZE, state )) {
			lua_pushnumber ( state, width );
			lua_pushnumber ( state, height );
			state.DebugCall ( 2, 0 );
		}
	}
}

//----------------------------------------------------------------//
int AKUSetWorkingDirectory ( char const* path ) {

	return zl_chdir ( path );
}

//----------------------------------------------------------------//
void AKUSoftReleaseGfxResources ( int age ) {

	MOAIGfxDevice::Get ().SoftReleaseResources ( age );
}

//----------------------------------------------------------------//
void AKUUpdate () {

	MOAISim::Get ().Update ();
}

//----------------------------------------------------------------//
void AKUSetArgv ( char **argv ) {

	int i;
	int argc = 0;

	lua_State* L = AKUGetLuaState ();

	// count argv
	while ( argv[argc] ) argc++;

	lua_createtable ( L, argc, 0 );
	int newTable = lua_gettop ( L );

	// arg[-1] => host binary (lua, luajit, moai-untz, ...)
	// arg[0]  => first arg (script name as passed to host binary)
	// arg[1]  => next arg/option/script
	// arg[2]  => next arg/option/script
	// ...
	for ( i=0; i < argc; i++ ) {
		lua_pushstring ( L, argv[i] );
		lua_rawseti ( L, newTable, i - 1 );
	}

	// same as lua global 'arg'
	lua_setglobal ( L, "arg" );
}
