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

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'color' )
program:setVertexAttribute ( 3, 'blend' )

program:reserveUniforms ( 4 )
program:declareUniform ( 1, 'transform', MOAIShaderProgram.UNIFORM_MATRIX_F4 )
program:declareUniform ( 2, 'ucolor', MOAIShaderProgram.UNIFORM_VECTOR_F4 )
program:declareUniform ( 3, 'color0', MOAIShaderProgram.UNIFORM_VECTOR_F4 )
program:declareUniform ( 4, 'color1', MOAIShaderProgram.UNIFORM_VECTOR_F4 )

program:reserveGlobals ( 2 )
program:setGlobal ( 1, 1, MOAIShaderProgram.GLOBAL_WORLD_VIEW_PROJ )
program:setGlobal ( 2, 2, MOAIShaderProgram.GLOBAL_PEN_COLOR )

program:load ( load ( 'shader.vsh' ), load ( 'shader.fsh' ))

shader = MOAIShader.new ()
shader:setProgram ( program )

color0 = MOAIColor.new ()
color0:setColor ( 1, 0, 0, 0 )
color0:moveColor ( 1, 0, 0, 1, 2 )
shader:setAttrLink ( 3, color0, MOAIColor.COLOR_TRAIT )

color1 = MOAIColor.new ()
color1:setColor ( 0, 1, 0, 0 )
color1:moveColor ( 0, 1, 0, 1, 2 )
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

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

tess:setStrokeStyle ( MOAIVectorTesselator.STROKE_EXTERIOR )
tess:setStrokeColor ( 0.45, 0.5, 1, 1  )
tess:setStrokeWidth ( 10 )
tess:setJoinStyle ( MOAIVectorTesselator.JOIN_MITER )
tess:setCapStyle ( MOAIVectorTesselator.CAP_POINTY )
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

local vtxBuffer = MOAIGfxBuffer.new ()
local idxBuffer = MOAIGfxBuffer.new ()
local totalElements = tess:getTriangles ( vtxBuffer, idxBuffer, 2 );

local mesh = MOAIMesh.new ()
mesh:setVertexBuffer ( vtxBuffer, vtxFormat )
mesh:setIndexBuffer ( idxBuffer )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
mesh:setShader ( shader )
mesh:setTotalElements ( totalElements )
mesh:setBounds ( vtxBuffer:computeBounds ( vtxFormat ))

local prop = MOAIProp.new ()
prop:setDeck ( mesh )
layer:insertProp ( prop )

prop:moveRot ( 0, 0, -180, 1.5 )