----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 768, 768 )

layer = MOAILayer2D.new ()
layer:setSortMode ( MOAILayer2D.SORT_Y_ASCENDING )
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 768, 768 )
viewport:setScale ( 768, -768 )
layer:setViewport ( viewport )

camera = MOAITransform.new ()
layer:setCamera ( camera )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "iso-tiles.png" )
tileDeck:setSize ( 4, 4 )
tileDeck:setRect ( -1, 1, 1, -3 )

grid = MOAIGrid.new ()
grid:setSize ( 5, 8, 128, 64 )

grid:setRow ( 1, 	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 )
grid:setRow ( 2, 	0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000 )
grid:setRow ( 3, 	0x00000000, 0x00000000, 0x00000000, 0x00000002, 0x00000000 )
grid:setRow ( 4, 	0x00000002, 0x00000000, 0x00000000, 0x00000000, 0x00000002 )
grid:setRow ( 5, 	0x00000000, 0x00000002, 0x00000000, 0x00000002, 0x00000000 )
grid:setRow ( 6, 	0x00000000, 0x00000000, 0x00000002, 0x00000000, 0x00000000 )
grid:setRow ( 7, 	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 )
grid:setRow ( 8, 	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 )

map = MOAIProp2D.new ()
map:setDeck ( tileDeck )
map:setGrid ( grid )
map:setLoc ( -320, -256 )
map:setScl ( 1, 1 )
map:setExpandForSort ( true )
layer:insertProp ( map )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setIndex ( 4 )
prop:setScl ( 128, 64 )
prop:setLoc ( 0, -256 )
prop:moveLoc ( 0, 512, 5 )
layer:insertProp ( prop )

