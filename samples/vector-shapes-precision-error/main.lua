--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

-- this sample is meant to repro a visual glitch that happens in libtess2 due to
-- floating point overflow. by default MOAIVectorTesselator will quantize all
-- floating point values to three decimal places. in this sample we turn quantization
-- off to show the glitch.

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
layer:pushRenderPass ()

-- make a tesselator
tess = MOAIVectorTesselator.new ()

tess:setPrecision ( 0 ) -- this causes a floating point precision error in libtess2

tess:setFillStyle ( MOAIVectorTesselator.FILL_NONE )

tess:setStrokeStyle ( MOAIVectorTesselator.STROKE_EXTERIOR )
tess:setStrokeColor ( 0.45, 0.5, 1, 1  )
tess:setStrokeWidth ( 15 )
tess:setJoinStyle ( MOAIVectorTesselator.JOIN_MITER )
tess:setCapStyle ( MOAIVectorTesselator.CAP_POINTY )
tess:setMiterLimit ( 10 )

tess:pushCombo ()
	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
tess:finish ()

-- yank a mesh out of the tess
local vtxFormat = MOAIVertexFormatMgr.getFormat ( MOAIVertexFormatMgr.XYZC )

local vtxBuffer = MOAIVertexBuffer.new ()
local idxBuffer = MOAIIndexBuffer.new ()

local totalElements = tess:tesselate ( vtxBuffer, idxBuffer, 2, vtxFormat )

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
mesh:setIndexBuffer ( idxBuffer )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
mesh:setTotalElements ( totalElements )
mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setPartition ( layer )

maxElements = totalElements / 3
showElements = 0

function clickCallback ( down )
    
    if down then
        showElements = showElements == maxElements and 1 or showElements + 1
        mesh:setTotalElements ( showElements * 3 )
    end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
