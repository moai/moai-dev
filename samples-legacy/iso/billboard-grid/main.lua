----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAICamera2D.new ()
camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
camera:moveRot ( 0, 0, -360, 3 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000002 )
grid:setRow ( 2,	0x40000003, 0x60000004 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "test.png" )
tileDeck:setSize ( 2, 2 )

prop = MOAIProp.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setPiv ( 64, 64, 0 )
prop:setBillboard ( true )
layer:insertProp ( prop )
