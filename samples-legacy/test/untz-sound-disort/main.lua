----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

print ( "lobby talking test" )
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAIUntzSystem.initialize ( 22050, 2048 )

sound = MOAIUntzSound.new ()
sound:load ( 'lobby_talking.ogg' )
sound:setVolume ( 0.5 )
sound:setLooping ( false )

function onTouchEvent ( eventType, idx, x, y, tapCount )	
	if eventType == MOAITouchSensor.TOUCH_DOWN then
		print ( "play sound" )
		sound:play ()
	end
end

MOAIInputMgr.device.touch:setCallback ( onTouchEvent )
