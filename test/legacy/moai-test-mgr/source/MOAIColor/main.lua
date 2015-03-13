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
	continue = false
	if action:isBusy () then
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
	screenIMG = MOAIImage.new ()
	if action:isBusy () then
		action:setListener ( MOAIAction.EVENT_STOP,
			function ()
				MOAIRenderMgr.grabNextFrame ( screenIMG, afterGrabTest )
			end )
	else
		MOAIRenderMgr.grabNextFrame ( screenIMG, afterGrabTest )
	end
end

function getImages ()
	action = MOAIAction.new ()
	
	prop:setColor ( .5, .6, 0, 0 )
	
	takeScreenshot ( 'output/setColor.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	action = prop:seekColor ( .2, 0, .9, 0, 3 )
	
	takeScreenshot ( 'output/seekColor.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	action = prop:moveColor ( 0, .9, .2, 1, 4 )
	
	takeScreenshot ( 'output/moveColor.png' )
	repeat coroutine.yield () until continue

	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function getImagesTest ()
	action = MOAIAction.new ()
	img1 = MOAIImage.new ()
	
	prop:setColor ( .5, .6, 0, 0 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( 'output/setColor.png' )
	if screenIMG:compare ( img1 ) == false then
		screenIMG:writePNG ( 'error/setColor.png' )
		evaluate ( false, 'compare setColor' )
	end
	resetProp ( prop )
	
	action = prop:seekColor ( .2, 0, .9, 0, 3 )
	
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( 'output/seekColor.png' )
	if screenIMG:compare ( img1 ) == false then
		screenIMG:writePNG ( 'error/seekColor.png' )
		evaluate ( false, 'compare seekColor' )
	end
	resetProp ( prop )
	
	action = prop:moveColor ( 0, .9, .2, 1, 4 )
	
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
