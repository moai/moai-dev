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

grid = MOAIFancyGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000001 )
grid:setRow ( 2,	0x40000001, 0x60000001 )

grid:setColorRow ( 1,	0xff0000ff, 0xff00ff00 )
grid:setColorRow ( 2,	0xffff0000, 0xff00ffff )

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )

prop = MOAIGraphicsGridProp.new ()
prop:setDeck ( spriteDeck )
prop:setGrid ( grid )
prop:setLoc ( -64, -64, 0 )
prop:setPartition ( layer )
