----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- texture names are just integers.
-- use them to attach textures to named slots in a material.
-- shaders may then refer to textures by name, instead of directly.
-- this is useful when you want to reuse the same shader across
-- many materials.

TEXTURE_NAME_1 = 1
TEXTURE_NAME_2 = 2

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

-- here we are going to bind textures to the shader program by name. another form of setTexture ()
-- accepts a texture directly. you cal also specify a name, a texture unit and a fallback. the
-- fallback texture will be used if no texture is provided at the named slot in the material.

program:reserveTextures ( 2 )
program:setTexture ( 1, TEXTURE_NAME_1, 1 ) -- index, texture name, texture unit: bind texture named '1' (in material) to texture unit 1
program:setTexture ( 2, TEXTURE_NAME_2, 2 ) -- index, texture name, texture unit: bind texture named '2' (in material) to texture unit 2

program:load ( vsh, fsh )

shader = MOAIShader.new ()
shader:setProgram ( program )

shader:setUniform ( 1, 0 )
shader:setUniform ( 2, 1 )

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

-- since MOAIProp () is also a matrial batch holder, we can setTexture () on it. in the example
-- below we will attach our two textures to the names our shader will look for in. these will
-- be attached in the first material in the batch (material index 1).

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( shader ) -- this shader expects two textures
prop:setTexture ( 1, TEXTURE_NAME_1, texture1 ) -- material index, texture name, texture: attach texture1 to texture name '1' in first material
prop:setTexture ( 1, TEXTURE_NAME_2, texture2 ) -- material index, texture name, texture: attach texture2 to texture name '2' in first material
prop:moveRot ( 0, 0, 360, 1.5 )
prop:moveScl ( 1.5, 1.5, 0, 1.5 )
prop:setPartition ( layer )

-- not using the special shader here. in addition to named textures, each material has
-- a "default texture" that always gets loaded into texture unit 1. the call to setTexture ()
-- below sets the default texture for material index 1. the default shader used by MOAISpriteDeck2D
-- does not look for any named textures; it simply uses whatever is loaded into texture unit 1.

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setTexture ( texture1 )
prop:moveLoc ( 0, -128, 0, 1.5 )
prop:moveRot ( 0, 0, 360, 1.5 )
prop:setPartition ( layer )
