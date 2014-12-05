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

function callback ( timer, keyframe, executed, keytime, value )
	if keyframe == 1 then
		timer:setTime ( 3 )
	end
	print ( "key frame: " .. keyframe .. ", executed: " .. executed )
end

timer = MOAITimer.new ()
timer:setMode ( MOAITimer.LOOP )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 3 )
curve:setKey ( 1, 1, 10, MOAIEaseType.LINEAR, .95 )
curve:setKey ( 2, 2.5, 15, MOAIEaseType.LINEAR, .95 )
curve:setKey ( 3, 4.5, 30, MOAIEaseType.LINEAR, .95 )

timer:setCurve ( curve )

timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, callback )
timer:setSpan ( 5 )
timer:start ()

