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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

shader = MOAISimpleShader.new ()
shader:setBlendMode ( MOAISimpleShader.GL_ONE, MOAISimpleShader.GL_ONE_MINUS_SRC_ALPHA )
shader:setColor ( 0, 0, 0, 0 )
shader:seekColor ( 1, 0, 0, 1, 1.5 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setShader ( shader )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

