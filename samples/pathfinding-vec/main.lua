----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 256, 256 )

graph = MOAIVecPathGraph.new ()
graph:reserveNodes ( 8 )
graph:setNode ( 1, 1, 1 )
graph:setNode ( 2, 5, 2 )
graph:setNode ( 3, 9, 1 )
graph:setNode ( 4, 9, 6 )
graph:setNode ( 5, 5, 7 )
graph:setNode ( 6, 9, 9 )
graph:setNode ( 7, 4, 8 )
graph:setNode ( 8, 2, 9 )
graph:setNeighbors ( 1, 2 )
graph:setNeighbors ( 2, 3 )
graph:setNeighbors ( 3, 5 )
graph:setNeighbors ( 5, 4 )
graph:setNeighbors ( 5, 6 )
graph:setNeighbors ( 6, 7 )
graph:setNeighbors ( 7, 8 )
graph:setNeighbors ( 7, 1 )

startNode = 1
endNode = 6

pathFinder = MOAIPathFinder.new ()
pathFinder:setGraph ( graph )
pathFinder:init ( startNode, endNode )

while pathFinder:findPath ( 3 ) do
	print ( 'finding...' )
end

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	-- draw graph
	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )

	for i = 1, graph:getNodeCount () do
		local x, y = graph:getNode ( i )
		MOAIDraw.fillCircle ( x, y, 0.2, 32 )

		for j = 1, graph:getNodeCount () do
			if graph:areNeighbors ( i, j ) then
				local x2, y2 = graph:getNode ( j )
				MOAIDraw.drawLine ( x, y, x2, y2 )
			end
		end
	end

	-- draw path
	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )

	local lastX, lastY
	for i = 1, pathFinder:getPathSize () do
		local entry = pathFinder:getPathEntry ( i )
		local x, y = graph:getNode ( entry )
		MOAIDraw.fillCircle ( x, y, 0.2, 32 )

		if lastX and lastY then
			MOAIDraw.drawLine ( lastX, lastY, x, y )
		end

		lastX, lastY = x, y
	end
end

viewport = MOAIViewport.new ()
viewport:setSize ( 256, 256 )
viewport:setOffset ( -1, -1 )
viewport:setScale ( 10, 10 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( 0, 0, 10, 10 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )
