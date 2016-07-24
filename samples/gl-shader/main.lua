----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc.
-- All Rights Reserved.
-- http://getmoai.com
----------------------------------------------------------------

require ( 'cube' )
require ( 'matrix' )

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

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

local program = MOAIShaderProgram.new ()

program:setVertexAttribute( 1, 'position' )
program:setVertexAttribute( 2, 'normal' )
program:setVertexAttribute( 3, 'uv' )
program:setVertexAttribute( 4, 'color' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'world', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4 )
program:declareUniform ( 2, 'viewProj', MOAIShaderProgram.UNIFORM_TYPE_FLOAT, MOAIShaderProgram.UNIFORM_WIDTH_MATRIX_4X4 )

program:reserveGlobals ( 1 )
program:setGlobal ( 1, 2, MOAIShaderProgram.GLOBAL_VIEW_PROJ )

program:load ( MOAIFileSystem.loadFile ( 'shader.vsh' ), MOAIFileSystem.loadFile ( 'shader.fsh' ))

local shader = MOAIShader.new ()
shader:setProgram ( program )

local mesh = cube.makeCube ( 128, 'moai.png' )
mesh:setShader ( shader )
mesh:setTexture ( 'moai.png' )

prop = MOAIProp.new ()
prop:setDeck ( mesh )
prop:setCullMode ( MOAIProp.CULL_BACK )
layer:insertProp ( prop )

local m = matrix.create ()
local i = 0

main = function ()

	while true do
		m:set ( matrix.rotateX ( i ))
		m:append ( matrix.rotateY ( i ))
		shader:setUniform ( 1, m:get ())
		i = ( i + 1 ) % 360
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )
