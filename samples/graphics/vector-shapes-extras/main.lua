--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

load = function ( filename )
	local file = assert ( io.open ( filename, mode ))
	local bytes = file:read ( '*all' )
	file:close ()
	return bytes
end

writeFormat = function ( formatStr, ... )
	local stream = MOAIMemStream.new ()
	stream:open ( 32 )
	stream:writeFormat ( formatStr, ... )
	return stream:getString ()
end

vtxFormat = MOAIVertexFormat.new ()
vtxFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
vtxFormat:declareColor ( 2, MOAIVertexFormat.GL_UNSIGNED_BYTE, 4 )
vtxFormat:declareAttribute ( 3, MOAIVertexFormat.GL_FLOAT, 1 )

shader = MOAIShader.new ()

shader:setVertexAttribute ( 1, 'position' )
shader:setVertexAttribute ( 2, 'color' )
shader:setVertexAttribute ( 3, 'blend' )

shader:reserveUniforms ( 4 )
shader:declareUniform ( 1, 'transform', MOAIShader.UNIFORM_WORLD_VIEW_PROJ )
shader:declareUniform ( 2, 'ucolor', MOAIShader.UNIFORM_PEN_COLOR )
shader:declareUniform ( 3, 'color0', MOAIShader.UNIFORM_COLOR )
shader:declareUniform ( 4, 'color1', MOAIShader.UNIFORM_COLOR )

shader:load ( load ( 'shader.vsh' ), load ( 'shader.fsh' ))

color0 = MOAIColor.new ()
color0:setColor ( 1, 0, 0, 0 )
shader:setAttrLink ( 3, color0, MOAIColor.COLOR_TRAIT )

color1 = MOAIColor.new ()
color1:setColor ( 0, 1, 0, 0 )
shader:setAttrLink ( 4, color1, MOAIColor.COLOR_TRAIT )

MOAISim.openWindow ( "test", 640, 480 )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- make a tesselator
tess = MOAIVectorTesselator.new ()

-- write 4 bytes of 'extra' per vertex
tess:reserveVertexExtras ( 2, 4 )
tess:setVertexExtra ( 1, writeFormat ( 'f', 0.0 )) -- fill color
tess:setVertexExtra ( 2, writeFormat ( 'f', 1.0 )) -- stroke color

tess:setCircleResolution ( 32 )

tess:setFillStyle ( MOAIVectorDrawing.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

tess:setStrokeStyle ( MOAIVectorDrawing.STROKE_EXTERIOR )
tess:setStrokeColor ( 0.45, 0.5, 1, 1  )
tess:setStrokeWidth ( 10 )
tess:setJoinStyle ( MOAIVectorDrawing.JOIN_MITER )
tess:setCapStyle ( MOAIVectorDrawing.CAP_POINTY )
tess:setMiterLimit ( 10 )

tess:setFillExtra ( 1 )
tess:setStrokeExtra ( 2 )

tess:pushCombo ()
	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )
	tess:pushPoly ( -50, 100, 50, 100, 0, 0 )
	tess:pushPoly ( 50, -100, -50, -100, 0, 0 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 0, 0, -100, 50, -100, -50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
tess:finish ()

tess:finish ()

-- yank a mesh out of the tess

local vtxBuffer = MOAIVertexBuffer.new ()
local idxBuffer = MOAIIndexBuffer.new ()

tess:getTriangles ( vtxBuffer, idxBuffer );

vtxBuffer:setFormat ( vtxFormat )
vtxBuffer:bless ()

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer )
mesh:setIndexBuffer ( idxBuffer )
mesh:setShader ( shader )

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )
