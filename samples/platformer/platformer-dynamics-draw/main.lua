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
dynamics:setJumpParams ( 120, 128, 1 )
dynamics:setFallParams ( -8, -128, 1 )

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	dynamics:drawJumpArc ( 100, 128 )
	
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -160, -240, 160, 240 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )
