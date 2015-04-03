----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

vertexFormat = MOAIVertexFormat.new ()

-- Moai's default shaders expect loc, uv, color
vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 2 )
vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

vbo = MOAIGfxBuffer.new ()
vbo:reserve ( 4 * vertexFormat:getVertexSize ())

vbo:writeFloat ( -64, -64 )
vbo:writeFloat ( 0, 1 )
vbo:writeColor32 ( 1, 0, 0 )

vbo:writeFloat ( 64, -64 )
vbo:writeFloat ( 1, 1 )
vbo:writeColor32 ( 1, 1, 0 )

vbo:writeFloat ( 64, 64 )
vbo:writeFloat ( 1, 0 )
vbo:writeColor32 ( 0, 1, 0 )

vbo:writeFloat ( -64, 64 )
vbo:writeFloat ( 0, 0 )
vbo:writeColor32 ( 0, 0, 1 )

mesh = MOAIMesh.new ()

mesh:setVertexBuffer ( vbo, vertexFormat )
mesh:setTotalElements ( vbo:countElements ( vertexFormat ))
mesh:setBounds ( vbo:computeBounds ( vertexFormat ))

mesh:setTexture ( "moai.png" )
mesh:setPrimType ( MOAIMesh.GL_TRIANGLE_FAN )

if MOAIGfxDevice.isProgrammable () then

	file = assert ( io.open ( 'shader.vsh', mode ))
	vsh = file:read ( '*all' )
	file:close ()

	file = assert ( io.open ( 'shader.fsh', mode ))
	fsh = file:read ( '*all' )
	file:close ()

	local program = MOAIShaderProgram.new ()

	program:setVertexAttribute ( 1, 'position' )
	program:setVertexAttribute ( 2, 'uv' )
	program:setVertexAttribute ( 3, 'color' )
	
	program:reserveUniforms ( 1 )
	program:declareUniform ( 1, 'transform', MOAIShaderProgram.UNIFORM_MATRIX_F4 )
	
	program:reserveGlobals ( 1 )
	program:setGlobal ( 1, 1, MOAIShaderProgram.GLOBAL_WORLD_VIEW_PROJ )
	
	program:load ( vsh, fsh )
	
	local shader = MOAIShader.new ()
	shader:setProgram ( program )
	
	mesh:setShader ( shader )
end

prop = MOAIProp2D.new ()
prop:setDeck ( mesh )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )

