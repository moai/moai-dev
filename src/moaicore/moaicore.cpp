// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <chipmunk/chipmunk.h>
#include <moaicore/moaicore.h>

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

	uslsext::InitGlobals ( globals );

	MOAIXmlParser::Get ();
	MOAIActionMgr::Get ();
	MOAIInputMgr::Get ();
	MOAILogMgr::Get ();
	MOAINodeMgr::Get ();
	MOAIDebugLines::Get ();
	MOAISim::Get ();
	
	// MOAI
	REGISTER_LUA_TYPE ( MOAIAction )
	REGISTER_LUA_TYPE ( MOAIAnim )
	REGISTER_LUA_TYPE ( MOAIAnimCurve )
	REGISTER_LUA_TYPE ( MOAIButtonSensor )
	REGISTER_LUA_TYPE ( MOAICameraFitter )
	REGISTER_LUA_TYPE ( MOAICheckpoint )
	REGISTER_LUA_TYPE ( MOAIColor )
	REGISTER_LUA_TYPE ( MOAICompassSensor )
	REGISTER_LUA_TYPE ( MOAIDataBuffer )
	REGISTER_LUA_TYPE ( MOAIDataIOAction )
	REGISTER_LUA_TYPE ( MOAIDebugLines )
	REGISTER_LUA_TYPE ( MOAIEaseDriver )
	REGISTER_LUA_TYPE ( MOAIEaseType )
	REGISTER_LUA_TYPE ( MOAIFont )
	REGISTER_LUA_TYPE ( MOAIGfxQuad2D )
	REGISTER_LUA_TYPE ( MOAIGfxQuadDeck2D )
	REGISTER_LUA_TYPE ( MOAIGfxQuadListDeck2D )
	REGISTER_LUA_TYPE ( MOAIGrid )
	REGISTER_LUA_TYPE ( MOAIHttpTask )
	REGISTER_LUA_TYPE ( MOAIIndexBuffer )
	REGISTER_LUA_TYPE ( MOAIInputDevice )
	REGISTER_LUA_TYPE ( MOAIInputMgr )
	REGISTER_LUA_TYPE ( MOAIJoystickSensor )
	REGISTER_LUA_TYPE ( MOAIKeyboardSensor )
	REGISTER_LUA_TYPE ( MOAILayer2D )
	REGISTER_LUA_TYPE ( MOAILayerBridge2D )
	REGISTER_LUA_TYPE ( MOAILayoutFrame )
	REGISTER_LUA_TYPE ( MOAILevelSensor )
	REGISTER_LUA_TYPE ( MOAILocationSensor )
	REGISTER_LUA_TYPE ( MOAILogMgr )
	REGISTER_LUA_TYPE ( MOAIOscillator )
	REGISTER_LUA_TYPE ( MOAIParticleEmitter )
	REGISTER_LUA_TYPE ( MOAIParticleForce )
	REGISTER_LUA_TYPE ( MOAIParticleScript )
	REGISTER_LUA_TYPE ( MOAIParticleState )
	REGISTER_LUA_TYPE ( MOAIParticleSystem )
	REGISTER_LUA_TYPE ( MOAIPartition )
	REGISTER_LUA_TYPE ( MOAIPointerSensor )
	REGISTER_LUA_TYPE ( MOAIProp2D )
	REGISTER_LUA_TYPE ( MOAIScriptNode )
	REGISTER_LUA_TYPE ( MOAISerializer )
	REGISTER_LUA_TYPE ( MOAISim )
	REGISTER_LUA_TYPE ( MOAISimpleShader )
	REGISTER_LUA_TYPE ( MOAIStretchPatch2D )
	REGISTER_LUA_TYPE ( MOAISurfaceDeck2D )
	REGISTER_LUA_TYPE ( MOAITextBox )
	REGISTER_LUA_TYPE ( MOAITextSpoolAction )
	REGISTER_LUA_TYPE ( MOAITexture )
	REGISTER_LUA_TYPE ( MOAIThread )
	REGISTER_LUA_TYPE ( MOAITileDeck2D )
	REGISTER_LUA_TYPE ( MOAITimer )
	REGISTER_LUA_TYPE ( MOAITouchSensor )
	REGISTER_LUA_TYPE ( MOAITransform2D )
	REGISTER_LUA_TYPE ( MOAIVertexBuffer )
	REGISTER_LUA_TYPE ( MOAIVertexFormat )
	REGISTER_LUA_TYPE ( MOAIViewport )
	REGISTER_LUA_TYPE ( MOAIXmlParser )
	
	#if USE_BOX2D
		REGISTER_LUA_TYPE ( MOAIBox2DArbiter )
		REGISTER_LUA_TYPE ( MOAIBox2DBody )
		REGISTER_LUA_TYPE ( MOAIBox2DFixture )
		REGISTER_LUA_TYPE ( MOAIBox2DWorld )
	#endif
	
	#if USE_CHIPMUNK
		REGISTER_LUA_TYPE ( MOAICp )
		REGISTER_LUA_TYPE ( MOAICpArbiter )
		REGISTER_LUA_TYPE ( MOAICpBody )
		REGISTER_LUA_TYPE ( MOAICpConstraint )
		REGISTER_LUA_TYPE ( MOAICpShape )
		REGISTER_LUA_TYPE ( MOAICpSpace )
	#endif
	
	#if USE_FMOD
		REGISTER_LUA_TYPE ( MOAIFmod )
		REGISTER_LUA_TYPE ( MOAIFmodChannel )
		REGISTER_LUA_TYPE ( MOAIFmodSound )
	#endif
}
