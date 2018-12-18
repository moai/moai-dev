--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
layer:pushRenderPass ()

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

writer = tess:openWriter ()

stream = MOAIMemStream.new ()
tess:writeShapes ( stream, writer )
stream:seek ( 0 )

tess = MOAIVectorTesselator.new ()
tess:readShapes ( stream )

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

local vtxBuffer = MOAIVertexBuffer.new ()
local idxBuffer = MOAIIndexBuffer.new ()

local totalElements = tess:tesselate ( vtxBuffer, idxBuffer, 2, vtxFormat );

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
mesh:setIndexBuffer ( idxBuffer, 2 )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
mesh:setTotalElements ( totalElements )
mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setPartition ( layer )
