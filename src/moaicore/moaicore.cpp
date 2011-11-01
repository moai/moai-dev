// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/moaicore.h>

//================================================================//
// MOAIGlobalsFinalizer
//================================================================//
class MOAIGlobalsFinalizer :
	public USGlobalsFinalizer {
public:

	//----------------------------------------------------------------//
	void OnFinalize () {
		MOAISim::Get ().SendFinalizeEvent ();
	}
};

//================================================================//
// moaicore
//================================================================//

//----------------------------------------------------------------//
void moaicore::InitGlobals ( USGlobals* globals ) {

	static bool sysInit = true;
	if ( sysInit ) {
		cpInitChipmunk ();
		
		sysInit = false;
	}

	MOAIXmlParser::Affirm ();
	MOAIActionMgr::Affirm ();
	MOAIInputMgr::Affirm ();
	MOAILogMgr::Affirm ();
	MOAINodeMgr::Affirm ();
	MOAIVertexFormatMgr::Affirm ();
	MOAIShaderMgr::Affirm ();
	MOAIGfxDevice::Affirm ();
	MOAIDraw::Affirm ();
	MOAIDebugLines::Affirm ();
	MOAIPartitionResultMgr::Affirm ();
	MOAISim::Affirm ();
	
	// after for now; USLuaState should be highest level object
	uslsext::InitGlobals ( globals );
	
	// Start Lua
	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
	luaRuntime.Open ();
	luaRuntime.LoadLibs ( "moai" );
	
	MOAILogMessages::RegisterDefaultLogMessages ();
	
	// MOAI
	REGISTER_LUA_CLASS ( MOAIAction )
	REGISTER_LUA_CLASS ( MOAIActionMgr )
	REGISTER_LUA_CLASS ( MOAIAnim )
	REGISTER_LUA_CLASS ( MOAIAnimCurve )
	REGISTER_LUA_CLASS ( MOAIButtonSensor )
	REGISTER_LUA_CLASS ( MOAICameraAnchor2D )
	REGISTER_LUA_CLASS ( MOAICameraFitter2D )
	REGISTER_LUA_CLASS ( MOAIColor )
	REGISTER_LUA_CLASS ( MOAICompassSensor )
	REGISTER_LUA_CLASS ( MOAIDataBuffer )
	REGISTER_LUA_CLASS ( MOAIDataIOAction )
	REGISTER_LUA_CLASS ( MOAIDebugLines )
	REGISTER_LUA_CLASS ( MOAIDeckRemapper )
	REGISTER_LUA_CLASS ( MOAIDraw )
	REGISTER_LUA_CLASS ( MOAIEnvironment )
	REGISTER_LUA_CLASS ( MOAIEaseDriver )
	REGISTER_LUA_CLASS ( MOAIEaseType )
	REGISTER_LUA_CLASS ( MOAIFileSystem )
	REGISTER_LUA_CLASS ( MOAIFont )
	REGISTER_LUA_CLASS ( MOAIGfxDevice )
	REGISTER_LUA_CLASS ( MOAIGfxQuad2D )
	REGISTER_LUA_CLASS ( MOAIGfxQuadDeck2D )
	REGISTER_LUA_CLASS ( MOAIGfxQuadListDeck2D )
	REGISTER_LUA_CLASS ( MOAIGrid )
	REGISTER_LUA_CLASS ( MOAIHttpTask )
	REGISTER_LUA_CLASS ( MOAIImage )
	REGISTER_LUA_CLASS ( MOAIIndexBuffer )
	REGISTER_LUA_CLASS ( MOAIInputDevice )
	REGISTER_LUA_CLASS ( MOAIInputMgr )
	REGISTER_LUA_CLASS ( MOAIJoystickSensor )
	REGISTER_LUA_CLASS ( MOAIJsonParser )
	REGISTER_LUA_CLASS ( MOAIKeyboardSensor )
	REGISTER_LUA_CLASS ( MOAILayer2D )
	REGISTER_LUA_CLASS ( MOAILayerBridge2D )
	//REGISTER_LUA_CLASS ( MOAILayoutFrame )
	REGISTER_LUA_CLASS ( MOAILocationSensor )
	REGISTER_LUA_CLASS ( MOAILogMgr )
	REGISTER_LUA_CLASS ( MOAIMesh )
	REGISTER_LUA_CLASS ( MOAIMotionSensor )
	REGISTER_LUA_CLASS ( MOAIParser )
	REGISTER_LUA_CLASS ( MOAIParticleDistanceEmitter )
	REGISTER_LUA_CLASS ( MOAIParticleForce )
	REGISTER_LUA_CLASS ( MOAIParticleScript )
	REGISTER_LUA_CLASS ( MOAIParticleState )
	REGISTER_LUA_CLASS ( MOAIParticleSystem )
	REGISTER_LUA_CLASS ( MOAIParticleTimedEmitter )
	REGISTER_LUA_CLASS ( MOAIPartition )
	REGISTER_LUA_CLASS ( MOAIPointerSensor )
	REGISTER_LUA_CLASS ( MOAIProp )
	REGISTER_LUA_CLASS ( MOAIProp2D )
	REGISTER_LUA_CLASS ( MOAIScriptDeck )
	REGISTER_LUA_CLASS ( MOAIScriptNode )
	REGISTER_LUA_CLASS ( MOAISerializer )
	REGISTER_LUA_CLASS ( MOAIShader )
	REGISTER_LUA_CLASS ( MOAISim )
	REGISTER_LUA_CLASS ( MOAIStretchPatch2D )
	REGISTER_LUA_CLASS ( MOAISurfaceDeck2D )
	REGISTER_LUA_CLASS ( MOAITextBox )
	REGISTER_LUA_CLASS ( MOAITexture )
	REGISTER_LUA_CLASS ( MOAIThread )
	REGISTER_LUA_CLASS ( MOAITileDeck2D )
	REGISTER_LUA_CLASS ( MOAITimer )
	REGISTER_LUA_CLASS ( MOAITouchSensor )
	REGISTER_LUA_CLASS ( MOAITraits )
	REGISTER_LUA_CLASS ( MOAITransform )
	REGISTER_LUA_CLASS ( MOAIVertexBuffer )
	REGISTER_LUA_CLASS ( MOAIVertexFormat )
	REGISTER_LUA_CLASS ( MOAIViewport )
	REGISTER_LUA_CLASS ( MOAIXmlParser )
	
	#if USE_BOX2D
		REGISTER_LUA_CLASS ( MOAIBox2DArbiter )
		REGISTER_LUA_CLASS ( MOAIBox2DBody )
		REGISTER_LUA_CLASS ( MOAIBox2DDistanceJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DFixture )
		REGISTER_LUA_CLASS ( MOAIBox2DFrictionJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DGearJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DMouseJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DPrismaticJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DPulleyJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DRevoluteJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DWeldJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DWheelJoint )
		REGISTER_LUA_CLASS ( MOAIBox2DWorld )
	#endif
	
	#if USE_CHIPMUNK
	
		MOAICp::Affirm ();
	
		REGISTER_LUA_CLASS ( MOAICp )
		REGISTER_LUA_CLASS ( MOAICpArbiter )
		REGISTER_LUA_CLASS ( MOAICpBody )
		REGISTER_LUA_CLASS ( MOAICpConstraint )
		REGISTER_LUA_CLASS ( MOAICpShape )
		REGISTER_LUA_CLASS ( MOAICpSpace )
	#endif
	
	globals->PushFinalizer < MOAIGlobalsFinalizer >();
}
