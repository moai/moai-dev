// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/host.h>
#include <moai-sim/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUEnqueueButtonEvent ( AKUContextID contextID, int deviceID, int sensorID, bool down ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIButtonSensor::EnqueueButtonEvent ( inputMgr, deviceID, sensorID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueCompassEvent ( AKUContextID contextID, int deviceID, int sensorID, float heading ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAICompassSensor::EnqueueCompassEvent ( inputMgr, deviceID, sensorID, heading );
}

//----------------------------------------------------------------//
void AKUEnqueueJoystickEvent ( AKUContextID contextID, int deviceID, int sensorID, float x, float y ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIJoystickSensor::EnqueueJoystickEvent ( inputMgr, deviceID, sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardCharEvent ( AKUContextID contextID, int deviceID, int sensorID, int unicodeChar ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIKeyboardSensor::EnqueueKeyboardCharEvent ( inputMgr, deviceID, sensorID, unicodeChar );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEditEvent ( AKUContextID contextID, int deviceID, int sensorID, char const* text, int start, int editLength, int maxLength) {
	
	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIKeyboardSensor::EnqueueKeyboardEditEvent ( inputMgr, deviceID, sensorID, text, ( u32 )start, ( u32 )editLength, ( u32 )maxLength );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardKeyEvent ( AKUContextID contextID, int deviceID, int sensorID, int keyID, bool down ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIKeyboardSensor::EnqueueKeyboardKeyEvent ( inputMgr, deviceID, sensorID, keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardTextEvent ( AKUContextID contextID, int deviceID, int sensorID, const char* text ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIKeyboardSensor::EnqueueKeyboardTextEvent ( inputMgr, deviceID, sensorID, text );
}

//----------------------------------------------------------------//
void AKUEnqueueLevelEvent ( AKUContextID contextID, int deviceID, int sensorID, float x, float y, float z ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIMotionSensor::EnqueueLevelEvent ( inputMgr, deviceID, sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueLocationEvent ( AKUContextID contextID, int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAILocationSensor::EnqueueLocationEvent ( inputMgr, deviceID, sensorID, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
}

//----------------------------------------------------------------//
void AKUEnqueuePointerEvent ( AKUContextID contextID, int deviceID, int sensorID, int x, int y ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIPointerSensor::EnqueuePointerEvent ( inputMgr, deviceID, sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEvent ( AKUContextID contextID, int deviceID, int sensorID, int touchID, bool down, float x, float y ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAITouchSensor::EnqueueTouchEvent ( inputMgr, deviceID, sensorID, ( u32 )touchID, down, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( AKUContextID contextID, int deviceID, int sensorID ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAITouchSensor::EnqueueTouchEventCancel ( inputMgr, deviceID, sensorID );
}

//----------------------------------------------------------------//
void AKUEnqueueVectorEvent ( AKUContextID contextID, int deviceID, int sensorID, float x, float y, float z ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIVectorSensor::EnqueueVectorEvent ( inputMgr, deviceID, sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueWheelEvent ( AKUContextID contextID, int deviceID, int sensorID, float value ) {

	MOAIInputMgr& inputMgr = (( ZLContext* )contextID )->Get < MOAIInputMgr >();
	MOAIWheelSensor::EnqueueWheelEvent ( inputMgr, deviceID, sensorID, value );
}

//----------------------------------------------------------------//
double AKUGetSimStep ( AKUContextID contextID ) {

	return (( ZLContext* )contextID )->Get < MOAISim >().GetStep ();
}

//----------------------------------------------------------------//
void AKUPause ( AKUContextID contextID, bool pause ) {

	MOAISim& sim = (( ZLContext* )contextID )->Get < MOAISim >();

	if ( pause ) {
		sim.Pause ();
	}
	else {
		sim.Resume ();
	}
}

//----------------------------------------------------------------//
void AKURender ( AKUContextID contextID ) {

	(( ZLContext* )contextID )->Get < MOAIGfxMgr >().Render ();
}

//----------------------------------------------------------------//
void AKUReserveInputDevices ( AKUContextID contextID, int total ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().ReserveDevices (( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUReserveInputDeviceSensors ( AKUContextID contextID, int deviceID, int total ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().ReserveSensors ( deviceID, ( ZLSize )total );
}

//----------------------------------------------------------------//
void AKUSetFunc_EnterFullscreenMode ( AKUContextID contextID, AKUEnterFullscreenModeFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetEnterFullscreenModeFunc ( MOAISim::EnterFullscreenModeFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_ExitFullscreenMode ( AKUContextID contextID, AKUExitFullscreenModeFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetExitFullscreenModeFunc ( MOAISim::ExitFullscreenModeFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_HideCursor ( AKUContextID contextID, AKUHideCursorFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetHideCursorFunc ( MOAISim::HideCursorFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_OpenWindow ( AKUContextID contextID, AKUOpenWindowFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetOpenWindowFunc ( MOAISim::OpenWindowFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_SetSimStep ( AKUContextID contextID, AKUSetSimStepFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetSetSimStepFunc ( MOAISim::SetSimStepFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_SetTextInputRect ( AKUContextID contextID, AKUSetTextInputRectFunc func, void* userdata ) {
	
	(( ZLContext* )contextID )->Get < MOAISim >().SetSetTextInputRectFunc ( MOAISim::SetTextInputRectFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetFunc_ShowCursor ( AKUContextID contextID, AKUShowCursorFunc func, void* userdata ) {

	(( ZLContext* )contextID )->Get < MOAISim >().SetShowCursorFunc ( MOAISim::ShowCursorFunc ( func, userdata ));
}

//----------------------------------------------------------------//
void AKUSetInputAutoTimestamp ( AKUContextID contextID, bool autotimestamp ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetAutotimestamp ( autotimestamp );
}

//----------------------------------------------------------------//
void AKUSetInputConfigurationName ( AKUContextID contextID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetConfigurationName ( name );
}

//----------------------------------------------------------------//
void AKUSetInputDevice ( AKUContextID contextID, int deviceID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetDevice ( deviceID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceHardwareInfo ( AKUContextID contextID, int deviceID, char const* hardwareInfo ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetDeviceHardwareInfo ( deviceID, hardwareInfo );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceActive ( AKUContextID contextID, int deviceID, bool active ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetDeviceActive ( deviceID, active );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceButton ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIButtonSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceCompass ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAICompassSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceKeyboard ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIKeyboardSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceJoystick ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIJoystickSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLevel ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIMotionSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLocation ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAILocationSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDevicePointer ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIPointerSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceTouch ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAITouchSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceVector ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIVectorSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceWheel ( AKUContextID contextID, int deviceID, int sensorID, char const* name ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetSensor < MOAIWheelSensor >( deviceID, sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputTimebase ( AKUContextID contextID, double timebase ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetTimebase ( timebase );
}

//----------------------------------------------------------------//
void AKUSetInputTimestamp ( AKUContextID contextID, double timestamp ) {

	(( ZLContext* )contextID )->Get < MOAIInputMgr >().SetTimestamp ( timestamp );
}

//----------------------------------------------------------------//
void AKUSetOrientation ( AKUContextID contextID, int orientation ) {

	(( ZLContext* )contextID )->Get < MOAIGfxMgr >().GetDefaultFrameBuffer ()->SetLandscape ( orientation == AKU_ORIENTATION_LANDSCAPE );
}

//----------------------------------------------------------------//	
void AKUSetScreenDpi ( AKUContextID contextID, int dpi ) {

	(( ZLContext* )contextID )->Get < MOAIEnvironment >().SetValue ( MOAI_ENV_screenDpi, dpi );
}

//----------------------------------------------------------------//
void AKUSetScreenSize ( AKUContextID contextID, int width, int height) {

	MOAIEnvironment& environment = (( ZLContext* )contextID )->Get < MOAIEnvironment >();

	environment.SetValue ( MOAI_ENV_horizontalResolution, width );
	environment.SetValue ( MOAI_ENV_verticalResolution, height );
}

//----------------------------------------------------------------//
void AKUSetViewSize ( AKUContextID contextID, int width, int height ) {
	
	MOAIGfxMgr& gfxMgr = (( ZLContext* )contextID )->Get < MOAIGfxMgr >();
	
	u32 currentWidth = gfxMgr.GetBufferWidth ();
	u32 currentHeight = gfxMgr.GetBufferHeight ();
	
	if (( currentWidth != ( u32 )width ) || ( currentHeight != ( u32 )height )) {
	
		gfxMgr.GetDefaultFrameBuffer ()->SetFrameSize ( width, height );
		
		MOAIScopedLuaState state = (( ZLContext* )contextID )->Get < MOAILuaRuntime >().State ();
		
		if ( gfxMgr.PushListener ( MOAIGfxMgr::EVENT_RESIZE, state )) {
			lua_pushnumber ( state, width );
			lua_pushnumber ( state, height );
			state.DebugCall ( 2, 0 );
		}
	}
}

//----------------------------------------------------------------//
void AKUSimAppFinalize () {
}

//----------------------------------------------------------------//
void AKUSimAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSimContextInitialize ( AKUContextID contextID ) {

	assert ( contextID );
	ZLContext* context = ( ZLContext* )contextID;

	context->Affirm < MOAIDebugLinesMgr >();
	context->Affirm < MOAIInputMgr >();
	context->Affirm < MOAIDraw >();
	
	// this is a stopgap; eventually MOAISim will be split up.
	// right now it's just a bag of legacy.
	context->Affirm < MOAISim >();
	context->RegisterAlias < MOAISim, MOAIUpdateMgr >();
	
	context->Affirm < MOAIMeshWriter >();
	context->Affirm < MOAIKeyCode >();
	
	// MOAI
	REGISTER_LUA_CLASS ( context, MOAIButtonSensor )
	REGISTER_LUA_CLASS ( context, MOAICameraAnchor2D )
	REGISTER_LUA_CLASS ( context, MOAICameraFitter2D )
	REGISTER_LUA_CLASS ( context, MOAICollisionDeck )
	REGISTER_LUA_CLASS ( context, MOAICollisionProp )
	REGISTER_LUA_CLASS ( context, MOAICollisionWorld )
	REGISTER_LUA_CLASS ( context, MOAICompassSensor )
	REGISTER_LUA_CLASS ( context, MOAIDebugLinesMgr )
	REGISTER_LUA_CLASS ( context, MOAIDeckRemapper )
	REGISTER_LUA_CLASS ( context, MOAIDrawDeck )
	REGISTER_LUA_CLASS ( context, MOAIFancyGrid )
	REGISTER_LUA_CLASS ( context, MOAIGraphicsProp )
	REGISTER_LUA_CLASS ( context, MOAIGraphicsGridProp )
	REGISTER_LUA_CLASS ( context, MOAIGrid )
	REGISTER_LUA_CLASS ( context, MOAIGridSpace )
	REGISTER_LUA_CLASS ( context, MOAIGridPathGraph )
	REGISTER_LUA_CLASS ( context, MOAIHitMask )
	REGISTER_LUA_CLASS ( context, MOAIHitMaskBatch )
	REGISTER_LUA_CLASS ( context, MOAIImage )
	REGISTER_LUA_CLASS ( context, MOAIInputDevice )
	REGISTER_LUA_CLASS ( context, MOAIInputMgr )
	REGISTER_LUA_CLASS ( context, MOAIJoystickSensor )
	REGISTER_LUA_CLASS ( context, MOAIKeyCode )
	REGISTER_LUA_CLASS ( context, MOAIKeyboardSensor )
	REGISTER_LUA_CLASS ( context, MOAILocationSensor )
	REGISTER_LUA_CLASS ( context, MOAIMeshDeck )
	REGISTER_LUA_CLASS ( context, MOAIMeshWriter )
	REGISTER_LUA_CLASS ( context, MOAIMetaTileDeck2D )
	REGISTER_LUA_CLASS ( context, MOAIMotionSensor )
	REGISTER_LUA_CLASS ( context, MOAIParticleCallbackPlugin )
	REGISTER_LUA_CLASS ( context, MOAIParticleDistanceEmitter )
	REGISTER_LUA_CLASS ( context, MOAIParticleForce )
	REGISTER_LUA_CLASS ( context, MOAIParticleScript )
	REGISTER_LUA_CLASS ( context, MOAIParticleState )
	REGISTER_LUA_CLASS ( context, MOAIParticleSystem )
	REGISTER_LUA_CLASS ( context, MOAIParticleTimedEmitter )
	REGISTER_LUA_CLASS ( context, MOAIPartition )
	REGISTER_LUA_CLASS ( context, MOAIPartitionResultBuffer )
	REGISTER_LUA_CLASS ( context, MOAIPartitionViewLayer )
	REGISTER_LUA_CLASS ( context, MOAIPath )
	REGISTER_LUA_CLASS ( context, MOAIPathFinder )
	REGISTER_LUA_CLASS ( context, MOAIPathStepper )
	REGISTER_LUA_CLASS ( context, MOAIPathTerrainDeck )
	REGISTER_LUA_CLASS ( context, MOAIPinTransform )
	REGISTER_LUA_CLASS ( context, MOAIPointerSensor )
	REGISTER_LUA_CLASS ( context, MOAIProjectionProp )
	REGISTER_LUA_CLASS ( context, MOAIRegion )
	REGISTER_LUA_CLASS ( context, MOAISim )
	REGISTER_LUA_CLASS ( context, MOAISpriteDeck2D )
	REGISTER_LUA_CLASS ( context, MOAIStretchPatch2D )
	REGISTER_LUA_CLASS ( context, MOAITableViewLayer )
	REGISTER_LUA_CLASS ( context, MOAITileDeck2D )
	REGISTER_LUA_CLASS ( context, MOAITouchSensor )
	REGISTER_LUA_CLASS ( context, MOAIVecPathGraph )
	REGISTER_LUA_CLASS ( context, MOAIVectorTesselator )
	REGISTER_LUA_CLASS ( context, MOAIWheelSensor )
	
	// text stuff here for now
	REGISTER_LUA_CLASS ( context, MOAIBitmapFontReader )
	REGISTER_LUA_CLASS ( context, MOAIDynamicGlyphCache )
	REGISTER_LUA_CLASS ( context, MOAIFont )
	REGISTER_LUA_CLASS ( context, MOAIStaticGlyphCache )
	REGISTER_LUA_CLASS ( context, MOAITextBundle )
	REGISTER_LUA_CLASS ( context, MOAITextLabel )context,
	REGISTER_LUA_CLASS ( context, MOAITextStyle )
	
	#if MOAI_WITH_TINYXML
		REGISTER_LUA_CLASS ( context, MOAIParticlePexPlugin )
	#endif
	
	#if MOAI_WITH_FREETYPE
		REGISTER_LUA_CLASS ( context, MOAIFreeTypeFontReader )
	#endif
}

//----------------------------------------------------------------//
void AKUUpdate ( AKUContextID contextID ) {

	(( ZLContext* )contextID )->Get < MOAISim >().Update ();
}
