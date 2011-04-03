----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

camera = MOAITransform.new ()
scene:setCamera ( camera )

brushlib = MOAIGfxQuadDeck2D.new ()
brushlib:setTexture ( "cathead.png" )

brushlib:reserveBrushes ( 1 )
brushlib:setRect ( 1, -32, -32, 32, 32 )
brushlib:setUVRect ( 1, 0, 1, 1, 0 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 64, 64 )

grid:setTile ( 1, 1, 0x40000001 )
grid:setTile ( 2, 1, 0x00000001 )
grid:setTile ( 1, 2, 0x00000001 )
grid:setTile ( 2, 2, 0x00000001 )

tilemap = MOAIProp2D.new ()
tilemap:setDeck ( brushlib )
tilemap:setGrid ( grid )
tilemap:setRepeat ( true )
partition:insertProp ( tilemap )

--tilemap:moveRot ( 360, 1.5 )
--tilemap:moveLoc ( -320, 0, 3 )

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )
