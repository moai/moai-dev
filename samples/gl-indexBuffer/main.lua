----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

function makeCube ( size )
	
	local vertexFormat = MOAIVertexFormat.new ()
	vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
	vertexFormat:declareColor ( 2, MOAIVertexFormat.GL_UNSIGNED_BYTE )

	local vbo = MOAIGfxBuffer.new ()
	vbo:setTarget ( MOAIGfxBuffer.VERTEX_BUFFER )
	vbo:reserve ( 8 * vertexFormat:getVertexSize ())

	-- 0: top back left
	vbo:writeFloat ( -size, size, -size )
	vbo:writeColor32 ( 1, 1, 0.5 )

	-- 1: top back right
	vbo:writeFloat ( size, size, -size )
	vbo:writeColor32 ( 1, 1, 0.75 )

	-- 2: top front right
	vbo:writeFloat ( size, size, size )
	vbo:writeColor32 ( 1, 0.5, 0.25 )

	-- 3: top front left
	vbo:writeFloat ( -size, size, size )
	vbo:writeColor32 ( 1, 0.5, 0 )

	-- 4: bottom back left
	vbo:writeFloat ( -size, -size, -size )
	vbo:writeColor32 ( 1, 0.5, 0.5 )

	-- 5: bottom back right
	vbo:writeFloat ( size, -size, -size )
	vbo:writeColor32 ( 1, 0.5, 0.75 )

	-- 6: bottom front right
	vbo:writeFloat ( size, -size, size )
	vbo:writeColor32 ( 1, 0, 0.25 )

	-- 7: bottom front left
	vbo:writeFloat ( -size, -size, size )
	vbo:writeColor32 ( 1, 0, 0 )

	local ibo = MOAIGfxBuffer.new ()
	ibo:setTarget ( MOAIGfxBuffer.INDEX_BUFFER )
	ibo:reserve ( 36 * 2 )

	-- front
	ibo:writeU16 ( 2 )
	ibo:writeU16 ( 3 )
	ibo:writeU16 ( 7 )
	ibo:writeU16 ( 7 )
	ibo:writeU16 ( 6 )
	ibo:writeU16 ( 2 )

	-- right
	ibo:writeU16 ( 1 )
	ibo:writeU16 ( 2 )
	ibo:writeU16 ( 6 )
	ibo:writeU16 ( 6 )
	ibo:writeU16 ( 5 )
	ibo:writeU16 ( 1 )

	-- back
	ibo:writeU16 ( 0 )
	ibo:writeU16 ( 1 )
	ibo:writeU16 ( 5 )
	ibo:writeU16 ( 5 )
	ibo:writeU16 ( 4 )
	ibo:writeU16 ( 0 )

	-- left
	ibo:writeU16 ( 3 )
	ibo:writeU16 ( 0 )
	ibo:writeU16 ( 4 )
	ibo:writeU16 ( 4 )
	ibo:writeU16 ( 7 )
	ibo:writeU16 ( 3 )

	-- top
	ibo:writeU16 ( 1 )
	ibo:writeU16 ( 0 )
	ibo:writeU16 ( 3 )
	ibo:writeU16 ( 3 )
	ibo:writeU16 ( 2 )
	ibo:writeU16 ( 1 )

	-- bottom
	ibo:writeU16 ( 7 )
	ibo:writeU16 ( 4 )
	ibo:writeU16 ( 5 )
	ibo:writeU16 ( 5 )
	ibo:writeU16 ( 6 )
	ibo:writeU16 ( 7 )

	local mesh = MOAIMesh.new ()

	mesh:setVertexBuffer ( vbo, vertexFormat )
	mesh:setIndexBuffer ( ibo )

	mesh:setTotalElements ( 36 )
	mesh:setBounds ( vbo:computeBounds ( vertexFormat ))

	mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )
	mesh:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.LINE_SHADER_3D ))
	
	return mesh
end

local mesh = makeCube ( 64 )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 360, 0, 6 )
prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
layer:insertProp ( prop )
