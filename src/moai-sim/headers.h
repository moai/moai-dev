// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAI_SIM_HEADERS_H
#define	MOAI_SIM_HEADERS_H

#include <moai-sim/pch.h>
#include <moai-sim/strings.h>

#include <moai-sim/MOAIAbstractBaseTransform.h>
#include <moai-sim/MOAIAbstractChildTransform.h>
#include <moai-sim/MOAIAbstractDrawable.h>
#include <moai-sim/MOAIAbstractDrawShape.h>
#include <moai-sim/MOAIAbstractLayer.h>
#include <moai-sim/MOAIAbstractProp.h>
#include <moai-sim/MOAIAbstractViewLayer.h>
#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIActionTree.h>
#include <moai-sim/MOAIAnim.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIAnimCurveBone.h>
#include <moai-sim/MOAIAnimCurveFloat.h>
#include <moai-sim/MOAIAnimCurveIndex.h>
#include <moai-sim/MOAIAnimCurveQuat.h>
#include <moai-sim/MOAIAnimCurveVec.h>
#include <moai-sim/MOAIAttribute.h>
#include <moai-sim/ZLBlendMode.h>
//#include <moai-sim/MOAIBoundsDeck.h>
//#include <moai-sim/MOAIBspBuilder2D.h>
#include <moai-sim/MOAIButtonSensor.h>
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAICameraAnchor2D.h>
#include <moai-sim/MOAICameraFitter2D.h>
#include <moai-sim/MOAICollisionConsts.h>
#include <moai-sim/MOAICollisionDeck.h>
#include <moai-sim/MOAICollisionPrim.h>
#include <moai-sim/MOAICollisionProp.h>
#include <moai-sim/MOAICollisionShape.h>
#include <moai-sim/MOAICollisionWorld.h>
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAICompassSensor.h>
#include <moai-sim/MOAICoroutine.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIDeckHolder.h>
#include <moai-sim/MOAIDeckHolderWithGrid.h>
#include <moai-sim/MOAIDeckHolderWithIndex.h>
#include <moai-sim/MOAIDeckProxy.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIDrawDeck.h>
#include <moai-sim/MOAIDrawShapeImmediate.h>
#include <moai-sim/MOAIDrawShapeRetained.h>
#include <moai-sim/MOAIEaseDriver.h>
#include <moai-sim/MOAIEaseType.h>
#include <moai-sim/MOAIFancyGrid.h>
#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGeometryWriter.h>
#include <moai-sim/MOAIAbstractGfxBuffer.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLGfxPipelineClerk.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/ZLGfxResourceClerk.h>
#include <moai-sim/ZLGfxStateCache.h>
#include <moai-sim/ZLGfxStateCPUCache.h>
#include <moai-sim/ZLGfxStateGPUCache.h>
#include <moai-sim/ZLGfxStateVertexCache.h>
#include <moai-sim/MOAIGraphicsProp.h>
#include <moai-sim/MOAIGraphicsGridProp.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIGridPathGraph.h>
#include <moai-sim/MOAIGridSpace.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIImageFormat.h>
#include <moai-sim/MOAIImageFormatMgr.h>
#include <moai-sim/MOAIImageLoadTask.h>
#include <moai-sim/MOAIImageTexture.h>
#include <moai-sim/MOAIIndexBuffer.h>
#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAIInputMgr.h>
#include <moai-sim/MOAIJoystickSensor.h>
#include <moai-sim/MOAIKeyCode.h>
#include <moai-sim/MOAIKeyCodeEnum.h>
#include <moai-sim/MOAIKeyboardSensor.h>
#include <moai-sim/MOAILayoutFrame.h>
#include <moai-sim/MOAILight.h>
#include <moai-sim/MOAILightFormat.h>
#include <moai-sim/MOAILocationSensor.h>
#include <moai-sim/MOAIMaterial.h>
#include <moai-sim/MOAIMaterialBase.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIMaterialBatchHolder.h>
#include <moai-sim/MOAIMaterialHolder.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIMatrix.h>
#include <moai-sim/MOAIMesh.h>
#include <moai-sim/MOAIMeshPartition.h>
#include <moai-sim/MOAIMeshSparseQuadTree.h>
#include <moai-sim/MOAIMeshTernaryTree.h>
#include <moai-sim/MOAIMetaTileDeck2D.h>
#include <moai-sim/MOAIMotionSensor.h>
#include <moai-sim/MOAIMoveConstraint2D.h>
#include <moai-sim/MOAINode.h>
#include <moai-sim/MOAINodeMgr.h>
#include <moai-sim/MOAIOverlap.h>
#include <moai-sim/MOAIOverlapResolver.h>
#include <moai-sim/MOAIParticle.h>
#include <moai-sim/MOAIParticleCallbackPlugin.h>
#include <moai-sim/MOAIParticleDistanceEmitter.h>
#include <moai-sim/MOAIParticleEmitter.h>
#include <moai-sim/MOAIParticleForce.h>
#include <moai-sim/MOAIParticlePexPlugin.h>
#include <moai-sim/MOAIParticlePlugin.h>
#include <moai-sim/MOAIParticleScript.h>
#include <moai-sim/MOAIParticleState.h>
#include <moai-sim/MOAIParticleSystem.h>
#include <moai-sim/MOAIParticleTimedEmitter.h>
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionCell.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIPartitionViewLayer.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIPath.h>
#include <moai-sim/MOAIPathFinder.h>
#include <moai-sim/MOAIPathStepper.h>
#include <moai-sim/MOAIPathTerrainDeck.h>
#include <moai-sim/MOAIPinTransform.h>
#include <moai-sim/MOAIPointerSensor.h>
//#include <moai-sim/MOAIProfiler.h>
//#include <moai-sim/MOAIProfilerAutoScope.h>
//#include <moai-sim/MOAIProfilerContext.h>
//#include <moai-sim/MOAIProfilerReport.h>
//#include <moai-sim/MOAIProfilerReportBase.h>
//#include <moai-sim/MOAIProfilerReportBox.h>
//#include <moai-sim/MOAIProfilerScope.h>
#include <moai-sim/MOAIProjectionProp.h>
#include <moai-sim/MOAIQuadBrush.h>
#include <moai-sim/MOAIRegion.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIScissorRect.h>
#include <moai-sim/MOAIScriptNode.h>
#include <moai-sim/MOAISelectionMesh.h>
#include <moai-sim/MOAISensor.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/ZLShaderUniform.h>
#include <moai-sim/ZLShaderUniformHandle.h>
#include <moai-sim/ZLAbstractShaderUniformSchema.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAISpanList.h>
#include <moai-sim/MOAISpriteDeck2D.h>
#include <moai-sim/MOAIStretchPatch2D.h>
#include <moai-sim/MOAISurfaceDeck2D.h>
#include <moai-sim/MOAITableLayer.h>
#include <moai-sim/MOAITableViewLayer.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITileDeck2D.h>
#include <moai-sim/MOAITileFlags.h>
#include <moai-sim/MOAITimer.h>
#include <moai-sim/MOAITouchSensor.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVecPathGraph.h>
#include <moai-sim/MOAIVectorCombo.h>
#include <moai-sim/MOAIVectorEllipse.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorPoly.h>
#include <moai-sim/MOAIVectorRect.h>
#include <moai-sim/MOAIVectorSensor.h>
#include <moai-sim/MOAIVectorShape.h>
#include <moai-sim/MOAIVectorStyle.h>
#include <moai-sim/MOAIVectorTesselator.h>
#include <moai-sim/MOAIVectorUtil.h>
#include <moai-sim/MOAIVertexArray.h>
#include <moai-sim/MOAIVertexBuffer.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIViewProj.h>
#include <moai-sim/MOAIWheelSensor.h>

// text stuff here for now
#include <moai-sim/MOAIBitmapFontReader.h>
#include <moai-sim/MOAIDynamicGlyphCache.h>
#include <moai-sim/MOAIDynamicGlyphCachePage.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAIFreeTypeFontReader.h>
#include <moai-sim/MOAIGlyph.h>
#include <moai-sim/MOAIGlyphCache.h>
#include <moai-sim/MOAIGlyphSet.h>
#include <moai-sim/MOAISpanList.h>
#include <moai-sim/MOAIStaticGlyphCache.h>
#include <moai-sim/MOAITextBundle.h>
#include <moai-sim/MOAITextLabel.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextLayoutEngine.h>
#include <moai-sim/MOAITextLayoutRules.h>
#include <moai-sim/MOAITextShaper.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyleCache.h>
#include <moai-sim/MOAITextStyleMap.h>
#include <moai-sim/MOAITextStyleParser.h>

#endif
