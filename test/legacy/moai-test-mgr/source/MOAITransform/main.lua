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
	if action:isBusy () then
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
	-- add, move, seek, set
	
	-- test addLoc
	prop:addLoc ( 0, 100, 0 )
	takeScreenshot ( 'output/addLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveLoc
	action = prop:moveLoc ( 0, -100, 0, 3 )
	takeScreenshot ( 'output/moveLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekLoc
	action = prop:seekLoc ( 100, 100, 0, 3 )
	takeScreenshot ( 'output/seekLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setLoc
	prop:setLoc ( 50, 0, 0 )
	takeScreenshot ( 'output/setLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addPiv
	prop:addPiv ( 80, 0, 0 )
	takeScreenshot ( 'output/addPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test movePiv	
	action = prop:movePiv ( 0, 50, 0, 3 )
	takeScreenshot ( 'output/movePiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekPiv
	action = prop:seekPiv ( -100, 0, 0, 3 )
	takeScreenshot ( 'output/seekPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setPiv
	prop:setPiv ( 50, 0, 0 )
	takeScreenshot ( 'output/setPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addRot
	prop:addPiv ( 180, 0, 0 )
	takeScreenshot ( 'output/addRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveRot
	action = prop:moveRot ( 0, 0, 360, 4 )
	takeScreenshot ( 'output/moveRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekRot
	action = prop:seekRot ( 1, 0, 180, 3 )
	takeScreenshot ( 'output/seekRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setRot
	prop:setRot ( 0, 0, -180 )
	takeScreenshot ( 'output/setRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addScl
	prop:addScl ( 1, 0, 0 )
	takeScreenshot ( 'output/addScl.png' )
	
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveScl	
	action = prop:moveScl ( 1, 1, 0, 4 )
	takeScreenshot ( 'output/moveScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekScl
	action = prop:seekScl ( 1, 2, 0, 3 )
	takeScreenshot ( 'output/seekScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setScl
	prop:setScl ( 1, 1, 0 )
	takeScreenshot ( 'output/setScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seek
	action = prop:seek ( 1, 1, 1, 0, 0, 360, 1, 2, 1, 5 )
	takeScreenshot ( 'output/seek.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test move
	action = prop:move ( 20, 1, 0, 0, 0, 180, 1, 1, 1, 3 )
	takeScreenshot ( 'output/move.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByX
	prop:setShearByX ( 2, 0 )
	takeScreenshot ( 'output/setShearByX.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByY
	prop:setShearByY ( 4, 2 )
	takeScreenshot ( 'output/setShearByY.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByZ
	prop:setShearByZ ( 20, 1 )
	takeScreenshot ( 'output/setShearByZ.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- create prop2
	prop2 = MOAIProp.new ()
	prop2:setDeck ( gfxQuad )
	layer:insertProp ( prop2 )
	
	-- test setParent
	prop2:setLoc ( 0, 150, 0 )
	prop2:setParent ( prop )
	
	-- test addLoc (setParent)
	prop:addLoc ( 0, 100, 0 )
	takeScreenshot ( 'output/setParent/addLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveLoc (setParent)
	action = prop:moveLoc ( 0, -100, 0, 3 )
	takeScreenshot ( 'output/setParent/moveLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekLoc (setParent)
	action = prop:seekLoc ( 100, 100, 0, 3 )
	takeScreenshot ( 'output/setParent/seekLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setLoc (setParent)
	prop:setLoc ( 50, 0, 0 )
	takeScreenshot ( 'output/setParent/setLoc.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addPiv (setParent)
	prop:addPiv ( 80, 0, 0 )
	takeScreenshot ( 'output/setParent/addPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test movePiv (setParent)
	action = prop:movePiv ( 0, 50, 0, 3 )
	takeScreenshot ( 'output/setParent/movePiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekPiv (setParent)
	action = prop:seekPiv ( -100, 0, 0, 3 )
	takeScreenshot ( 'output/setParent/seekPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setPiv (setParent)
	prop:setPiv ( 50, 0, 0 )
	takeScreenshot ( 'output/setParent/setPiv.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addRot (setParent)
	prop:addPiv ( 180, 0, 0 )
	takeScreenshot ( 'output/setParent/addRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveRot (setParent)
	action = prop:moveRot ( 0, 0, 360, 4 )
	takeScreenshot ( 'output/setParent/moveRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekRot (setParent)
	action = prop:seekRot ( 0, 0, 180, 3 )
	takeScreenshot ( 'output/setParent/seekRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setRot (setParent)
	prop:setRot ( 0, 0, -180 )
	takeScreenshot ( 'output/setParent/setRot.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test addScl (setParent)
	prop:addScl ( 1, 0, 0 )
	takeScreenshot ( 'output/setParent/addScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test moveScl (setParent)	
	action = prop:moveScl ( 1, 1, 0, 4 )
	takeScreenshot ( 'output/setParent/moveScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seekScl (setParent)
	action = prop:seekScl ( 1, 2, 0, 3 )
	takeScreenshot ( 'output/setParent/seekScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setScl (setParent)
	prop:setScl ( 1, 1, 0 )
	takeScreenshot ( 'output/setParent/setScl.png' )
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test seek (setParent)
	action = prop:seek ( 1, 1, 1, 0, 0, 360, 1, 2, 1, 5 )
	takeScreenshot ( 'output/setParent/seek.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test move (setParent)
	action = prop:move ( 20, 1, 0, 0, 0, 180, 1, 1, 1, 3 )
	takeScreenshot ( 'output/setParent/move.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByX (setParent)
	prop:setShearByX ( 2, 0 )
	takeScreenshot ( 'output/setParent/setShearByX.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByY (setParent)
	prop:setShearByY ( 4, 2 )
	takeScreenshot ( 'output/setParent/setShearByY.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
	
	-- test setShearByZ (setParent)
	prop:setShearByZ ( 20, 1 )
	takeScreenshot ( 'output/setParent/setShearByZ.png'	)
	repeat coroutine.yield () until continue
	resetProp ( prop )
			
	-- temporary until there's a nice way to close a window
	print ( "Staging done. Please close the Moai window now." )
end

function getImagesTest ()	
	-- test getLoc
	prop:addLoc ( 0, 50, 100 )
	x, y, z = prop:getLoc ()
	if x ~= 0 or y ~= 50 or z ~= 100 then
		evaluate ( false, 'getLoc' )
	end
	resetProp ( prop ) 
	
	-- test getPiv
	prop:addPiv ( 50, 20, 200 )
	x, y, z = prop:getPiv ()
	if x ~= 50 or y ~= 20 or z ~= 200 then
		evaluate ( false, 'getPiv' )
	end
	resetProp ( prop ) 
	
	-- test getRot
	prop:addRot ( 0, 0, 270 )
	x, y, z = prop:getRot ()
	if x ~= 0 or y ~= 0 or z ~= 270 then
		evaluate ( false, 'getRot' )
	end
	resetProp ( prop ) 
	
	-- test getScl
	prop:addScl ( 1, 1, 0 )
	x, y, z = prop:getScl ()
	if x ~= 2 or y ~= 2 or z ~= 1 then
		evaluate ( false, 'getScl' )
	end
	resetProp ( prop ) 
	
	-- test modelToWorld
	prop:setLoc ( 8, 90, 30 )
	prop:setRot ( 0, 0, 360 )
	prop:setScl ( 2, 1, 0 )
	
	x, y, z = prop:modelToWorld ( 25, 70, 90 )
	evaluate ( x == 25 and y == 70 and z == 90, 'modelToWorld' )
	
	timer = MOAITimer.new () 
	timer:setSpan ( 1 )
	timer:start ()
	repeat coroutine.yield () until not timer:isBusy ()
	
	x, y, z = prop:getWorldDir ()
	evaluate ( x == 1 and y == 0 and z == 0, 'getWorldDir' )
	
	x, y, z = prop:getWorldLoc ()
	evaluate ( x == 8 and y == 90 and z == 30, 'getWorldLoc' )
	
	x = prop:getWorldRot ()
	evaluate ( x == 90, 'getWorldRot' )
	
	x, y, z = prop:getWorldScl ()
	evaluate ( x == 2 and y == 1 and z == 0, 'getWorldScl' )
 
	resetProp ( prop )
	
	-- test worldToModel
	x, y, z = prop:worldToModel ( 80, 20, 10 )
	evaluate ( x == 168 and y == 110 and z == 30, 'worldToModel' )

	resetProp ( prop )
	
	action = MOAIAction.new ()
	-- add, move, seek, set
	
	-- test addLoc
	prop:addLoc ( 0, 100, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1 = MOAIImage.new ()
	img1:load ( "output/addLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareAddLoc.png' )
		evaluate ( false, 'compare addLoc' )
	end
	
	resetProp ( prop )
	
	-- test moveLoc
	action = prop:moveLoc ( 0, -100, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/moveLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareMoveLoc.png' )
		evaluate ( false, 'compare moveLoc' )
	end
	
	resetProp ( prop )
	
	-- test seekLoc
	action = prop:seekLoc ( 100, 100, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/seekLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSeekLoc.png' )
		evaluate ( false, 'compare seekLoc' )
	end
	
	resetProp ( prop )
	
	-- test setLoc
	prop:setLoc ( 50, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetLoc.png' )
		evaluate ( false, 'compare setLoc' )
	end
	
	resetProp ( prop )
	
	-- test addPiv
	prop:addPiv ( 80, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/addPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareAddPiv.png' )
		evaluate ( false, 'compare addPiv' )
	end
	
	resetProp ( prop )
	
	-- test movePiv	
	action = prop:movePiv ( 0, 50, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/movePiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareMovePiv.png' )
		evaluate ( false, 'compare movePiv' )
	end
	
	resetProp ( prop )
	
	-- test seekPiv
	action = prop:seekPiv ( -100, 0, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/seekPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSeekPiv.png' )
		evaluate ( false, 'compare seekPiv' )
	end
	
	resetProp ( prop )
	
	-- test setPiv
	prop:setPiv ( 50, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetPiv.png' )
		evaluate ( false, 'compare setPiv' )
	end
	
	resetProp ( prop )
	
	-- test addRot
	prop:addPiv ( 180, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/addRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareAddRot.png' )
		evaluate ( false, 'compare addRot' )
	end
	
	resetProp ( prop )
	
	-- test moveRot
	action = prop:moveRot ( 0, 0, 360, 4 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/moveRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareMoveRot.png' )
		evaluate ( false, 'compare moveRot' )
	end
	
	resetProp ( prop )
	
	-- test seekRot
	action = prop:seekRot ( 1, 0, 180, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/seekRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSeekRot.png' )
		evaluate ( false, 'compare seekRot' )
	end
	
	resetProp ( prop )
	
	-- test setRot
	prop:setRot ( 0, 0, -180 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetRot.png' )
		evaluate ( false, 'compare setRot' )
	end
	
	resetProp ( prop )
	
	-- test addScl
	prop:addScl ( 1, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/addScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareAddScl.png' )
		evaluate ( false, 'compare addScl' )
	end
	
	resetProp ( prop )
	
	-- test moveScl	
	action = prop:moveScl ( 1, 1, 0, 4 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/moveScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareMoveScl.png' )
		evaluate ( false, 'compare moveScl' )
	end
	
	resetProp ( prop )
	
	-- test seekScl
	action = prop:seekScl ( 1, 2, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/seekScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSeekScl.png' )
		evaluate ( false, 'compare seekScl' )
	end
	
	resetProp ( prop )
	
	-- test setScl
	prop:setScl ( 1, 1, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetScl.png' )
		evaluate ( false, 'compare setScl' )
	end
	
	resetProp ( prop )
	
	-- test seek
	action = prop:seek ( 1, 1, 1, 0, 0, 360, 1, 2, 1, 5 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/seek.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSeek.png' )
		evaluate ( false, 'compare seek' )
	end
	
	resetProp ( prop )
	
	-- test move
	action = prop:move ( 20, 1, 0, 0, 0, 180, 1, 1, 1, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/move.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareMove.png' )
		evaluate ( false, 'compare move' )
	end
	
	resetProp ( prop )
	
	-- test setShearByX
	prop:setShearByX ( 2, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setShearByX.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetShearByX.png' )
		evaluate ( false, 'compare setShearByX' )
	end
	
	resetProp ( prop )
	
	-- test setShearByY
	prop:setShearByY ( 4, 2 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setShearByY.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/compareSetShearByY.png' )
		evaluate ( false, 'compare setShearByY' )
	end
	
	resetProp ( prop )
	
	-- test setShearByZ
	prop:setShearByZ ( 20, 1 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setShearByZ.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/comparesetShearByZ.png' )
		evaluate ( false, 'compare setShearByZ' )
	end
	
	resetProp ( prop )
	
	-- create prop2
	prop2 = MOAIProp.new ()
	prop2:setDeck ( gfxQuad )
	layer:insertProp ( prop2 )
	
	-- test setParent
	prop2:setLoc ( 0, 150, 0 )
	prop2:setParent ( prop )
	
	-- test addLoc (setParent)
	prop:addLoc ( 0, 100, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/addLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareAddLoc.png' )
		evaluate ( false, 'compare setParent addLoc' )
	end
	
	resetProp ( prop )
	
	-- test moveLoc (setParent)
	action = prop:moveLoc ( 0, -100, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/moveLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareMoveLoc.png' )
		evaluate ( false, 'compare setParent moveLoc' )
	end
	
	resetProp ( prop )
	
	-- test seekLoc (setParent)
	action = prop:seekLoc ( 100, 100, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/addLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSeekLoc.png' )
		evaluate ( false, 'compare setParent seekLoc' )
	end
	
	resetProp ( prop )
	
	-- test setLoc (setParent)
	prop:setLoc ( 50, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setLoc.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetLoc.png' )
		evaluate ( false, 'compare setParent setLoc' )
	end
	
	resetProp ( prop )
	
	-- test addPiv (setParent)
	prop:addPiv ( 80, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/addPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareAddPiv.png' )
		evaluate ( false, 'compare setParent addPiv' )
	end
	
	resetProp ( prop )
	
	-- test movePiv (setParent)
	action = prop:movePiv ( 0, 50, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/movePiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareMovePiv.png' )
		evaluate ( false, 'compare setParent movePiv' )
	end
	
	resetProp ( prop )
	
	-- test seekPiv (setParent)
	action = prop:seekPiv ( -100, 0, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/seekPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSeekPiv.png' )
		evaluate ( false, 'compare setParent seekPiv' )
	end
	
	resetProp ( prop )
	
	-- test setPiv (setParent)
	prop:setPiv ( 50, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setPiv.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetPiv.png' )
		evaluate ( false, 'compare setParent setPiv' )
	end
	
	resetProp ( prop )
	
	-- test addRot (setParent)
	prop:addPiv ( 180, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/addRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareAddRot.png' )
		evaluate ( false, 'compare setParent addRot' )
	end
	
	resetProp ( prop )
	
	-- test moveRot (setParent)
	action = prop:moveRot ( 0, 0, 360, 4 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/moveRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareMoveRot.png' )
		evaluate ( false, 'compare setParent moveRot' )
	end
	
	resetProp ( prop )
	
	-- test seekRot (setParent)
	action = prop:seekRot ( 0, 0, 180, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/seekRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSeekRot.png' )
		evaluate ( false, 'compare setParent seekRot' )
	end
	
	resetProp ( prop )
	
	-- test setRot (setParent)
	prop:setRot ( 0, 0, -180 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setRot.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetRot.png' )
		evaluate ( false, 'compare setParent setRot' )
	end
	
	resetProp ( prop )
	
	-- test addScl (setParent)
	prop:addScl ( 1, 0, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/addScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareAddScl.png' )
		evaluate ( false, 'compare setParent addScl' )
	end
	
	resetProp ( prop )
	
	-- test moveScl (setParent)	
	action = prop:moveScl ( 1, 1, 0, 4 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/moveScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareMoveScl.png' )
		evaluate ( false, 'compare setParent moveScl' )
	end
	
	resetProp ( prop )
	
	-- test seekScl (setParent)
	action = prop:seekScl ( 1, 2, 0, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/seekScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSeekScl.png' )
		evaluate ( false, 'compare setParent seekScl' )
	end
	
	resetProp ( prop )
	
	-- test setScl (setParent)
	prop:setScl ( 1, 1, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setScl.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetScl.png' )
		evaluate ( false, 'compare setParent setScl' )
	end
	
	resetProp ( prop )
	
	-- test seek (setParent)
	action = prop:seek ( 1, 1, 1, 0, 0, 360, 1, 2, 1, 5 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/seek.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSeek.png' )
		evaluate ( false, 'compare setParent seek' )
	end
	
	resetProp ( prop )
	
	-- test move (setParent)
	action = prop:move ( 20, 1, 0, 0, 0, 180, 1, 1, 1, 3 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/move.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareMove.png' )
		evaluate ( false, 'compare setParent move' )
	end
	
	resetProp ( prop )
	
	-- test setShearByX (setParent)
	prop:setShearByX ( 2, 0 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setShearByX.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetShearByX.png' )
		evaluate ( false, 'compare setParent setShearByX' )
	end
	
	resetProp ( prop )
	
	-- test setShearByY (setParent)
	prop:setShearByY ( 4, 2 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setShearByY.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetShearByY.png' )
		evaluate ( false, 'compare setParent setShearByY' )
	end
	
	resetProp ( prop )
	
	-- test setShearByZ (setParent)
	prop:setShearByZ ( 20, 1 )
	takeScreenshotTest ()
	repeat coroutine.yield () until continue
	
	img1:load ( "output/setParent/setShearByZ.png" )
	if img:compare ( img1 ) == false then
		img:writePNG ( 'error/setParent/compareSetShearByZ.png' )
		evaluate ( false, 'compare setParent setShearByZ' )
	end
	
	resetProp ( prop )
	
	MOAITestMgr.endTest ( success )
	-- temporary until there's a nice way to close a window
	print ( "Testing done. Please close the MOAI window now." )
end

function stage ()
	MOAITestMgr.comment ( 'staging MOAITransform' )
	
	MOAISim.openWindow ( "MOAITransform", 400, 600 )
	
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
	MOAITestMgr.beginTest ( 'MOAITransform' )
	success = true
	
	MOAISim.openWindow ( "MOAITransform", 400, 600 )
	
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
	
	threadTest = MOAIThread.new ()
	threadTest:run ( getImagesTest )
end

MOAITestMgr.setStagingFunc ( stage )
MOAITestMgr.setTestFunc ( test )
MOAITestMgr.setFilter ( MOAITestMgr.UTIL )
