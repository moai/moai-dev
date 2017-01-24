----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

function onDraw ( index )

	MOAIDraw.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.fillCircle ( 0, 0, 64, 32 )

	MOAIDraw.setPenColor ( 1, 1, 1, 1 )
	MOAIDraw.setPenWidth ( 2 )
	MOAIDraw.drawEllipse ( 0, 0, 64, 64, 32 )
	
	MOAIDraw.drawRay ( 0, 0, 0, 1 )
	
	--[[
	MOAIDraw.drawLine (
		-32, 32,
		32, 32,
		32, -32,
		-32, -32,
		-32, 32
	)
	]]--

	MOAIDraw.drawLine (
		-64, 64,
		64, 64,
		64, -64,
		-64, -64,
		-64, 64
	)

	MOAIDraw.setPenColor ( 1, 1, 0, 1 )
	
	MOAIDraw.fillFan (
		-32, -64,
		0, 64,
		32, -64
	)
	
end

drawDeck = MOAIDrawDeck.new ()
drawDeck:setBounds ()
drawDeck:setDrawCallback ( onDraw )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( drawDeck )
prop:setLayer ( layer )

prop:moveRot ( 0, 0, 360, 1.5 )
