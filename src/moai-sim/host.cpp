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

	MOAIButtonSensor::EnqueueButtonEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), down );
}

//----------------------------------------------------------------//
void AKUEnqueueCompassEvent ( int deviceID, int sensorID, float heading ) {

	MOAICompassSensor::EnqueueCompassEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), heading );
}

//----------------------------------------------------------------//
void AKUEnqueueJoystickEvent( int deviceID, int sensorID, float x, float y ) {

	MOAIJoystickSensor::EnqueueJoystickEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardCharEvent ( int deviceID, int sensorID, int unicodeChar ) {

	MOAIKeyboardSensor::EnqueueKeyboardCharEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), unicodeChar );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEditEvent ( int deviceID, int sensorID, char const* text, int start, int editLength, int maxLength) {
	
	MOAIKeyboardSensor::EnqueueKeyboardEditEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), text, ( u32 )start, ( u32 )editLength, ( u32 )maxLength );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardKeyEvent ( int deviceID, int sensorID, int keyID, bool down ) {

	MOAIKeyboardSensor::EnqueueKeyboardKeyEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardTextEvent ( int deviceID, int sensorID, const char* text ) {

	MOAIKeyboardSensor::EnqueueKeyboardTextEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), text );
}

//----------------------------------------------------------------//
void AKUEnqueueLevelEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIMotionSensor::EnqueueLevelEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueLocationEvent ( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAILocationSensor::EnqueueLocationEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
}

//----------------------------------------------------------------//
void AKUEnqueuePointerEvent ( int deviceID, int sensorID, int x, int y ) {

	MOAIPointerSensor::EnqueuePointerEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y ) {

	MOAITouchSensor::EnqueueTouchEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), ( u32 )touchID, down, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( int deviceID, int sensorID ) {

	MOAITouchSensor::EnqueueTouchEventCancel ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ) );
}

//----------------------------------------------------------------//
void AKUEnqueueVectorEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIVectorSensor::EnqueueVectorEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueWheelEvent ( int deviceID, int sensorID, float value ) {

	MOAIWheelSensor::EnqueueWheelEvent ( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), value );
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
void AKUSetFunc_HideCursor ( AKUHideCursorFunc func ) {

	MOAISim::Get ().SetHideCursorFunc ( func );
}

//----------------------------------------------------------------//
void AKUReserveInputDevices ( int total ) {

	MOAIInputMgr::Get ().ReserveDevices (( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUReserveInputDeviceSensors ( int deviceID, int total ) {

	MOAIInputMgr::Get ().ReserveSensors ( ZLIndexCast ( deviceID ), ( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUSetFunc_EnterFullscreenMode ( AKUEnterFullscreenModeFunc func ) {

	MOAISim::Get ().SetEnterFullscreenModeFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetFunc_ExitFullscreenMode ( AKUExitFullscreenModeFunc func ) {

	MOAISim::Get ().SetExitFullscreenModeFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetFunc_OpenWindow ( AKUOpenWindowFunc func ) {

	MOAISim::Get ().SetOpenWindowFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetFunc_SetSimStep ( AKUSetSimStepFunc func ) {

	MOAISim::Get ().SetSetSimStepFunc ( func );
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

	MOAIInputMgr::Get ().SetDevice ( ZLIndexCast ( deviceID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceHardwareInfo ( int deviceID, char const* hardwareInfo ) {

	MOAIInputMgr::Get ().SetDeviceHardwareInfo ( ZLIndexCast ( deviceID ), hardwareInfo );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceActive ( int deviceID, bool active ) {

	MOAIInputMgr::Get ().SetDeviceActive ( ZLIndexCast ( deviceID ), active );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceButton ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIButtonSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceCompass ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAICompassSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceKeyboard ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIKeyboardSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceJoystick ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIJoystickSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLevel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIMotionSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLocation ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAILocationSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDevicePointer ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIPointerSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceTouch ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAITouchSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceVector ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIVectorSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceWheel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIWheelSensor >( ZLIndexCast ( deviceID ), ZLIndexCast ( sensorID ), name );
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
void AKUSetFunc_ShowCursor ( AKUShowCursorFunc func ) {

	MOAISim::Get ().SetShowCursorFunc ( func );
}

//----------------------------------------------------------------//
void AKUSetFunc_SetTextInputRect ( AKUSetTextInputRectFunc func ) {
	
	MOAISim::Get ().SetSetTextInputRectFunc ( func );
}

//----------------------------------------------------------------//
void AKUSimAppFinalize () {
}

//----------------------------------------------------------------//
void AKUSimAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSimContextInitialize () {

	MOAINodeMgr::Affirm ();
	MOAIActionStackMgr::Affirm ();

	MOAIMaterialMgr::Affirm ();
	
	MOAIDraw::Affirm ();
	MOAIDebugLinesMgr::Affirm ();
	MOAIPartitionResultMgr::Affirm ();
	MOAIInputMgr::Affirm ();
	MOAISim::Affirm ();
	
	MOAIEaseType::Affirm ();
	MOAIGeometryWriter::Affirm ();
	MOAIKeyCode::Affirm ();
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIAction )
	REGISTER_LUA_CLASS ( MOAIActionTree )
	REGISTER_LUA_CLASS ( MOAIAnim )
	REGISTER_LUA_CLASS ( MOAIAnimCurveBone )
	REGISTER_LUA_CLASS ( MOAIAnimCurveFloat )
	REGISTER_LUA_CLASS ( MOAIAnimCurveIndex )
	REGISTER_LUA_CLASS ( MOAIAnimCurveQuat )
	REGISTER_LUA_CLASS ( MOAIAnimCurveVec )
	REGISTER_LUA_CLASS ( MOAIButtonSensor )
	REGISTER_LUA_CLASS ( MOAICamera )
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
	REGISTER_LUA_CLASS ( MOAIDraw )
	REGISTER_LUA_CLASS ( MOAIDrawDeck )
	REGISTER_LUA_CLASS ( MOAIEaseDriver )
	REGISTER_LUA_CLASS ( MOAIEaseType )
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
	REGISTER_LUA_CLASS ( MOAILight )
	REGISTER_LUA_CLASS ( MOAILightFormat )
	REGISTER_LUA_CLASS ( MOAILocationSensor )
	REGISTER_LUA_CLASS ( MOAIMaterialBatch )
	REGISTER_LUA_CLASS ( MOAIMatrix )
	REGISTER_LUA_CLASS ( MOAIMesh )
	REGISTER_LUA_CLASS ( MOAIMetaTileDeck2D )
	REGISTER_LUA_CLASS ( MOAIMotionSensor )
	REGISTER_LUA_CLASS ( MOAINode )
	REGISTER_LUA_CLASS ( MOAINodeMgr )
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
	REGISTER_LUA_CLASS ( MOAITimer )
	REGISTER_LUA_CLASS ( MOAITouchSensor )
	REGISTER_LUA_CLASS ( MOAITransform )
	REGISTER_LUA_CLASS ( MOAIVecPathGraph )
	REGISTER_LUA_CLASS ( MOAIVectorTesselator )
	REGISTER_LUA_CLASS ( MOAIViewport )
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
