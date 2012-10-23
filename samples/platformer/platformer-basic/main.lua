----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

surfaceDeck = MOAISurfaceDeck2D.new ()

surfaceDeck:reserveSurfaceLists ( 9 )

-- single floor
surfaceDeck:reserveSurfaces ( 1, 1 )
surfaceDeck:setSurface ( 1, 1, 128, -16, -128, -16 )

-- box
surfaceDeck:reserveSurfaces ( 2, 4 )
surfaceDeck:setSurface ( 2, 1, -64, -64, 64, -64 )
surfaceDeck:setSurface ( 2, 2, 64, -64, 64, 64  )
surfaceDeck:setSurface ( 2, 3, 64, 64, -64, 64 )
surfaceDeck:setSurface ( 2, 4, -64, 64, -64, -64 )

-- arena with vertical walls
surfaceDeck:reserveSurfaces ( 3, 4 )
surfaceDeck:setSurface ( 3, 1, 0, 0, -128, 128 )
surfaceDeck:setSurface ( 3, 2, 128, 128, 0, 0 )
surfaceDeck:setSurface ( 3, 3, 96, 256, 96, 0 ) -- right
surfaceDeck:setSurface ( 3, 4, -96, 0, -96, 256 ) -- left

-- single floor, two walls
surfaceDeck:reserveSurfaces ( 4, 3 )
surfaceDeck:setSurface ( 4, 1, 128, 0, -128, 0 )
surfaceDeck:setSurface ( 4, 2, -16, 0, -16, 24 )
surfaceDeck:setSurface ( 4, 3, 16, 24, 16, 0 )

-- peaked floor and cathedral ceiling
surfaceDeck:reserveSurfaces ( 5, 4 )
surfaceDeck:setSurface ( 5, 1, 128, -96, 0, -32 )
surfaceDeck:setSurface ( 5, 2, 0, -32, -128, -96 )
surfaceDeck:setSurface ( 5, 3, -128, 64, 0, 128 )
surfaceDeck:setSurface ( 5, 4, 0, 128, 128, 64 )

-- single ceiling
surfaceDeck:reserveSurfaces ( 6, 1 )
surfaceDeck:setSurface ( 6, 1, -128, 16, 128, 16 )

-- pointy ledges
surfaceDeck:reserveSurfaces ( 7, 4 )
surfaceDeck:setSurface ( 7, 1, -256, -32, -64, 0 )
surfaceDeck:setSurface ( 7, 2, -64, 0, -256, 32 )
surfaceDeck:setSurface ( 7, 3, 256, 32, 64, 0 )
surfaceDeck:setSurface ( 7, 4, 64, 0, 256, -32 )

-- wedge shaped corner
surfaceDeck:reserveSurfaces ( 8, 2 )
surfaceDeck:setSurface ( 8, 1, -256, 128, 256, 0 )
surfaceDeck:setSurface ( 8, 2, 256, 0, -256, -128 )

-- ramp with walls in floor
surfaceDeck:reserveSurfaces ( 9, 6 )
surfaceDeck:setSurface ( 9, 1, 256, 128, 0, 0 )
surfaceDeck:setSurface ( 9, 2, 0, 0, -256, -128 )
surfaceDeck:setSurface ( 9, 3, 0, 0, 0, -64 )
surfaceDeck:setSurface ( 9, 4, 0, -64, 0, 0 )
surfaceDeck:setSurface ( 9, 5, 64, 128, 64, -64 )
surfaceDeck:setSurface ( 9, 6, -64, -64, -64, 128 )

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
terrain:setIndex ( 9 )
layer:insertProp ( terrain )

platformer = MOAIPlatformerBody2D.new ()
layer:insertProp ( platformer )
platformer:setFloorAngle ( 60 )
platformer:setCeilingAngle ( 30 )
--platformer:setScl ( 0.5, 1, 1 )

local function main ()
	
	local x, y
	local keyboard = MOAIInputMgr.device.keyboard
	local checkKey = function ( k ) return keyboard:keyIsDown ( k ) end
	
	while true do
		
		x, y = 0, 0
		
		if checkKey ( 'a' ) then
			x = -2
		end
		
		if checkKey ( 'd' ) then
			x = 2
		end
		
		if checkKey ( 'w' ) then
			y = 2
		end
		
		if checkKey ( 's' ) then
			y = -2
		end
		
		platformer:setMove ( x, y )
		
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )
