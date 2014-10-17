----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	--MOAIDraw.fillCircle ( 0, 0, 64, 32 )

	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	MOAIGfxDevice.setPenWidth ( 2 )
	--MOAIDraw.drawEllipse ( 0, 0, 64, 64, 32 )
	
	--MOAIDraw.drawRay ( 0, 0, 0, 1 )
	
	MOAIDraw.drawLine (
		-64, 64,
		64, 64,
		64, -64,
		-64, -64,
		-64, 64
	)
	
	--[[
	MOAIGfxDevice.setPenColor ( 1, 1, 0, 1 )
	
	MOAIDraw.fillFan (
		-32, -64,
		0, 64,
		32, -64
	)
	]]--
	
	-- one inflection
	MOAIGfxDevice.setPenColor ( 0, 1, 1, 1 )
	MOAIDraw.drawBezierCurve (
		-64, -64,
		-64, 0,
		64, 0,
		64, 64
	)
	
	-- one inflection
	MOAIGfxDevice.setPenColor ( 1, 0, 1, 1 )
	MOAIDraw.drawBezierCurve (
		64, -64,
		64, 0,
		-64, 0,
		-64, 64
	)
	
	-- one cusp
	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.drawBezierCurve (
		-64, -64,
		64, 64,
		-64, 64,
		64, -64
	)
	
	-- no inflections
	MOAIGfxDevice.setPenColor ( 1, 1, 0, 1 )
	MOAIDraw.drawBezierCurve (
		-64, -64,
		-64, 64,
		64, 64,
		64, -64
	)
	
	-- two inflections
	MOAIGfxDevice.setPenColor ( 0, 1, 0, 1 )
	MOAIDraw.drawBezierCurve (
		-64, -64,
		0, 64,
		0, 64,
		64, -64
	)
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

--prop:moveRot ( 360, 1.5 )
