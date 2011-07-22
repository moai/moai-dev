----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

file = assert ( io.open ( 'shader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'shader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

--transform = MOAITransform.new ()
--transform:moveLoc ( 1, 0, 3 )
--transform:moveRot ( 360, 3 )

program = MOAIGlslProgram.new ()

program:reserveUniforms ( 1 )
program:declareUniform ( 1, 'transform', MOAIGlslProgram.UNIFORM_VIEW_PROJ )

program:setVertexAttribute ( 0, 'position' )
program:setVertexAttribute ( 2, 'color' )

program:load ( vsh, fsh )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( program )
layer:insertProp ( prop )

--prop:moveRot ( 360, 1.5 )
