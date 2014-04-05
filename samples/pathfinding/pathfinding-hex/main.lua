----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:initHexGrid ( 5, 16, 32 )

grid:setRow ( 1, 	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 2,	0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 3,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 4,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 5,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 6,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 7,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 8,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 9,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 10,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 11,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 12,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 13,	0x03, 0x03, 0x03, 0x00, 0x03 )
grid:setRow ( 14,	0x03, 0x00, 0x03, 0x03, 0x03 )
grid:setRow ( 15,	0x03, 0x03, 0x03, 0x03, 0x03 )
grid:setRow ( 16,	0x03, 0x00, 0x03, 0x03, 0x03 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "hex-tiles.png" )
tileDeck:setSize ( 4, 4, 0.25, 0.216796875 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -264, -224 )
layer:insertProp ( prop )

startNode = grid:getCellAddr ( 1, 1 )
endNode = grid:getCellAddr ( 5, 16 )

pathFinder = MOAIPathFinder.new ()
pathFinder:setGraph ( grid )
pathFinder:setHeuristic ( MOAIGridPathGraph.EUCLIDEAN_DISTANCE )
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
grid:setTile ( 5, 16, 0x04 )
