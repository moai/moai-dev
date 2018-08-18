----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

region = MOAIRegion.new ()

region:reservePolygons ( 5 )

poly, x, y = 1, -100, 0
region:reserveVertices ( poly, 4 )
region:setVertex ( poly, 1, 	x + 0,		y + 0 )
region:setVertex ( poly, 2, 	x + 100,	y + 0 )
region:setVertex ( poly, 3, 	x + 100,	y + 100 )
region:setVertex ( poly, 4, 	x + 0,		y + 100 )

poly, x, y = 2, 0, 0
region:reserveVertices ( poly, 4 )
region:setVertex ( poly, 1, 	x + 0,		y + 0 )
region:setVertex ( poly, 2, 	x + 0,		y + 100 )
region:setVertex ( poly, 3, 	x + 100,	y + 100 )
region:setVertex ( poly, 4, 	x + 100,	y + 0 )

poly, x, y = 3, -100, -100
region:reserveVertices ( poly, 4 )
region:setVertex ( poly, 1, 	x + 0,		y + 0 )
region:setVertex ( poly, 2, 	x + 100,	y + 0 )
region:setVertex ( poly, 3, 	x + 10,		y + 10 )
region:setVertex ( poly, 4, 	x + 0,		y + 100 )

poly, x, y = 4, 0, -100
region:reserveVertices ( poly, 4 )
region:setVertex ( poly, 1, 	x + 0,		y + 0 )
region:setVertex ( poly, 2, 	x + 0,		y + 100 )
region:setVertex ( poly, 3, 	x + 10,		y + 10 )
region:setVertex ( poly, 4, 	x + 100,	y + 0 )

poly, x, y = 5, -100, -200
region:reserveVertices ( poly, 4 )
region:setVertex ( poly, 1, 	x + 0,		y + 0 )
region:setVertex ( poly, 2, 	x + 100,	y + 0 )
region:setVertex ( poly, 3, 	x + 0,		y + 100 )
region:setVertex ( poly, 4, 	x + 100,	y + 100 )

region:bless ()

function onDraw ( index, xOff, yOff, xFlip, yFlip )
	region:drawDebug ()
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp.new ()
prop:setDeck ( scriptDeck )
prop:setPartition ( layer )
