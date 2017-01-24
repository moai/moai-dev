----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( '../resources/test.png' )
tileDeck:setSize ( 2, 2 )
tileDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( tileDeck )
prop:setLayer ( layer )
