----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SCREEN_SCALE = 1/16

camera = MOAITransform.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 960*SCREEN_SCALE, 640*SCREEN_SCALE )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "test", 960, 640 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "building_tile_big.png" )
tileDeck:setSize ( 8, 16 )

grid = dofile ( "testMOAIbig_block_2grid.lua" )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -960 * SCREEN_SCALE, -320 * SCREEN_SCALE )
layer:insertProp ( prop )

--WHAT TO LOOK FOR: This tilemap is meant to be the corner segment of a building.
--The missing tile is the one on the upper-right corner of the segment.