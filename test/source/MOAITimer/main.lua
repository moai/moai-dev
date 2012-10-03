----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
	end
end

function timerTest ()
	action = MOAIAction.new ()	
	
	timer = MOAITimer.new ()  
    timer:setSpan ( 2 ) 
    timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until timer:getTime () > 1
    timer:pause ()
	
	evaluate ( timer:getTime () > 1, 'getTime1' )
	evaluate ( timer:getTimesExecuted () == 0, 'getTimesExecuted0' )

	timer:setTime ( .5 ) 
    timer:start () 
	evaluate ( timer:getTime () == .5, 'setTime' )
	repeat coroutine.yield () until not timer:isBusy () 
	
	evaluate ( timer:getTime () == 2, 'getTime2' )
	evaluate ( timer:getTimesExecuted () == 1, 'getTimesExecuted1' )
	
	timer:stop ()
	
	timer2 = MOAITimer.new ()
	timer2:setSpan ( 4 )
	timer2:setSpeed ( 2 )
	timer2:setMode ( MOAITimer.NORMAL )
	
	timer:start ()
	timer2:start ()
	
	repeat coroutine.yield () until timer:getTime () > 1
	timer:pause ()
	timer2:pause ()
	
	evaluate ( timer2:getTime () > 2, 'setSpeed' )
	
	timer:stop ()
	timer2:stop ()
	
	timer:setSpan ( 3 )
	timer2:setSpeed ( 1 ) 
	timer2:setSpan ( .5 )
	timer2:setMode ( MOAITimer.LOOP )
	
	timer:start ()
	timer2:start ()
	
	timer2:setListener ( MOAITimer.EVENT_TIMER_END_SPAN, 
		function () 
			evaluate ( timer2:getTime () > .5, 'setMode' )
			if timer2:getTimesExecuted () == 5 then
				timer2:stop ()
			end
		end
	)
	repeat coroutine.yield () until not timer:isBusy ()
	
	timer:stop ()

	curve = MOAIAnimCurve.new () 
    curve:reserveKeys ( 3 ) 
    curve:setKey ( 1, 1, 1, MOAIEaseType.LINEAR, .95 ) 
    curve:setKey ( 2, 2, 2, MOAIEaseType.LINEAR, .95 ) 
    curve:setKey ( 3, 3, 3, MOAIEaseType.LINEAR, .95 ) 
     
    timer:setCurve ( curve ) 
    timer:setMode ( MOAITimer.LOOP ) 
    timer:setSpan ( 3 ) 
  
	timer:start ()
	i = 1
    timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, 
         function ( timer, keyframe, executed, keytime, value ) 
			evaluate ( keyframe == i, 'setCurve' )
			i = i + 1
			if i > 3 then
				timer:stop ()
			end
         end 
    ) 
	repeat coroutine.yield () until not timer:isBusy ()
	
	MOAITestMgr.endTest ( success )	
	
	-- temporary until there's a nice way to close a window
	print ( "Testing done. Please close the MOAI window now." )
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAITimer' )
	MOAISim.openWindow ( "MOAITimer", 400, 600 )
	
	viewport = MOAIViewport.new ()
	viewport:setSize ( 400, 600 )
	viewport:setScale ( 400, 600 )

	layer = MOAILayer.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "moai.png" )
	gfxQuad:setRect ( -64, -64, 64, 64 )
	gfxQuad:setUVRect ( 0, 1, 1, 0 )
	
	prop = MOAIProp.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	
	img = MOAIImage.new()
	
	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function test ()	
	MOAITestMgr.beginTest ( 'MOAITimer' )
	success = true
	
	-------------------------------------------------------------
	MOAISim.openWindow ( "MOAITimer", 400, 600 )
	
	viewport = MOAIViewport.new ()
	viewport:setSize ( 400, 600 )
	viewport:setScale ( 400, 600 )

	layer = MOAILayer.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "moai.png" )
	gfxQuad:setRect ( -64, -64, 64, 64 )
	gfxQuad:setUVRect ( 0, 1, 1, 0 )
	
	prop = MOAIProp.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	
	img = MOAIImage.new()

	thread = MOAIThread.new () 
	thread:run ( timerTest )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
