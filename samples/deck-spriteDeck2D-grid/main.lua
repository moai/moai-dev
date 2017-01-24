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

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000001 )
grid:setRow ( 2,	0x40000001, 0x60000001 )

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )

prop = MOAIGraphicsGridProp.new ()
prop:setDeck ( spriteDeck )
prop:setGrid ( grid )
prop:setLoc ( -64, -64, 0 )
prop:setLayer ( layer )
