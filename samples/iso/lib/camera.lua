----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

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

function runCamera ( camera, target )

	local rot = {}
	
	rot [ 1 ] = {}
	rot [ 2 ] = {}
	
	rot [ 1 ].x, rot [ 1 ].y, rot [ 1 ].z = camera:getRot ()
	rot [ 2 ].x, rot [ 2 ].y, rot [ 2 ].z = 0, 0, 135
	
	local keyboard = MOAIInputMgr.device.keyboard
	local rotID = 1
	
	local function main ()
		
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
				rotID = rotID + 1
				if #rot < rotID then rotID = 1 end
				local angles = rot [ rotID ]
				camera:setRot ( angles.x, angles.y, angles.z )
			end
			
			camera:setLoc ( x, y, 0 )
			
			coroutine.yield ()
		end
	end

	local function targetMain ()
		
		while true do
		
			if keyboard:keyDown ( 'c' ) then
				rotID = rotID + 1
				if #rot < rotID then rotID = 1 end
				local angles = rot [ rotID ]
				camera:setRot ( angles.x, angles.y, angles.z )
			end
		
			local x, y = target:getLoc ()
			camera:setLoc ( x, y, 0 )
			coroutine.yield ()
		end
	end
	
	local thread = MOAICoroutine.new ()
	
	if target then
		thread:run ( targetMain )
	else
		thread:run ( main )
	end
end