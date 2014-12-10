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

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000002 )
grid:setRow ( 2,	0x40000003, 0x60000004 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "test.png" )
tileDeck:setSize ( 2, 2 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
layer:insertProp ( prop )
