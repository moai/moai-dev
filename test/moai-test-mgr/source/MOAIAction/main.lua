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

function isBusyCheck ()
	evaluate ( action:isBusy () == true, 'action isBusy' )
end

function isActiveCheck ()	
	evaluate ( action:isActive () == true, 'action isActive' )
end	

function onLoop ( timer ) 
	isBusyCheck ()
	isActiveCheck ()
	if timer:getTimesExecuted () == 6 then
		timer:stop ()
	end
end

function resetProp ( prop )
	prop:setLoc ( 0, 0, 0 )
	prop:setPiv ( 0, 0, 0 )
	prop:setRot ( 0, 0, 0 )
	prop:setScl ( 1, 1, 1 )
	prop:setShearByX ( 0, 0 )
	prop:setShearByY ( 0, 0 )
	prop:setShearByZ ( 0, 0 )
end

function afterGrab ()
	img:writePNG ( fileName )
	continue = true
end

function afterGrabTest ()
	continue = true
end

function takeScreenshot ( fName )
	continue = false
	if timer:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrab )
			end )
	else
		MOAIRenderMgr.grabNextFrame ( img, afterGrab )
	end
	fileName = fName
end

function takeScreenshotTest ()
	continue = false
	if timer:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
			end )
	else
		MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
	end
end

function getImages ()
	action = MOAIAction.new ()
	
	-- test pause 
	action = prop:moveLoc ( 80, 125, 0, 3 )
	
	timer = MOAITimer.new ()  -- lets the action run for one second and then pauses it
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	action:pause ()

	timer:setTime ( 0 ) -- goes from 1 to 2 seconds
    timer:start () 
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshot ( 'output/pause1.png' )
	repeat coroutine.yield () until continue 
	
	-- test start
	action:start ()
	timer = MOAITimer.new () 
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL ) 
    timer:start () 
    repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshot ( 'output/pause2.png' )
	repeat coroutine.yield () until continue 
	
	timer:setTime ( 1 ) 
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	resetProp ( prop )
	
	-- test stop
	action = prop:moveLoc ( -80, -120, 0, 2 )
	
	timer = MOAITimer.new () 
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL ) 
    timer:start () 
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshot ( 'output/stop1.png' )
	repeat coroutine.yield () until continue
	
	action:stop ()
	
	takeScreenshot ( 'output/stop2.png' )
	repeat coroutine.yield () until continue
	
	action:start ()
	
	timer = MOAITimer.new ()
	timer:setSpan ( 2 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () -- timer goes from 0 to 2
	
	takeScreenshot ( 'output/stop3.png' )
	repeat coroutine.yield () until continue
	
	resetProp ( prop )	
	
	action = prop:moveRot ( 0, 0, 360, 4 ) -- throttle
	action:throttle ( 5 )
	
	timer = MOAITimer.new ()
	timer:setSpan ( .5 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshot ( 'output/throttle1.png' )
	repeat coroutine.yield () until continue
	
	timer = MOAITimer.new ()
	timer:setSpan ( 3.5 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshot ( 'output/throttle2.png' )
	repeat coroutine.yield () until continue	
	
	-- for addChild you pass the child and the object is the parent
	-- for attach you pass ( ) the parent and the object is the child
	
	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function timerTest ()
	action = MOAIAction.new ()	
	img1 = MOAIImage.new ()
	
	action = prop:moveLoc ( 80, 125, 0, 3 )
	
	timer = MOAITimer.new ()  
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	action:pause ()

	timer:setTime ( 0 ) 
    timer:start () 
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/pause1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/pause1.png' ) 
		evaluate ( false, 'compare pause1' )
	end
	
	action:start ()
	timer = MOAITimer.new () 
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL ) 
    timer:start () 
    repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/pause2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/pause2.png' ) 
		evaluate ( false, 'compare pause2' )
	end
	
	timer:setTime ( 1 ) 
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	resetProp ( prop )
	
	action = prop:moveLoc ( -80, -120, 0, 2 )
	
	timer = MOAITimer.new () 
    timer:setSpan ( 1 ) 
    timer:setMode ( MOAITimer.NORMAL ) 
    timer:start () 
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/stop1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/stop1.png' ) 
		evaluate ( false, 'compare stop1' )
	end
	
	action:stop ()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/stop2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/stop2.png' ) 
		evaluate ( false, 'compare stop2' )
	end
	
	action:start ()
	
	timer = MOAITimer.new ()
	timer:setSpan ( 2 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () -- timer goes from 0 to 2
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/stop3.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/stop3.png' ) 
		evaluate ( false, 'compare stop3' )
	end
	
	resetProp ( prop )	
	
	action = prop:moveRot ( 0, 0, 360, 4 ) -- throttle
	action:throttle ( 5 )
	
	timer = MOAITimer.new ()
	timer:setSpan ( .5 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/throttle1.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/throttle1.png' ) 
		evaluate ( false, 'compare throttle1' )
	end
	
	timer = MOAITimer.new ()
	timer:setSpan ( 3.5 )
	timer:setMode ( MOAITimer.NORMAL )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy () 
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/throttle2.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/throttle2.png' ) 
		evaluate ( false, 'compare throttle2' )
	end

	evaluate ( action:isDone () == true, 'action isDone' )
	
	MOAITestMgr.endTest ( success )	
	-- temporary until there's a nice way to close a window
	print ( "Testing done. Please close the MOAI window now." )
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIAction' )
	
	MOAISim.openWindow ( "MOAIAction", 400, 600 )
	
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
	thread:run ( getImages )
end



function test ()	
	MOAITestMgr.beginTest ( 'MOAIAction' )
	success = true
	
	---------------------------------------------------------------
	MOAISim.openWindow ( "MOAIAction", 400, 600 )
	
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
	
	
	---------------------------------------------------------------------------
	
	
	---------------------------------------------------------------------------
	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
