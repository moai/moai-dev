----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, -512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAITexture.new ()
texture:setFilter ( MOAITexture.GL_LINEAR_MIPMAP_LINEAR ) -- set the filter *before* loading the texture!
texture:load ( "moai.png" )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:setScl ( 0, 0 )
prop:moveScl ( 4, 4, 6, MOAIEaseType.LINEAR )
