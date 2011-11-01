// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAICORE_H
#define	MOAICORE_H

#include <moaicore/pch.h>

#include <moaicore/MOAIAction.h>
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAIAnim.h>
#include <moaicore/MOAIAnimCurve.h>
#include <moaicore/MOAIBitmapFontRipper.h>
#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAIButtonSensor.h>
#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAICameraFitter2D.h>
#include <moaicore/MOAIColor.h>
#include <moaicore/MOAICompassSensor.h>
#include <moaicore/MOAIDataBuffer.h>
#include <moaicore/MOAIDataIOAction.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIDeck.h>
#include <moaicore/MOAIDeck2D.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAIEaseDriver.h>
#include <moaicore/MOAIEaseType.h>
#include <moaicore/MOAIEventSource.h>
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAIFont.h>
#include <moaicore/MOAIFrameBuffer.h>
#include <moaicore/MOAIFreetypeFontRipper.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxQuad2D.h>
#include <moaicore/MOAIGfxQuadDeck2D.h>
#include <moaicore/MOAIGfxQuadListDeck2D.h>
#include <moaicore/MOAIGlyph.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAIGridSpace.h>
#include <moaicore/MOAIHttpTask.h>
#include <moaicore/MOAIImage.h>
#include <moaicore/MOAIIndexBuffer.h>
#include <moaicore/MOAIInputDevice.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAIJoystickSensor.h>
#include <moaicore/MOAIJsonParser.h>
#include <moaicore/MOAIKeyboardSensor.h>
#include <moaicore/MOAILayer2D.h>
#include <moaicore/MOAILayerBridge2D.h>
#include <moaicore/MOAILayoutFrame.h>
#include <moaicore/MOAILineBrush.h>
#include <moaicore/MOAILocationSensor.h>
#include <moaicore/MOAILogMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMesh.h>
#include <moaicore/MOAIMotionSensor.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIParser.h>
#include <moaicore/MOAIParticle.h>
#include <moaicore/MOAIParticleDistanceEmitter.h>
#include <moaicore/MOAIParticleEmitter.h>
#include <moaicore/MOAIParticleForce.h>
#include <moaicore/MOAIParticlePlugin.h>
#include <moaicore/MOAIParticleScript.h>
#include <moaicore/MOAIParticleState.h>
#include <moaicore/MOAIParticleSystem.h>
#include <moaicore/MOAIParticleTimedEmitter.h>
#include <moaicore/MOAIPartition.h>
#include <moaicore/MOAIPartitionCell.h>
#include <moaicore/MOAIPartitionLayer.h>
#include <moaicore/MOAIPartitionResultBuffer.h>
#include <moaicore/MOAIPartitionResultMgr.h>
#include <moaicore/MOAIPointerSensor.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAIPvrHeader.h>
#include <moaicore/MOAIQuadBrush.h>
#include <moaicore/MOAIScriptDeck.h>
#include <moaicore/MOAIScriptNode.h>
#include <moaicore/MOAISensor.h>
#include <moaicore/MOAISerializer.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIStretchPatch2D.h>
#include <moaicore/MOAISurfaceDeck2D.h>
#include <moaicore/MOAITextBox.h>
#include <moaicore/MOAITextFrame.h>
#include <moaicore/MOAITextLayout.h>
#include <moaicore/MOAITexture.h>
#include <moaicore/MOAIThread.h>
#include <moaicore/MOAITileDeck2D.h>
#include <moaicore/MOAITileFlags.h>
#include <moaicore/MOAITimer.h>
#include <moaicore/MOAITouchSensor.h>
#include <moaicore/MOAITraits.h>
#include <moaicore/MOAITransform.h>
#include <moaicore/MOAITransformBase.h>
#include <moaicore/MOAIVertexBuffer.h>
#include <moaicore/MOAIVertexFormat.h>
#include <moaicore/MOAIVertexFormatMgr.h>
#include <moaicore/MOAIViewport.h>
#include <moaicore/MOAIXmlParser.h>

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

//================================================================//
// moaicore
//================================================================//
namespace moaicore {

	//----------------------------------------------------------------//
	void			InitGlobals			( USGlobals* globals );
}

#endif
