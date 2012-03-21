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

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "test.png" )
tileDeck:setSize ( 2, 2 )
tileDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
layer:insertProp ( prop )
