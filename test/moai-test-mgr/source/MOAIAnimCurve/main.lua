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
    timer:setSpan ( 3 ) 
    timer:setMode ( MOAITimer.LOOP )	

	curve = MOAIAnimCurve.new () 
    curve:reserveKeys ( 3 ) 
    curve:setKey ( 1, 1, 0, MOAIEaseType.LINEAR ) 
    curve:setKey ( 2, 2, 50, MOAIEaseType.LINEAR ) 
    curve:setKey ( 3, 3, 100, MOAIEaseType.LINEAR ) 
     
    timer:setCurve ( curve ) 
	evaluate ( curve:getLength () == 2, 'setCurve - getLength' )
	 
	timer:start ()
	i = 1
	j = 0
    timer:setListener ( MOAITimer.EVENT_TIMER_KEYFRAME, 
         function ( timer, keyframe, executed, keytime, value ) 
			evaluate ( curve:getValueAtTime ( timer:getTime ()) >= j, 'getValueAtTime' )
			evaluate ( keyframe == i, 'setCurve - keyframe' )
			i = i + 1
			j = j + 50
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
	MOAITestMgr.comment ( 'staging MOAIAnimCurve' )
	MOAISim.openWindow ( 'MOAIAnimCurve', 400, 600 )
	
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
	
	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function test ()	
	MOAITestMgr.beginTest ( 'MOAIAnimCurve' )
	success = true
	
	-------------------------------------------------------------
	MOAISim.openWindow ( 'MOAIAnimCurve', 400, 600 )
	
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

	thread = MOAIThread.new () 
	thread:run ( timerTest )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
