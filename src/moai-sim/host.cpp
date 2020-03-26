// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/host.h>
#include <moai-sim/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUEnqueueButtonEvent ( int deviceID, int sensorID, bool down ) {

	MOAIButtonSensor::EnqueueButtonEvent ( deviceID, sensorID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueCompassEvent ( int deviceID, int sensorID, float heading ) {

	MOAICompassSensor::EnqueueCompassEvent ( deviceID, sensorID, heading );
}

//----------------------------------------------------------------//
void AKUEnqueueJoystickEvent( int deviceID, int sensorID, float x, float y ) {

	MOAIJoystickSensor::EnqueueJoystickEvent ( deviceID, sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardCharEvent ( int deviceID, int sensorID, int unicodeChar ) {

	MOAIKeyboardSensor::EnqueueKeyboardCharEvent ( deviceID, sensorID, unicodeChar );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEditEvent ( int deviceID, int sensorID, char const* text, int start, int editLength, int maxLength) {
	
	MOAIKeyboardSensor::EnqueueKeyboardEditEvent ( deviceID, sensorID, text, ( u32 )start, ( u32 )editLength, ( u32 )maxLength );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardKeyEvent ( int deviceID, int sensorID, int keyID, bool down ) {

	MOAIKeyboardSensor::EnqueueKeyboardKeyEvent ( deviceID, sensorID, keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardTextEvent ( int deviceID, int sensorID, const char* text ) {

	MOAIKeyboardSensor::EnqueueKeyboardTextEvent ( deviceID, sensorID, text );
}

//----------------------------------------------------------------//
void AKUEnqueueLevelEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIMotionSensor::EnqueueLevelEvent ( deviceID, sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueLocationEvent ( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAILocationSensor::EnqueueLocationEvent ( deviceID, sensorID, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
}

//----------------------------------------------------------------//
void AKUEnqueuePointerEvent ( int deviceID, int sensorID, int x, int y ) {

	MOAIPointerSensor::EnqueuePointerEvent ( deviceID, sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y ) {

	MOAITouchSensor::EnqueueTouchEvent ( deviceID, sensorID, ( u32 )touchID, down, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( int deviceID, int sensorID ) {

	MOAITouchSensor::EnqueueTouchEventCancel ( deviceID, sensorID );
}

//----------------------------------------------------------------//
void AKUEnqueueVectorEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIVectorSensor::EnqueueVectorEvent ( deviceID, sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueWheelEvent ( int deviceID, int sensorID, float value ) {

	MOAIWheelSensor::EnqueueWheelEvent ( deviceID, sensorID, value );
}

//----------------------------------------------------------------//
double AKUGetSimStep () {

	return MOAISim::Get ().GetStep ();
}

//----------------------------------------------------------------//
void AKUPause ( bool pause ) {

	if ( pause ) {
		MOAISim::Get().Pause ();
	}
	else {
		MOAISim::Get().Resume ();
	}
}

//----------------------------------------------------------------//
void AKURender () {

	MOAIGfxMgr::Get ().Render ();
}

//----------------------------------------------------------------//
void AKUSetFunc_HideCursor ( AKUHideCursorFunc func, void* userdata ) {

	MOAISim::Get ().SetHideCursorFunc ( MOAISim::HideCursorFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUReserveInputDevices ( int total ) {

	MOAIInputMgr::Get ().ReserveDevices (( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUReserveInputDeviceSensors ( int deviceID, int total ) {

	MOAIInputMgr::Get ().ReserveSensors ( deviceID, ( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUSetFunc_EnterFullscreenMode ( AKUEnterFullscreenModeFunc func, void* userdata ) {

	MOAISim::Get ().SetEnterFullscreenModeFunc ( MOAISim::EnterFullscreenModeFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_ExitFullscreenMode ( AKUExitFullscreenModeFunc func, void* userdata ) {

	MOAISim::Get ().SetExitFullscreenModeFunc ( MOAISim::ExitFullscreenModeFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_OpenWindow ( AKUOpenWindowFunc func, void* userdata ) {

	MOAISim::Get ().SetOpenWindowFunc ( MOAISim::OpenWindowFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_SetSimStep ( AKUSetSimStepFunc func, void* userdata ) {

	MOAISim::Get ().SetSetSimStepFunc ( MOAISim::SetSimStepFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetInputAutoTimestamp ( bool autotimestamp ) {

	MOAIInputMgr::Get ().SetAutotimestamp ( autotimestamp );
}

//----------------------------------------------------------------//
void AKUSetInputConfigurationName ( char const* name ) {

	MOAIInputMgr::Get ().SetConfigurationName ( name );
}

//----------------------------------------------------------------//
void AKUSetInputDevice ( int deviceID, char const* name ) {

	MOAIInputMgr::Get ().SetDevice ( deviceID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceHardwareInfo ( int deviceID, char const* hardwareInfo ) {

	MOAIInputMgr::Get ().SetDeviceHardwareInfo ( deviceID, hardwareInfo );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceActive ( int deviceID, bool active ) {

	MOAIInputMgr::Get ().SetDeviceActive ( deviceID, active );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceButton ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIButtonSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceCompass ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAICompassSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceKeyboard ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIKeyboardSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceJoystick ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIJoystickSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLevel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIMotionSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLocation ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAILocationSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDevicePointer ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIPointerSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceTouch ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAITouchSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceVector ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIVectorSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceWheel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIWheelSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputTimebase ( double timebase ) {

	MOAIInputMgr::Get ().SetTimebase ( timebase );
}

//----------------------------------------------------------------//
void AKUSetInputTimestamp ( double timestamp ) {

	MOAIInputMgr::Get ().SetTimestamp ( timestamp );
}

//----------------------------------------------------------------//
void AKUSetOrientation ( int orientation ) {

	MOAIGfxMgr::Get ().GetDefaultFrameBuffer ()->SetLandscape ( orientation == AKU_ORIENTATION_LANDSCAPE );
}

//----------------------------------------------------------------//	
void AKUSetScreenDpi ( int dpi ) {

	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_screenDpi, dpi );
}

//----------------------------------------------------------------//
void AKUSetScreenSize ( int width, int height) {

	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_horizontalResolution, width );
	MOAIEnvironment::Get ().SetValue ( MOAI_ENV_verticalResolution, height );
}

//----------------------------------------------------------------//
void AKUSetViewSize ( int width, int height ) {
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	
	u32 currentWidth = gfxMgr.GetBufferWidth ();
	u32 currentHeight = gfxMgr.GetBufferHeight ();
	
	if (( currentWidth != ( u32 )width ) || ( currentHeight != ( u32 )height )) {
	
		gfxMgr.GetDefaultFrameBuffer ()->SetBufferSize ( width, height );
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( gfxMgr.PushListener ( MOAIGfxMgr::EVENT_RESIZE, state )) {
			lua_pushnumber ( state, width );
			lua_pushnumber ( state, height );
			state.DebugCall ( 2, 0 );
		}
	}
}

//----------------------------------------------------------------//
void AKUSetFunc_ShowCursor ( AKUShowCursorFunc func, void* userdata ) {

	MOAISim::Get ().SetShowCursorFunc ( MOAISim::ShowCursorFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_SetTextInputRect ( AKUSetTextInputRectFunc func, void* userdata ) {
	
	MOAISim::Get ().SetSetTextInputRectFunc ( MOAISim::SetTextInputRectFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSimAppFinalize () {
}

//----------------------------------------------------------------//
void AKUSimAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSimContextInitialize () {

	MOAIDebugLinesMgr::Affirm ();
	MOAIPartitionResultMgr::Affirm ();
	MOAIInputMgr::Affirm ();
	MOAIDraw::Affirm ();
	
	// this is a stopgap; eventuall MOAISim will be split up.
	// right now it's just a bag of legacy.
	MOAISim::Affirm ();
	MOAISim::RegisterAlias < MOAIUpdateMgr >();
	
	MOAIGeometryWriter::Affirm ();
	MOAIKeyCode::Affirm ();
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIButtonSensor )
	REGISTER_LUA_CLASS ( MOAICameraAnchor2D )
	REGISTER_LUA_CLASS ( MOAICameraFitter2D )
	REGISTER_LUA_CLASS ( MOAICollisionDeck )
	REGISTER_LUA_CLASS ( MOAICollisionProp )
	REGISTER_LUA_CLASS ( MOAICollisionWorld )
	REGISTER_LUA_CLASS ( MOAIColor )
	REGISTER_LUA_CLASS ( MOAICompassSensor )
	REGISTER_LUA_CLASS ( MOAICoroutine )
	REGISTER_LUA_CLASS ( MOAIDebugLinesMgr )
	REGISTER_LUA_CLASS ( MOAIDeckRemapper )
	REGISTER_LUA_CLASS ( MOAIDrawDeck )
	REGISTER_LUA_CLASS ( MOAIFancyGrid )
	REGISTER_LUA_CLASS ( MOAIGeometryWriter )
	REGISTER_LUA_CLASS ( MOAIGraphicsProp )
	REGISTER_LUA_CLASS ( MOAIGraphicsGridProp )
	REGISTER_LUA_CLASS ( MOAIGrid )
	REGISTER_LUA_CLASS ( MOAIGridSpace )
	REGISTER_LUA_CLASS ( MOAIGridPathGraph )
	REGISTER_LUA_CLASS ( MOAIImage )
	REGISTER_LUA_CLASS ( MOAIInputDevice )
	REGISTER_LUA_CLASS ( MOAIInputMgr )
	REGISTER_LUA_CLASS ( MOAIJoystickSensor )
	REGISTER_LUA_CLASS ( MOAIKeyCode )
	REGISTER_LUA_CLASS ( MOAIKeyboardSensor )
	REGISTER_LUA_CLASS ( MOAILocationSensor )
	REGISTER_LUA_CLASS ( MOAIMesh )
	REGISTER_LUA_CLASS ( MOAIMetaTileDeck2D )
	REGISTER_LUA_CLASS ( MOAIMotionSensor )
	REGISTER_LUA_CLASS ( MOAIParticleCallbackPlugin )
	REGISTER_LUA_CLASS ( MOAIParticleDistanceEmitter )
	REGISTER_LUA_CLASS ( MOAIParticleForce )
	REGISTER_LUA_CLASS ( MOAIParticleScript )
	REGISTER_LUA_CLASS ( MOAIParticleState )
	REGISTER_LUA_CLASS ( MOAIParticleSystem )
	REGISTER_LUA_CLASS ( MOAIParticleTimedEmitter )
	REGISTER_LUA_CLASS ( MOAIPartition )
	REGISTER_LUA_CLASS ( MOAIPartitionViewLayer )
	REGISTER_LUA_CLASS ( MOAIPath )
	REGISTER_LUA_CLASS ( MOAIPathFinder )
	REGISTER_LUA_CLASS ( MOAIPathStepper )
	REGISTER_LUA_CLASS ( MOAIPathTerrainDeck )
	REGISTER_LUA_CLASS ( MOAIPinTransform )
	REGISTER_LUA_CLASS ( MOAIPointerSensor )
	REGISTER_LUA_CLASS ( MOAIProjectionProp )
	REGISTER_LUA_CLASS ( MOAIRegion )
	REGISTER_LUA_CLASS ( MOAIScissorRect )
	REGISTER_LUA_CLASS ( MOAIScriptNode )
	REGISTER_LUA_CLASS ( MOAISelectionMesh )
	REGISTER_LUA_CLASS ( MOAISim )
	REGISTER_LUA_CLASS ( MOAISpriteDeck2D )
	REGISTER_LUA_CLASS ( MOAIStretchPatch2D )
	REGISTER_LUA_CLASS ( MOAITableLayer )
	REGISTER_LUA_CLASS ( MOAITableViewLayer )
	REGISTER_LUA_CLASS ( MOAITileDeck2D )
	REGISTER_LUA_CLASS ( MOAITouchSensor )
	REGISTER_LUA_CLASS ( MOAIVecPathGraph )
	REGISTER_LUA_CLASS ( MOAIVectorTesselator )
	REGISTER_LUA_CLASS ( MOAIWheelSensor )
	
	// text stuff here for now
	REGISTER_LUA_CLASS ( MOAIBitmapFontReader )
	REGISTER_LUA_CLASS ( MOAIDynamicGlyphCache )
	REGISTER_LUA_CLASS ( MOAIFont )
	REGISTER_LUA_CLASS ( MOAIStaticGlyphCache )
	REGISTER_LUA_CLASS ( MOAITextBundle )
	REGISTER_LUA_CLASS ( MOAITextLabel )
	REGISTER_LUA_CLASS ( MOAITextStyle )
	
	#if MOAI_WITH_TINYXML
		REGISTER_LUA_CLASS ( MOAIParticlePexPlugin )
	#endif
	
	#if MOAI_WITH_FREETYPE
		REGISTER_LUA_CLASS ( MOAIFreeTypeFontReader )
	#endif
}

//----------------------------------------------------------------//
void AKUUpdate () {

	MOAISim::Get ().Update ();
}
