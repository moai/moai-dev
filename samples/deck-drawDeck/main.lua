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

function onDraw ( draw, index )

	draw:setPenColor ( 1, 0, 0, 1 )
	draw:fillCircle ( 0, 0, 64, 32 )

	draw:setPenColor ( 1, 1, 1, 1 )
	draw:setPenWidth ( 2 )
	draw:drawEllipse ( 0, 0, 64, 64, 32 )
	
	draw:drawRay ( 0, 0, 0, 1 )

	draw:drawLine (
		-64, 64,
		64, 64,
		64, -64,
		-64, -64,
		-64, 64
	)

	draw:setPenColor ( 1, 1, 0, 1 )
	
	draw:fillFan (
		-32, -64,
		0, 64,
		32, -64
	)
end

drawDeck = MOAIDrawDeck.new ()
drawDeck:setDrawCallback ( onDraw )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( drawDeck )
prop:setPartition ( layer )

prop:moveRot ( 0, 0, 360, 1.5 )
