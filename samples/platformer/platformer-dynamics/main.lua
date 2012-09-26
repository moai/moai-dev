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

surfaceDeck:reserveSurfaceLists ( 5 )

-- single floor
surfaceDeck:reserveSurfaces ( 1, 1 )
surfaceDeck:setSurface ( 1, 1, 128, 0, -128, 0 )

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

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
terrain:setIndex ( 1 )
layer:insertProp ( terrain )

body = MOAIPlatformerBody2D.new ()
body:setFloorAngle ( 60 )
body:setCeilingAngle ( 0 )
body:setLoc ( 0, 64 )
layer:insertProp ( body )

dynamics = MOAIPlatformerDynamics2D.new ()
dynamics:setBody ( body )
dynamics:start ()

--[[
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
]]--
