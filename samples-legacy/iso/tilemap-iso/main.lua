----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 724, 512 )

layer = MOAILayer2D.new ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 724, 512 )
viewport:setScale ( 724, 512 )
layer:setViewport ( viewport )

camera = MOAICamera.new ()
camera:setOrtho ( true )
camera:setNearPlane ( 10000 )
camera:setFarPlane ( -10000 )
camera:setRot ( 60, 0, 45 )
layer:setCamera ( camera )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "diamond-tiles.png" )
tileDeck:setSize ( 4, 4 )
tileDeck:setUVQuad ( 0, 0.5, 0.5, 0, 0, -0.5, -0.5, 0 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 64, 64 )

grid:setRow ( 1, 	0x01, 0x02, 0x03, 0x04, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 2, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 3, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 4, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 5, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 6, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 7, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 8, 	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 )

prop = MOAIProp.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setPiv ( 256, 256 )
prop:setScl ( 1, -1 )
prop:moveRot ( 0, 0, 360, 3 )
layer:insertProp ( prop )

