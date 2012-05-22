----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function makeBirdsEyeCamera ()
	local camera = MOAICamera.new ()
	camera:setOrtho ( true )
	camera:setNearPlane ( 10000 )
	camera:setFarPlane ( -10000 )
	camera:setRot ( 0, 0, 180 )
	return camera
end

function makeDimetricCamera ()
	local camera = MOAICamera.new ()
	camera:setOrtho ( true )
	camera:setNearPlane ( 10000 )
	camera:setFarPlane ( -10000 )
	camera:setRot ( 60, 0, 135 )
	return camera
end

function makeIsometricCamera ()
	local camera = MOAICamera.new ()
	camera:setOrtho ( true )
	camera:setNearPlane ( 10000 )
	camera:setFarPlane ( -10000 )
	camera:setRot ( 54.736, 0, 135 )
	return camera
end

function runCamera ( layer, camera )

	local birdsEye = makeBirdsEyeCamera ()
	local cameras = { camera, birdsEye }

	layer:setCamera ( camera )
	
	local function main ()

		local keyboard = MOAIInputMgr.device.keyboard
		local cameraID = 1
		local x, y = camera:getLoc ()
		
		while true do

			if keyboard:keyIsDown ( 'j' ) then
				y = y - 2
			end
			
			if keyboard:keyIsDown ( 'l' ) then
				y = y + 2
			end
			
			if keyboard:keyIsDown ( 'i' ) then
				x = x - 2
			end
			
			if keyboard:keyIsDown ( 'k' ) then
				x = x + 2
			end
			
			if keyboard:keyDown ( 'c' ) then
				cameraID = cameraID + 1
				if #cameras < cameraID then cameraID = 1 end
				layer:setCamera ( cameras [ cameraID ])
			end
			
			cameras [ cameraID ]:setLoc ( x, y, 0 )
			
			coroutine.yield ()
		end
	end

	local thread = MOAICoroutine.new ()
	thread:run ( main )
end