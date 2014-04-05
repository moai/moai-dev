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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )

color = MOAIColor.new ()
color:setColor ( 0, 0, 0, 0 )
color:moveColor ( 1, 0, 0, 0, 1.5 )

shader = MOAIShader.new ()
shader:reserveUniforms ( 1 )
shader:declareUniform ( 1, 'maskColor', MOAIShader.UNIFORM_COLOR )

shader:setAttrLink ( 1, color, MOAIColor.COLOR_TRAIT )

shader:setVertexAttribute ( 1, 'position' )
shader:setVertexAttribute ( 2, 'uv' )
shader:setVertexAttribute ( 3, 'color' )
shader:load ( vsh, fsh )

gfxQuad:setShader ( shader )
