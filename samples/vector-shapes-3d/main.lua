--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )

gViewport = MOAIViewport.new ()
gViewport:setSize ( 640, 480 )
gViewport:setScale ( 640, 480 )

gWorldLayer = MOAIPartitionViewLayer.new ()
gWorldLayer:setViewport ( gViewport )
gWorldLayer:setClearDepth ( true )
gWorldLayer:setClearColor ( 1, 1, 1, 1 )
gWorldLayer:pushRenderPass ()

gCameraPivot = MOAITransform.new ()
gCameraSpin = MOAITransform.new ()

gCamera = MOAICamera.new ()
gCamera:setType ( MOAICamera.CAMERA_TYPE_3D )
gCamera:setLoc ( 0, 100, gCamera:getFocalLength ( 320 ))
gCamera:lookAt ( 0, 0, 0 )
gWorldLayer:setCamera ( gCamera )

gCameraPivot:setAttrLink ( MOAIProp.INHERIT_TRANSFORM, gCameraSpin, MOAIProp.TRANSFORM_TRAIT )
gCamera:setAttrLink ( MOAIProp.INHERIT_TRANSFORM, gCameraPivot, MOAIProp.TRANSFORM_TRAIT )

gCameraPivot:setRot ( 80, 0, 0 )
gCameraSpin:moveRot ( 0, 0, 360, 5 )

tess = MOAIVectorTesselator.new ()

tess:setCircleResolution ( 32 )

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )
tess:setCircleResolution ( 32 )

tess:setLightVec ( -1, -1 )

tess:setLightColor ( 1, 1, 1, 1 )
tess:setLightCurve ( MOAIEaseType.LINEAR )

tess:setShadowColor ( 0, 0, 0, 0.5 )
tess:setShadowCurve ( MOAIEaseType.LINEAR )

tess:setExtrude ( 20 )

tess:pushCombo ()
	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPoly ( -50, 100, 50, 100, 0, 0 )
	tess:pushPoly ( 50, -100, -50, -100, 0, 0 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 0, 0, -100, 50, -100, -50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
tess:finish ()

local vtxStream = MOAIMemStream.new ()
local idxStream = MOAIMemStream.new ()

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

tess:tesselate ( vtxStream, idxStream, vtxFormat )

vtxStream:seek ()
idxStream:seek ()

local mesh = MOAIGeometryWriter.getMesh ( vtxFormat, vtxStream, vtxStream:getLength (), idxStream, idxStream:getLength ())
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setCullMode ( MOAIProp.CULL_BACK )
prop:setDepthTest ( MOAIProp.DEPTH_TEST_LESS_EQUAL )
prop:setPartition ( gWorldLayer )
