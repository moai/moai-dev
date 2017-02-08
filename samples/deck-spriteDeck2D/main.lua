----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( spriteDeck )
prop:setPartition ( layer )
