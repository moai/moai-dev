// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/host.h>
#include <moai-sim/headers.h>

//================================================================//
// aku
//================================================================//

//----------------------------------------------------------------//
void AKUSimAppFinalize () {
}

//----------------------------------------------------------------//
void AKUSimAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSimContextInitialize () {

	ZLBitBuffer::Test ();

	MOAINodeMgr::Affirm ();
	MOAIActionStackMgr::Affirm ();

	//MOAIProfiler::Affirm ();
	MOAIGfxResourceMgr::Affirm ();
	MOAIGfxDevice::Affirm ();
	MOAIImageFormatMgr::Affirm ();
	MOAIVertexFormatMgr::Affirm ();
	MOAIShaderMgr::Affirm ();
	MOAIDraw::Affirm ();
	MOAIDebugLines::Affirm ();
	MOAIPartitionResultMgr::Affirm ();
	MOAINodeMgr::Affirm ();
	MOAIInputMgr::Affirm ();
	MOAISim::Affirm ();
	MOAIRenderMgr::Affirm ();
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIAction )
	REGISTER_LUA_CLASS ( MOAIActionTree )
	REGISTER_LUA_CLASS ( MOAIAnim )
	REGISTER_LUA_CLASS ( MOAIAnimCurve )
	REGISTER_LUA_CLASS ( MOAIAnimCurveQuat )
	REGISTER_LUA_CLASS ( MOAIAnimCurveVec )
	REGISTER_LUA_CLASS ( MOAIBoundsDeck )
	//REGISTER_LUA_CLASS ( MOAIBspBuilder2D )
	REGISTER_LUA_CLASS ( MOAIButtonSensor )
	REGISTER_LUA_CLASS ( MOAICamera )
	REGISTER_LUA_CLASS ( MOAICameraAnchor2D )
	REGISTER_LUA_CLASS ( MOAICameraFitter2D )
	REGISTER_LUA_CLASS ( MOAICollisionProp )
	REGISTER_LUA_CLASS ( MOAICollisionWorld )
	REGISTER_LUA_CLASS ( MOAIColor )
	REGISTER_LUA_CLASS ( MOAICompassSensor )
	REGISTER_LUA_CLASS ( MOAICoroutine )
	REGISTER_LUA_CLASS ( MOAIDebugLines )
	REGISTER_LUA_CLASS ( MOAIDeckRemapper )
	REGISTER_LUA_CLASS ( MOAIDraw )
	REGISTER_LUA_CLASS ( MOAIEaseDriver )
	REGISTER_LUA_CLASS ( MOAIEaseType )
	REGISTER_LUA_CLASS ( MOAIFrameBuffer )
	REGISTER_LUA_CLASS ( MOAIFrameBufferTexture )
	REGISTER_LUA_CLASS ( MOAIGeometryWriter )
	REGISTER_LUA_CLASS ( MOAIGfxBuffer )
	REGISTER_LUA_CLASS ( MOAIGfxDevice )
	REGISTER_LUA_CLASS ( MOAIGfxQuad2D )
	REGISTER_LUA_CLASS ( MOAIGfxQuadDeck2D )
	REGISTER_LUA_CLASS ( MOAIGfxQuadListDeck2D )
	REGISTER_LUA_CLASS ( MOAIGfxResourceMgr )
	REGISTER_LUA_CLASS ( MOAIGraphicsProp )
	REGISTER_LUA_CLASS ( MOAIGrid )
	REGISTER_LUA_CLASS ( MOAIGridDeck2D )
	REGISTER_LUA_CLASS ( MOAIGridSpace )
	REGISTER_LUA_CLASS ( MOAIGridPathGraph )
	REGISTER_LUA_CLASS ( MOAIGridFancy )
	REGISTER_LUA_CLASS ( MOAIImage )
	REGISTER_LUA_CLASS ( MOAIImageTexture )
	REGISTER_LUA_CLASS ( MOAIInputDevice )
	REGISTER_LUA_CLASS ( MOAIInputMgr )
	REGISTER_LUA_CLASS ( MOAIJoystickSensor )
	REGISTER_LUA_CLASS ( MOAIKeyCode )
	REGISTER_LUA_CLASS ( MOAIKeyboardSensor )
	REGISTER_LUA_CLASS ( MOAILayer )
	//REGISTER_LUA_CLASS ( MOAILayoutFrame )
	REGISTER_LUA_CLASS ( MOAILocationSensor )
	REGISTER_LUA_CLASS ( MOAIMaterialBatch )
	REGISTER_LUA_CLASS ( MOAIMatrix )
	REGISTER_LUA_CLASS ( MOAIMesh )
	REGISTER_LUA_CLASS ( MOAIMotionSensor )
	REGISTER_LUA_CLASS ( MOAIMultiTexture )
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
	REGISTER_LUA_CLASS ( MOAIPathFinder )
	REGISTER_LUA_CLASS ( MOAIPathTerrainDeck )
	REGISTER_LUA_CLASS ( MOAIPinTransform )
	REGISTER_LUA_CLASS ( MOAIPointerSensor )
	//REGISTER_LUA_CLASS ( MOAIProfilerReportBox )
	REGISTER_LUA_CLASS ( MOAIRegion )
	REGISTER_LUA_CLASS ( MOAIRenderMgr )
	REGISTER_LUA_CLASS ( MOAIScissorRect )
	REGISTER_LUA_CLASS ( MOAIScriptDeck )
	REGISTER_LUA_CLASS ( MOAIScriptNode )
	REGISTER_LUA_CLASS ( MOAISelectionMesh )
	REGISTER_LUA_CLASS ( MOAIShader )
	REGISTER_LUA_CLASS ( MOAIShaderMgr )
	REGISTER_LUA_CLASS ( MOAIShaderProgram )
	REGISTER_LUA_CLASS ( MOAISim )
	REGISTER_LUA_CLASS ( MOAIStretchPatch2D )
	REGISTER_LUA_CLASS ( MOAISurfaceDeck2D )
	REGISTER_LUA_CLASS ( MOAITexture )
	REGISTER_LUA_CLASS ( MOAITileDeck2D )
	REGISTER_LUA_CLASS ( MOAITimer )
	REGISTER_LUA_CLASS ( MOAITouchSensor )
	REGISTER_LUA_CLASS ( MOAITransform )
	REGISTER_LUA_CLASS ( MOAIVecPathGraph )
	REGISTER_LUA_CLASS ( MOAIVectorTesselator )
	REGISTER_LUA_CLASS ( MOAIVertexFormat )
	REGISTER_LUA_CLASS ( MOAIVertexFormatMgr )
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
void AKUDetectFramebuffer () {

	MOAIGfxDevice::Get ().DetectFramebuffer ();
}

//----------------------------------------------------------------//
void AKUDetectGfxContext () {

	MOAIGfxDevice::Get ().DetectContext ();
}

//----------------------------------------------------------------//
void AKUEnqueueButtonEvent ( int deviceID, int sensorID, bool down ) {

	MOAIButtonSensor::EnqueueButtonEvent (( u8 )deviceID, ( u8 )sensorID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueCompassEvent ( int deviceID, int sensorID, float heading ) {

	MOAICompassSensor::EnqueueCompassEvent (( u8 )deviceID, ( u8 )sensorID, heading );
}

//----------------------------------------------------------------//
void AKUEnqueueJoystickEvent( int deviceID, int sensorID, float x, float y ) {

	MOAIJoystickSensor::EnqueueJoystickEvent (( u8 )deviceID, ( u8 )sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardCharEvent ( int deviceID, int sensorID, int unicodeChar ) {

	MOAIKeyboardSensor::EnqueueKeyboardCharEvent (( u8 )deviceID, ( u8 )sensorID, unicodeChar );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardEditEvent ( int deviceID, int sensorID, char const* text, int start, int editLength, int maxLength) {
	
	MOAIKeyboardSensor::EnqueueKeyboardEditEvent (( u8 )deviceID, ( u8 )sensorID, text, ( u32 )start, ( u32 )editLength, ( u32 )maxLength );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardKeyEvent ( int deviceID, int sensorID, int keyID, bool down ) {

	MOAIKeyboardSensor::EnqueueKeyboardKeyEvent (( u8 )deviceID, ( u8 )sensorID, keyID, down );
}

//----------------------------------------------------------------//
void AKUEnqueueKeyboardTextEvent ( int deviceID, int sensorID, const char* text ) {

	MOAIKeyboardSensor::EnqueueKeyboardTextEvent (( u8 )deviceID, ( u8 )sensorID, text );
}

//----------------------------------------------------------------//
void AKUEnqueueLevelEvent ( int deviceID, int sensorID, float x, float y, float z ) {

	MOAIMotionSensor::EnqueueLevelEvent (( u8 )deviceID, ( u8 )sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueLocationEvent ( int deviceID, int sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	MOAILocationSensor::EnqueueLocationEvent (( u8 )deviceID, ( u8 )sensorID, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
}

//----------------------------------------------------------------//
void AKUEnqueuePointerEvent ( int deviceID, int sensorID, int x, int y ) {

	MOAIPointerSensor::EnqueuePointerEvent (( u8 )deviceID, ( u8 )sensorID, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y ) {

	MOAITouchSensor::EnqueueTouchEvent (( u8 )deviceID, ( u8 )sensorID, ( u32 )touchID, down, x, y );
}

//----------------------------------------------------------------//
void AKUEnqueueTouchEventCancel ( int deviceID, int sensorID ) {

	MOAITouchSensor::EnqueueTouchEventCancel (( u8 )deviceID, ( u8 )sensorID );
}

//----------------------------------------------------------------//
void AKUEnqueueVectorEvent ( int deviceID, int sensorID, int touchID, bool down, float x, float y, float z ) {

	MOAIVectorSensor::EnqueueVectorEvent (( u8 )deviceID, ( u8 )sensorID, x, y, z );
}

//----------------------------------------------------------------//
void AKUEnqueueWheelEvent ( int deviceID, int sensorID, float value ) {

	MOAIWheelSensor::EnqueueWheelEvent (( u8 )deviceID, ( u8 )sensorID, value );
}

//----------------------------------------------------------------//
double AKUGetSimStep () {

	return MOAISim::Get ().GetStep ();
}

//----------------------------------------------------------------//
int AKUIsGfxBufferOpaque () {

	return MOAIGfxDevice::Get ().IsOpaque ();
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

	MOAIRenderMgr::Get ().Render ();
}

//----------------------------------------------------------------//
void AKUSetFunc_HideCursor ( AKUHideCursorFunc func ) {

	MOAISim::Get ().SetHideCursorFunc ( func );
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
void AKUSetInputConfigurationName ( char const* name ) {

	MOAIInputMgr::Get ().SetConfigurationName ( name );
}

//----------------------------------------------------------------//
void AKUSetInputDevice ( int deviceID, char const* name ) {

	MOAIInputMgr::Get ().SetDevice (( u8 )deviceID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceHardwareInfo ( int deviceID, char const* hardwareInfo ) {

	MOAIInputMgr::Get ().SetDeviceHardwareInfo (( u8 )deviceID, hardwareInfo );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceActive ( int deviceID, bool active ) {

	MOAIInputMgr::Get ().SetDeviceActive (( u8 )deviceID, active );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceButton ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIButtonSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceCompass ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAICompassSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceKeyboard ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIKeyboardSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceJoystick ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIJoystickSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLevel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIMotionSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceLocation ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAILocationSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDevicePointer ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIPointerSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceTouch ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAITouchSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceVector ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIVectorSensor >(( u8 )deviceID, ( u8 )sensorID, name );
}

//----------------------------------------------------------------//
void AKUSetInputDeviceWheel ( int deviceID, int sensorID, char const* name ) {

	MOAIInputMgr::Get ().SetSensor < MOAIWheelSensor >(( u8 )deviceID, ( u8 )sensorID, name );
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
		
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( device.PushListener ( MOAIGfxDevice::EVENT_RESIZE, state )) {
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
void AKUUpdate () {

	MOAISim::Get ().Update ();
}
