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

local vtxStream = MOAIMemStream.new ()
local idxStream = MOAIMemStream.new ()

local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

tess = MOAIVectorTesselator.new ()

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )
tess:setCircleResolution ( 32 )

tess:pushEllipse ( -100, -100, 100 )
tess:tesselate ( vtxStream, idxStream, vtxFormat );
tess:clearShapes ()

tess:pushEllipse ( 100, -100, 100 )
tess:tesselate ( vtxStream, idxStream, vtxFormat );
tess:clearShapes ()

tess:pushEllipse ( 100, 100, 100 )
tess:tesselate ( vtxStream, idxStream, vtxFormat );
tess:clearShapes ()

tess:pushEllipse ( -100, 100, 100 )
tess:tesselate ( vtxStream, idxStream, vtxFormat );
tess:clearShapes ()

vtxStream:seek ()
idxStream:seek ()

local mesh = MOAIGeometryWriter.getMesh ( vtxFormat, vtxStream, idxStream, 2 )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )
