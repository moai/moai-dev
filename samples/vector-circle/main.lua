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
layer:insertProp ( prop )
