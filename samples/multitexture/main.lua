----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

file = assert ( io.open ( 'shader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'shader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

shader = MOAIShader.new ()

shader:reserveUniforms ( 2 )
shader:declareUniformSampler ( 1, 'sampler0', 1 )
shader:declareUniformSampler ( 2, 'sampler1', 2 )

shader:setVertexAttribute ( 1, 'position' )
shader:setVertexAttribute ( 2, 'uv' )
shader:setVertexAttribute ( 3, 'color' )
shader:load ( vsh, fsh )

multitexture = MOAIMultiTexture.new ()
multitexture:reserve ( 2 )

cathead = MOAITexture.new ()
cathead:load ( "moai.png" )
multitexture:setTexture ( 1, cathead )

numbers = MOAITexture.new ()
numbers:load ( "numbers.png" )
numbers:setWrap ( true )
multitexture:setTexture ( 2, numbers )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( multitexture )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( shader )
prop:moveRot ( 360, 1.5 )
prop:moveScl ( 1.5, 1.5, 1.5 )
layer:insertProp ( prop )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setTexture ( cathead )
prop:moveLoc ( 0, -128, 1.5 )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )


