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

gfxQuadDeck = MOAIGfxQuadDeck2D.new ()
gfxQuadDeck:setTexture ( "moai.png" )
gfxQuadDeck:reserve ( 1 )
gfxQuadDeck:setRect ( 1, -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadDeck )
layer:insertProp ( prop )

