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
color = MOAIColor.new ()
color:setColor ( 1, 0, 1, 1 )

shader:reserveUniforms ( 2 )
shader:declareUniform ( 1, 'transform', MOAIShader.UNIFORM_WORLD_VIEW_PROJ )
shader:declareUniform ( 2, 'maskColor', MOAIShader.UNIFORM_COLOR )

shader:setUniform ( 2, color )

shader:setVertexAttribute ( 1, 'position' )
shader:setVertexAttribute ( 2, 'uv' )
shader:setVertexAttribute ( 3, 'color' )

shader:load ( vsh, fsh )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( shader )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )
