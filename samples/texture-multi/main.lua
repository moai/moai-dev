----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

file = assert ( io.open ( 'shader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'shader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

texture1 = MOAITexture.new ()
texture1:load ( "moai.png" )

texture2 = MOAITexture.new ()
texture2:load ( "numbers.png" )
texture2:setWrap ( true )

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 2 )
program:declareUniform ( 1, 'sampler0', MOAIShaderProgram.UNIFORM_TYPE_INT )
program:declareUniform ( 2, 'sampler1', MOAIShaderProgram.UNIFORM_TYPE_INT )
--program:declareUniformSampler ( 1, 'sampler0', 1 )
--program:declareUniformSampler ( 2, 'sampler1', 2 )

program:reserveTextures ( 2 )
program:setTexture ( 1, texture1, 1 )
program:setTexture ( 2, texture2, 2 )

program:load ( vsh, fsh )

shader = MOAIShader.new ()
shader:setProgram ( program )

shader:setUniform ( 1, 0 )
shader:setUniform ( 2, 1 )

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( shader )
prop:moveRot ( 0, 0, 360, 1.5 )
prop:moveScl ( 1.5, 1.5, 0, 1.5 )
prop:setPartition ( layer )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setTexture ( texture1 )
prop:moveLoc ( 0, -128, 0, 1.5 )
prop:moveRot ( 0, 0, 360, 1.5 )
prop:setPartition ( layer )
