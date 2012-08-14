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

surfaceDeck:reserveSurfaceLists ( 4 )

-- arena with vertical walls
surfaceDeck:reserveSurfaces ( 1, 4 )
surfaceDeck:setSurface ( 1, 1, 0, 0, -128, 128 )
surfaceDeck:setSurface ( 1, 2, 128, 128, 0, 0 )
surfaceDeck:setSurface ( 1, 3, 96, 256, 96, 0 ) -- right
surfaceDeck:setSurface ( 1, 4, -96, 0, -96, 256 ) -- left

-- arena with overlapping floor
surfaceDeck:reserveSurfaces ( 2, 2 )
surfaceDeck:setSurface ( 2, 1, 128, 128, -128, -128 )
surfaceDeck:setSurface ( 2, 2, 128, -128, 0, 0 )

-- box
surfaceDeck:reserveSurfaces ( 3, 4 )
surfaceDeck:setSurface ( 3, 1, -64, -64, 64, -64 )
surfaceDeck:setSurface ( 3, 2, 64, -64, 64, 64  )
surfaceDeck:setSurface ( 3, 3, 64, 64, -64, 64 )
surfaceDeck:setSurface ( 3, 4, -64, 64, -64, -64 )

-- single floor
surfaceDeck:reserveSurfaces ( 4, 3 )
surfaceDeck:setSurface ( 4, 1, 128, 0, -128, 0 )
--surfaceDeck:setSurface ( 4, 2, -24, 0, -8, 24 )
--surfaceDeck:setSurface ( 4, 2, -34, 0, -34, 24 )
surfaceDeck:setSurface ( 4, 2, 16, 24, 16, 0 )
--surfaceDeck:setSurface ( 4, 3, 34, 24, 34, 0 )

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
terrain:setIndex ( 4 )
layer:insertProp ( terrain )

platformer = MOAIPlatformerBody2D.new ()
layer:insertProp ( platformer )
platformer:setFloorAngle ( 60 )
platformer:setCeilingAngle ( 0 )

platformer:setMove ( -32, 0 )

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

--local thread = MOAICoroutine.new ()
--thread:run ( main )
