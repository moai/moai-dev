----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 256, 256 )

viewport = MOAIViewport.new ()
viewport:setSize ( 256, 256 )
viewport:setScale ( 256, -256 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 32, 32 )

grid:setRow ( 1, 	0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 2,	0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 3,	0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03 )
grid:setRow ( 4,	0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03 )
grid:setRow ( 5,	0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03 )
grid:setRow ( 6,	0x03, 0x03, 0x00, 0x03, 0x03, 0x00, 0x03, 0x03 )
grid:setRow ( 7,	0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03 )
grid:setRow ( 8,	0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "test.png" )
tileDeck:setSize ( 2, 2 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -128, -128 )
layer:insertProp ( prop )

startNode = grid:getCellAddr ( 1, 1 )
endNode = grid:getCellAddr ( 8, 8 )

pathFinder = MOAIPathFinder.new ()
pathFinder:setGraph ( grid )
pathFinder:init ( startNode, endNode )

while pathFinder:findPath ( 3 ) do
	print ( 'finding...' )
end

pathSize = pathFinder:getPathSize ()
for i = 1, pathSize do
	entry = pathFinder:getPathEntry ( i )
	x, y = grid:cellAddrToCoord ( entry )
	grid:setTile ( x, y, 0x02 )
end

grid:setTile ( 1, 1, 0x01 )
grid:setTile ( 8, 8, 0x04 )
