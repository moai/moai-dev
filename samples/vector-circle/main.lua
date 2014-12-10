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

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )
tess:setCircleResolution ( 32 )

tess:pushEllipse ( 0, 0, 100 )
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
