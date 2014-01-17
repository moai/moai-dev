----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local frameBuffer = nil

local function evaluate ( pass, str )
	if not pass then
		MOAITestMgr.comment ( "FAILED\t" .. str )
		success = false
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
	prop:setColor ( 0, 0, 0, 1 )
end

function afterGrab ()
	img:writePNG ( fileName )
	continue = true
end

function afterGrabTest ()
	continue = true
end

function takeScreenshot ( fName )
    frameBuffer = MOAIGfxDevice.getFrameBuffer()

	continue = false
	if action:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
                frameBuffer:grabNextFrame ( img, afterGrab )
			end )
	else
        frameBuffer:grabNextFrame ( img, afterGrab )
	end
	fileName = fName
end

function takeScreenshotTest ()
    frameBuffer = MOAIGfxDevice.getFrameBuffer()

	continue = false
	screenIMG = MOAIImage.new ()
	if action:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
                frameBuffer:grabNextFrame(  screenIMG, afterGrabTest )
			end )
	else
        frameBuffer:grabNextFrame ( screenIMG, afterGrabTest )
	end
end

function getImages ()

	action = MOAITimer.new ()
    action:setSpan(0.3)
    
	prop:setColor ( .5, .6, 0, 1 )
    action:start()
	
	takeScreenshot ( 'output/setColor.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	--action = prop:seekColor ( .2, 0, .9, 0, 3 )
    action = MOAIEaseDriver.new()
    action:reserveLinks(4)
    action:setLink(1, prop, MOAIProp.ATTR_R_COL, 0.2, MOAIEaseType.LINEAR)
    action:setLink(2, prop, MOAIProp.ATTR_G_COL, 0, MOAIEaseType.LINEAR)
    action:setLink(3, prop, MOAIProp.ATTR_B_COL, .9, MOAIEaseType.LINEAR)
    action:setLink(4, prop, MOAIProp.ATTR_A_COL, .3, MOAIEaseType.LINEAR)
    
    action:setSpan( 3 )
    action:start()
	
	takeScreenshot ( 'output/seekColor.png' )
	repeat coroutine.yield () until continue
    
    
	resetProp ( prop )
	

	--action = prop:moveColor ( 0, .9, .2, .7, 4 )
    local rr, gg, bb, aa = prop:getColor()

    action = MOAIEaseDriver.new()
    action:reserveLinks(4)
    
    action:setLink(1, prop, MOAIProp.ATTR_R_COL, rr + 0, MOAIEaseType.LINEAR)
    action:setLink(2, prop, MOAIProp.ATTR_G_COL, gg + 0.9, MOAIEaseType.LINEAR)
    action:setLink(3, prop, MOAIProp.ATTR_B_COL, bb + 0.2, MOAIEaseType.LINEAR)
    action:setLink(4, prop, MOAIProp.ATTR_A_COL, aa + 0.7, MOAIEaseType.LINEAR)
    

    action:setSpan( 4 )
    action:start()
	
	takeScreenshot ( 'output/moveColor.png' )
	repeat coroutine.yield () until continue
    

	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function getImagesTest ()
    
	action = MOAITimer.new ()
    action:setSpan(0.3)

	img1 = MOAIImage.new ()

	prop:setColor ( .5, .6, 0, 1 )
	
    action:start()

	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( 'output/setColor.png' )
	if screenIMG:compare ( img1 ) == false then
		screenIMG:writePNG ( 'error/setColor.png' )
		evaluate ( false, 'compare setColor' )
	end
	resetProp ( prop )
	
	--action = prop:seekColor ( .2, 0, .9, 0, 3 )
    action = MOAIEaseDriver.new()
    action:reserveLinks(4)
    action:setLink(1, prop, MOAIProp.ATTR_R_COL, 0.2, MOAIEaseType.LINEAR)
    action:setLink(2, prop, MOAIProp.ATTR_G_COL, 0, MOAIEaseType.LINEAR)
    action:setLink(3, prop, MOAIProp.ATTR_B_COL, .9, MOAIEaseType.LINEAR)
    action:setLink(4, prop, MOAIProp.ATTR_A_COL, .3, MOAIEaseType.LINEAR)
    
    action:setSpan( 3 )
    action:start()

	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( 'output/seekColor.png' )
	if screenIMG:compare ( img1 ) == false then
		screenIMG:writePNG ( 'error/seekColor.png' )
		evaluate ( false, 'compare seekColor' )
	end
	resetProp ( prop )
	
	--action = prop:moveColor ( 0, .9, .2, 1, 4 )
    local rr, gg, bb, aa = prop:getColor()

    action = MOAIEaseDriver.new()
    action:reserveLinks(4)
    
    action:setLink(1, prop, MOAIProp.ATTR_R_COL, rr + 0, MOAIEaseType.LINEAR)
    action:setLink(2, prop, MOAIProp.ATTR_G_COL, gg + 0.9, MOAIEaseType.LINEAR)
    action:setLink(3, prop, MOAIProp.ATTR_B_COL, bb + 0.2, MOAIEaseType.LINEAR)
    action:setLink(4, prop, MOAIProp.ATTR_A_COL, aa + 0.7, MOAIEaseType.LINEAR)

    action:setSpan( 4 )
    action:start()
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( 'output/moveColor.png' )
	if screenIMG:compare ( img1 ) == false then
		screenIMG:writePNG ( 'error/moveColor.png' )
		evaluate ( false, 'compare moveColor' )
	end
	
	MOAITestMgr.endTest ( success )	
	-- temporary until there's a nice way to close a window
	print ( "Testing done. Please close the MOAI window now." )
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIColor' )
	
	MOAISim.openWindow ( "MOAIColor", 400, 600 )
	
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
	MOAITestMgr.beginTest ( 'MOAIColor' )
	success = true
	
	MOAISim.openWindow ( "MOAIColor", 400, 600 )
	
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
	thread:run ( getImagesTest )	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
