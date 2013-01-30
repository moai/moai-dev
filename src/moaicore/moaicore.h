// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICORE_H
#define	MOAICORE_H

#include <moaicore/pch.h>

#include <moaicore/MOAILuaSharedPtr.h>
#include <moaicore/MOAILuaState.h>
#include <moaicore/MOAILuaStateHandle.h>
#include <moaicore/MOAILuaObject.h>
#include <moaicore/MOAILuaRuntime.h>
#include <moaicore/MOAILuaRef.h>

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIAnimCurveBase.h>
#include <moaicore/MOAIAnimCurveQuat.h>
#include <moaicore/MOAIAnimCurveVec.h>
#include <moaicore/MOAIAttrOp.h>
#include <moaicore/MOAIBitmapFontReader.h>
#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAIBoundsDeck.h>
#include <moaicore/MOAIButtonSensor.h>
#include <moaicore/MOAICamera.h>
#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAICameraFitter2D.h>
#include <moaicore/MOAICanary.h>
#include <moaicore/MOAICollisionShape.h>
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAICompassSensor.h>
#include <moaicore/MOAICoroutine.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIDataBufferStream.h>
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDeserializer.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIEaseType.h>
#include <moaicore/MOAIEventSource.h>
#include <moaicore/MOAIFileStream.h>
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAIFoo.h>
#include <moaicore/MOAIFooMgr.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFontReader.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIFreeTypeFontReader.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxQuad2D.h>
#include <moaicore/MOAIGfxQuadDeck2D.h>
#include <moaicore/MOAIGfxQuadListDeck2D.h>
#include <moaicore/MOAIGlobals.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIGlyphCache.h>
#include <moaicore/MOAIGlyphCacheBase.h>
#include <moaicore/MOAIGlyphCachePage.h>
#include <moaicore/MOAIGlyphSet.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGridDeck2D.h>
#include <moaicore/MOAIGridPathGraph.h>
#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIHashWriter.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIImageTexture.h>
#include <moaicore/MOAIIndexBuffer.h>
#include <moaicore/MOAIInputDevice.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAIJoystickSensor.h>
#include <moaicore/MOAIJsonParser.h>
#include <moaicore/MOAIKeyboardSensor.h>
#include <moaicore/MOAILayer.h>
#include <moaicore/MOAILayerBridge.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAILocationSensor.h>
#include <moaicore/MOAILogMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMemStream.h>
#include <moaicore/MOAIMesh.h>
#include <moaicore/MOAIMotionSensor.h>
#include <moaicore/MOAIMultiTexture.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIObject.h>
#include <moaicore/MOAIParser.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleCallbackPlugin.h>
#include <moaicore/MOAIParticleDistanceEmitter.h>
#include <moaicore/MOAIParticleEmitter.h>
#include <moaicore/MOAIParticleForce.h>
#include <moaicore/MOAIParticlePexPlugin.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAIParticleTimedEmitter.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLevel.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIPartitionResultMgr.h>
#include <moaicore/MOAIPathFinder.h>
#include <moaicore/MOAIPathTerrainDeck.h>
#include <moaicore/MOAIPointerSensor.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIPvrHeader.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAIRenderable.h>
#include <moaicore/MOAIRenderMgr.h>
#include <moaicore/MOAIRtti.h>
#include <moaicore/MOAIScissorRect.h>
#include <moaicore/MOAIScriptDeck.h>
#include <moaicore/MOAIScriptNode.h>
#include <moaicore/MOAISensor.h>
#include <moaicore/MOAISerializer.h>
#include <moaicore/MOAISerializerBase.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISharedPtr.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAISpanList.h>
#include <moaicore/MOAIStaticGlyphCache.h>
#include <moaicore/MOAIStream.h>
#include <moaicore/MOAIStreamReader.h>
#include <moaicore/MOAIStreamWriter.h>
#include <moaicore/MOAIStretchPatch2D.h>
#include <moaicore/MOAISurfaceDeck2D.h>
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAITextBundle.h>
#include <moaicore/MOAITextDesigner.h>
#include <moaicore/MOAITextStyle.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAITextureBase.h>
#include <moaicore/MOAITileDeck2D.h>
#include <moaicore/MOAITileFlags.h>
#include <moaicore/MOAITimer.h>
#include <moaicore/MOAITouchSensor.h>
#include <moaicore/MOAITransform.h>
#include <moaicore/MOAITransformBase.h>
#include <moaicore/MOAIVertexBuffer.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexFormatMgr.h>
#include <moaicore/MOAIViewport.h>
#include <moaicore/MOAIWeakPtr.h>
#include <moaicore/MOAIWheelSensor.h>
#include <moaicore/MOAIXmlParser.h>
#include <moaicore/MOAIXmlWriter.h>

#if USE_BOX2D
	#include <moaicore/MOAIBox2DArbiter.h>
	#include <moaicore/MOAIBox2DBody.h>
	#include <moaicore/MOAIBox2DDebugDraw.h>
	#include <moaicore/MOAIBox2DDistanceJoint.h>
	#include <moaicore/MOAIBox2DFixture.h>
	#include <moaicore/MOAIBox2DFrictionJoint.h>
	#include <moaicore/MOAIBox2DGearJoint.h>
	#include <moaicore/MOAIBox2DJoint.h>
	#include <moaicore/MOAIBox2DMouseJoint.h>
	#include <moaicore/MOAIBox2DPrismaticJoint.h>
	#include <moaicore/MOAIBox2DPulleyJoint.h>
	#include <moaicore/MOAIBox2DRopeJoint.h>
	#include <moaicore/MOAIBox2DRevoluteJoint.h>
	#include <moaicore/MOAIBox2DWeldJoint.h>
	#include <moaicore/MOAIBox2DWheelJoint.h>
	#include <moaicore/MOAIBox2DWorld.h>
#endif

#if USE_CHIPMUNK
	#include <moaicore/MOAICp.h>
	#include <moaicore/MOAICpArbiter.h>
	#include <moaicore/MOAICpBody.h>
	#include <moaicore/MOAICpConstraint.h>
	#include <moaicore/MOAICpDebugDraw.h>
	#include <moaicore/MOAICpShape.h>
	#include <moaicore/MOAICpSpace.h>
#endif

#if USE_CURL
	#include <moaicore/MOAIHttpTaskCurl.h>
	#include <moaicore/MOAIUrlMgrCurl.h>
#endif

#if MOAI_OS_NACL
	#include <moaicore/MOAIHttpTaskNaCl.h>
	#include <moaicore/MOAIUrlMgrNaCl.h>
#endif

#include <moaicore/MOAILuaState-impl.h>

//================================================================//
// moaicore
//================================================================//
namespace moaicore {

	//----------------------------------------------------------------//
	void			InitGlobals			( MOAIGlobals* globals ); // initialize a Moai context
	void			SystemFinalize		(); // call *once* at exit
	void			SystemInit			(); // call *once* at program startup
}

#endif
