----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 768, 768 )

layer = MOAILayer2D.new ()
layer:setSortMode ( MOAILayer2D.SORT_VECTOR_ASCENDING )
layer:setSortScale ( 0, 1, 10000 )
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 768, 768 )
viewport:setScale ( 768, -768 )
layer:setViewport ( viewport )

camera = MOAITransform.new ()
layer:setCamera ( camera )

texture = MOAITexture.new ()
texture:load ( "iso-tiles.png" )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( texture )
tileDeck:setSize ( 4, 4 )
tileDeck:setRect ( -1, 1, 1, -3 )

grid = MOAIGrid.new ()
grid:setSize ( 7, 10, 128, 64 )

grid:setRow ( 1, 	0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000 )
grid:setRow ( 2, 	0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001 )
grid:setRow ( 3, 	0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000 )
grid:setRow ( 4, 	0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001 )
grid:setRow ( 5, 	0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000 )
grid:setRow ( 6, 	0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001 )
grid:setRow ( 7, 	0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000 )
grid:setRow ( 8, 	0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001 )
grid:setRow ( 9, 	0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000 )
grid:setRow ( 10, 	0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001, 0x00000000, 0x00000001 )

map = MOAIProp2D.new ()
map:setDeck ( tileDeck )
map:setGrid ( grid )
map:setLoc ( -448, -320 )
map:setScl ( 1, 1 )
map:setPriority ( 0 )
map:setExpandForSort ( true )
layer:insertProp ( map )

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
map:setPriority ( 1 )
map:setExpandForSort ( true )
layer:insertProp ( map )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -128, -192, 128, 64 )
gfxQuad:setUVRect ( 0.75, 0, 1, 0.25 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setPriority ( 1 )
prop:setLoc ( 0, -256 )
prop:moveLoc ( 0, 480, 5 )
layer:insertProp ( prop )

