--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )

gFrameBuffer = MOAIGfxDevice.getFrameBuffer ()
gFrameBuffer:setClearDepth ( true )
gFrameBuffer:setClearColor ( 1, 1, 1, 1 )

gViewport = MOAIViewport.new ()
gViewport:setSize ( 640, 480 )
gViewport:setScale ( 640, 480 )

gWorldLayer = MOAILayer2D.new ()
gWorldLayer:setViewport ( gViewport )
MOAISim.pushRenderPass ( gWorldLayer )

gCameraPivot = MOAITransform.new ()
gCameraSpin = MOAITransform.new ()

gCamera = MOAICamera.new ()
gCamera:setLoc ( 0, 0, gCamera:getFocalLength ( 640 ))
gWorldLayer:setCamera ( gCamera )

gCameraPivot:setAttrLink ( MOAIProp2D.INHERIT_TRANSFORM, gCameraSpin, MOAIProp2D.TRANSFORM_TRAIT )
gCamera:setAttrLink ( MOAIProp2D.INHERIT_TRANSFORM, gCameraPivot, MOAIProp2D.TRANSFORM_TRAIT )

gCameraPivot:setRot ( 80, 0, 0 )
--gCameraSpin:setRot ( 0, 0, -160 )
gCameraSpin:moveRot ( 0, 0, -360, 5 )

tess = MOAIVectorTesselator.new ()

tess:setCircleResolution ( 32 )

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

tess:setLightVec ( -1, -1 )

tess:setLightColor ( 1, 1, 1, 1 )
tess:setLightCurve ( MOAIEaseType.LINEAR ) -- SHARP_EASE_OUT

tess:setShadowColor ( 0, 0, 0, 0.5 )
tess:setShadowCurve ( MOAIEaseType.LINEAR )

tess:setExtrude ( 100 )
--tess:pushRect ( -50, -50, 50, 50 )
tess:pushEllipse ( 0, 0, 100 )
tess:setExtrude ( 0 )

tess:setStrokeStyle ( MOAIVectorTesselator.STROKE_EXTERIOR )
tess:setStrokeColor ( 0.45, 0.5, 1, 1  )
tess:setStrokeWidth ( 50 )
tess:setJoinStyle ( MOAIVectorTesselator.JOIN_ROUND )
tess:setCapStyle ( MOAIVectorTesselator.CAP_POINTY )
tess:setMiterLimit ( 10 )

--tess:pushRect ( -50, -50, 50, 50 )

--tess:pushEllipse ( 0, 0, 100 )

--tess:pushPath ( -50, -50, 50, 50 )
--tess:pushPath ( 50, -50, 50, 50, -50, 50, -50, -50 )
--tess:pushPath ( 50, -50, -50, -50, -50, 50, 50, 50 )
--tess:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
--tess:pushPolygon ( 50, -50, 0, 50, -50, -50 )

--[[
tess:pushCombo ()
	tess:pushPolygon ( -50, -150, -150, -150, -150, -50, -50, -50 )
	tess:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPolygon ( 150, 50, 50, 50, 50, 150, 150, 150 )
tess:finish ()
]]--

--[[
tess:pushCombo ()
	tess:pushPolygon ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPolygon ( -50, 100, 50, 100, 0, 0 )
	tess:pushPolygon ( 50, -100, -50, -100, 0, 0 )
	tess:pushPolygon ( 0, 0, 100, -50, 100, 50 )
	tess:pushPolygon ( 0, 0, -100, 50, -100, -50 )
	tess:pushPolygon ( 25, -25, -25, -25, -25, 25, 25, 25 )
tess:finish ()
]]--

--[[
tess:pushPolygon ()
	tess:pushVertex ( 175, 175 )
	tess:pushVertex ( 175, -175 )
	tess:pushVertex ( -175, -175 )
	tess:pushVertex ( -175, 175 )
	tess:pushVertex ( 75, 175 )
	tess:pushVertex ( 75, -75 )
	tess:pushVertex ( -75, -75 )
	tess:pushVertex ( -75, 75 )
	tess:pushVertex ( -25, 75 )
	tess:pushVertex ( -25, -25 )
	tess:pushVertex ( 25, -25 )
	tess:pushVertex ( 25, 125 )
	tess:pushVertex ( -125, 125 )
	tess:pushVertex ( -125, -125 )
	tess:pushVertex ( 125, -125 )
	tess:pushVertex ( 125, 175 )
tess:finish ()
]]--

tess:finish ()

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

local vtxBuffer = MOAIGfxBuffer.new ()
local idxBuffer = MOAIGfxBuffer.new ()
local totalElements = tess:getTriangles ( vtxBuffer, idxBuffer, 2 );

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
mesh:setIndexBuffer ( idxBuffer )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
mesh:setTotalElements ( totalElements )
mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setCullMode ( MOAIProp.CULL_BACK )
prop:setDepthTest ( MOAIProp.DEPTH_TEST_LESS_EQUAL )

gWorldLayer:insertProp ( prop )
