----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAICamera.new ()

camera:moveLoc ( 128, 0, 0, 3 )
camera:moveRot ( 0, 0, 180, 3 )
camera:moveScl ( 1, 1, 0, 3 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
layer:pushRenderPass ()

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( spriteDeck )
prop:setPartition ( layer )

