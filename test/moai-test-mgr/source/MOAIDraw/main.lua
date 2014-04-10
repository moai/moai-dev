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
	MOAIRenderMgr.grabNextFrame ( img, afterGrab )
	fileName = fName
end

function takeScreenshotTest ()
	continue = false
	MOAIRenderMgr.grabNextFrame ( img, afterGrabTest )
end

function draw ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 0, 1, .7, 1 )
	MOAIDraw.fillCircle ( 0, 0, 64, 32 )
end

function draw2 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 1, 0, 1 )
	MOAIDraw.drawBoxOutline ( 25, -25, 0, 100, 100, 0 )
end

function draw3 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 1, 0, 1 )
	MOAIDraw.drawCircle ( 0, 0, 20, 32 ) 
end

function draw4 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 1, 0, 1 )
	MOAIGfxDevice.setPenWidth ( 4 )
	MOAIDraw.drawEllipse ( 0, 0, 64, 64, 32 )
end

function draw5 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 0, 1, 0, 1 )
	MOAIGfxDevice.setPenWidth ( 2 )
	MOAIDraw.drawLine ( -50, 0, 5, 5, 0, 30 )
end

function draw6 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	MOAIGfxDevice.setPenWidth ( 3 )
	MOAIDraw.drawPoints {
		0, 10,
		0, 100,
		0, -100
	}
end

function draw7 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.drawRay ( 10, 20, 0, 1 )
end

function draw8 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, 1, 1, 1 )
	MOAIDraw.drawRect ( -100, -100, -25, 0 )
end

function draw9 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 0, .3, 1, 1 )
	MOAIDraw.fillEllipse ( 100, -75, 20, 50, 50 ) 
end

function draw10 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 0, .3, 1, 1 )
	MOAIDraw.fillFan {
		0, 0,
		125, 125, 
		150, 0
	}
end

function draw11 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 0, 1, 0, .5 )
	MOAIDraw.fillRect ( -100, 100, -50, 50 )
end

function makeCurve ( ease )
	local curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 2 )
	curve:setKey ( 1, 0, 0, ease )
	curve:setKey ( 2, 256, 256 )
	return curve
end

function draw12 ( index, xOff, yOff, xFlip, yFlip )
	MOAIGfxDevice.setPenColor ( 1, .2, 0, 1 )
	MOAIGfxDevice.setPenWidth ( 3 )
	local curves = {
	makeCurve ( MOAIEaseType.EASE_IN )
	}
	local curveID = 1
	MOAIDraw.drawAnimCurve ( curves [ curveID ], 100 )
end

function getImages ()
	scriptDeck = MOAIScriptDeck.new ()
	scriptDeck:setRect ( -64, -64, 64, 64 )
	
	timer = MOAITimer.new ()
	timer:setSpan ( .2 )
	timer:setMode ( MOAITimer.NORMAL )
	
	scriptDeck:setDrawCallback ( draw ) 
	prop = MOAIProp2D.new ()
	prop:setDeck ( scriptDeck )
	layer:insertProp ( prop )
	
	timer:start ()
	takeScreenshot ( 'output/fillCircle.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()

	scriptDeck:setDrawCallback ( draw2 )
	timer:start ()
	takeScreenshot ( 'output/drawBoxOutline.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw3 )
	timer:start ()
	takeScreenshot ( 'output/drawCircle.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw4 )
	timer:start ()
	takeScreenshot ( 'output/drawEllipse.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw5 )
	timer:start ()
	takeScreenshot ( 'output/drawLine.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw6 )
	timer:start ()
	takeScreenshot ( 'output/drawPoints.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw7 )
	timer:start ()
	takeScreenshot ( 'output/drawRay.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw8 )
	timer:start ()
	takeScreenshot ( 'output/drawRect.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw9 )
	timer:start ()
	takeScreenshot ( 'output/fillEllipse.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw10 )
	timer:start ()
	takeScreenshot ( 'output/fillFan.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw11 )
	timer:start ()
	takeScreenshot ( 'output/fillRect.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw12 )
	prop:setLoc ( -128, -128 )
	timer:start ()
	takeScreenshot ( 'output/drawAnimCurve.png' )
	repeat coroutine.yield () until not timer:isBusy ()
	timer:stop ()
	
	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the MOAI window now." )
end

function getImagesTest ()
	img1 = MOAIImage.new ()
	
	scriptDeck = MOAIScriptDeck.new ()
	scriptDeck:setRect ( -64, -64, 64, 64 )
	
	timer = MOAITimer.new ()
	timer:setSpan ( .2 )
	timer:setMode ( MOAITimer.NORMAL )
	
	scriptDeck:setDrawCallback ( draw ) 
	prop = MOAIProp2D.new ()
	prop:setDeck ( scriptDeck )
	layer:insertProp ( prop )
	
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/fillCircle.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/fillCircle.png' ) 
		evaluate ( false, 'compare fillCircle' )
	end
	timer:stop ()

	scriptDeck:setDrawCallback ( draw2 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawBoxOutline.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawBoxOutline.png' ) 
		evaluate ( false, 'compare drawBoxOutline' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw3 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawCircle.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawCircle.png' ) 
		evaluate ( false, 'compare drawCircle' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw4 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawEllipse.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawEllipse.png' ) 
		evaluate ( false, 'compare drawEllipse' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw5 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawLine.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawLine.png' ) 
		evaluate ( false, 'compare drawLine' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw6 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawPoints.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawPoints.png' ) 
		evaluate ( false, 'compare drawPoints' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw7 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawRay.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawRay.png' ) 
		evaluate ( false, 'compare drawRay' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw8 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawRect.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawRect.png' ) 
		evaluate ( false, 'compare drawRect' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw9 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/fillEllipse.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/fillEllipse.png' ) 
		evaluate ( false, 'compare fillEllipse' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw10 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/fillFan.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/fillFan.png' ) 
		evaluate ( false, 'compare fillFan' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw11 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/fillRect.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/fillRect.png' ) 
		evaluate ( false, 'compare fillRect' )
	end
	timer:stop ()
	
	scriptDeck:setDrawCallback ( draw12 )
	prop:setLoc ( -128, -128 )
	timer:start ()
	takeScreenshotTest ()
	repeat coroutine.yield () until continue 
	
	img1:load ( "output/drawAnimCurve.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/drawAnimCurve.png' ) 
		evaluate ( false, 'compare drawAnimCurve' )
	end
	timer:stop ()
	
	MOAITestMgr.endTest ( success )	
	-- temporary until there's a nice way to close a window
	print ( "Testing done. Please close the MOAI window now." )
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAIDraw' )
	
	MOAISim.openWindow ( "MOAIDraw", 320, 480 )

	viewport = MOAIViewport.new ()
	viewport:setSize ( 320, 480 )
	viewport:setScale ( 320, 480 )

	layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	img = MOAIImage.new()
	
	thread = MOAIThread.new ()
	thread:run ( getImages )
end

function test ()	
	MOAITestMgr.beginTest ( 'MOAIDraw' )
	success = true
	
	MOAISim.openWindow ( "MOAIDraw", 320, 480 )

	viewport = MOAIViewport.new ()
	viewport:setSize ( 320, 480 )
	viewport:setScale ( 320, 480 )

	layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	MOAISim.pushRenderPass ( layer )
	
	img = MOAIImage.new()

	thread = MOAIThread.new () 
	thread:run ( getImagesTest )	
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
