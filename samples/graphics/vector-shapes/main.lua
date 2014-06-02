--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

tess = MOAIVectorTesselator.new ()

tess:setCircleResolution ( 32 )

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

tess:setStrokeStyle ( MOAIVectorTesselator.STROKE_EXTERIOR )
tess:setStrokeColor ( 0.45, 0.5, 1, 1  )
tess:setStrokeWidth ( 10 )
tess:setJoinStyle ( MOAIVectorTesselator.JOIN_ROUND )
tess:setCapStyle ( MOAIVectorTesselator.CAP_POINTY )
tess:setMiterLimit ( 10 )

tess:pushPoly ()
tess:pushBezierVertices ( -50, -50, -50, 50, 50, 50, 50, -50 )
tess:finish ()

--tess:pushRect ( -50, -50, 50, 50 )

--tess:pushScale ( 1, 0.5 )
tess:pushTranslate ( 50, 100 )
--tess:pushEllipse ( 0, 0, 100 )

--tess:pushPath ( -50, -50, 50, 50 )
--tess:pushPath ( 50, -50, 50, 50, -50, 50, -50, -50 )
--tess:pushPath ( 50, -50, -50, -50, -50, 50, 50, 50 )
--tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
--tess:pushPoly ( 50, -50, 0, 50, -50, -50 )

--[[
tess:pushCombo ()
	tess:pushPoly ( -50, -150, -150, -150, -150, -50, -50, -50 )
	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPoly ( 150, 50, 50, 50, 50, 150, 150, 150 )
tess:finish ()
]]--

--[[
tess:pushCombo ()
	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPoly ( -50, 100, 50, 100, 0, 0 )
	tess:pushPoly ( 50, -100, -50, -100, 0, 0 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 0, 0, -100, 50, -100, -50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
tess:finish ()
]]--

--[[
tess:pushPoly ()
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

local vtxBuffer = MOAIVertexBuffer.new ()
local idxBuffer = MOAIIndexBuffer.new ()

tess:getTriangles ( vtxBuffer, idxBuffer );

vtxBuffer:setFormat ( MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC ))
vtxBuffer:bless ()

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer )
mesh:setIndexBuffer ( idxBuffer )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )
