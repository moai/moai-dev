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

dynamics = MOAIPlatformerDynamics2D.new ()
dynamics:setJumpParams ( 0, 64, .5, 128, 1 )
dynamics:setFallParams ( 0, 128, .5, 192, 1 )

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 0.5, 0.5, 0.5, 1 )
	dynamics:drawJumpHull ( 256 )

	MOAIGfxDevice.setPenWidth ( 2 )
	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	dynamics:drawJumpArc ( 128, 256 )
	
	MOAIGfxDevice.setPointSize ( 4 )
	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	dynamics:drawJumpPoints ( 256 )
	
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -160, -240, 160, 240 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
prop:setLoc ( -128, 0 )
layer:insertProp ( prop )
