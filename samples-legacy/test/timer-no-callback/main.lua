----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local test = 1

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

function KeyframeTest ()
	local timer = MOAITimer.new ()
	
	curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 3 )
	curve:setKey ( 1, 1, 1, MOAIEaseType.LINEAR, .95 )
	curve:setKey ( 2, 2, 2, MOAIEaseType.LINEAR, .95 )
	curve:setKey ( 3, 3, 3, MOAIEaseType.LINEAR, .95 )
	
	timer:setCurve ( curve )
	timer:setMode ( MOAITimer.LOOP )
	timer:setSpan ( 3 )
	
	timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME,
		function ( timer, keyframe, executed, keytime, value )
			print ( "Keyframe Callback on keyframe: " .. keyframe )
		end
	)
	
	timer:start ()
end

function LoopTest ()
	local timer = MOAITimer.new ()
	timer:setSpan ( 1 )
	timer:setMode ( MOAITimer.LOOP )
	timer:setListener ( MOAITimer.EVENT_TIMER_LOOP,
		function ()
			print ( "Loop Callback" )
		end
	)
	timer:start ()
	print ( "Timer started." )
end

if test == 1 then
	KeyframeTest ()
elseif test == 2 then
	LoopTest ()
end