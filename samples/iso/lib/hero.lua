----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function runHero ( ... )

	local props = arg

	if type ( props [ 1 ]) == 'table' then
		props = props [ 1 ]
	end
	
	local startLocs = {}
	
	for i, prop in ipairs ( props ) do
		local x, y, z = prop:getLoc ()
		startLocs [ i ] = { x, y, z }
	end
	
	local function getStartLoc ( i )
		local loc = startLocs [ i ]
		return loc [ 1 ], loc [ 2 ], loc [ 3 ]
	end
	
	local function main ()

		local keyboard = MOAIInputMgr.device.keyboard
		local heroID = 1
		local x, y, z = getStartLoc ( heroID )
		
		local xd = 4
		local yd = 4
		local zd = 2
		
		while true do

			if keyboard:keyIsDown ( 'a' ) then
				y = y - yd 
			end
			
			if keyboard:keyIsDown ( 'd' ) then
				y = y + yd 
			end
			
			if keyboard:keyIsDown ( 'w' ) then
				x = x - xd
			end
			
			if keyboard:keyIsDown ( 's' ) then
				x = x + xd
			end
			
			if keyboard:keyIsDown ( 'e' ) then
				z = z + zd
			end
			
			if keyboard:keyIsDown ( 'q' ) then
				z = z - zd
			end
			
			if keyboard:keyIsDown ( 'x' ) then
				x, y, z = getStartLoc ( heroID )
			end
			
			if keyboard:keyIsDown ( 'g' ) then
				heroID = 1
				x, y, z = getStartLoc ( heroID )
				for i, prop in ipairs ( props ) do
					prop:setLoc ( getStartLoc ( i ))
				end
			end
			
			if keyboard:keyDown ( 'p' ) then
				for i, prop in ipairs ( props ) do
					local x, y, z = prop:getLoc ()
					print ( string.format ( 'prop %d: %f %f %f', i, x, y, z ))
				end
				print ()
			end
			
			if keyboard:keyDown ( ' ' ) then
				heroID = heroID + 1
				if #props < heroID then heroID = 1 end
				x, y, z = props [ heroID ]:getLoc ()
			end
			
			props [ heroID ]:setLoc ( x, y, z )
			
			coroutine.yield ()
		end
	end

	local thread = MOAICoroutine.new ()
	thread:run ( main )
end