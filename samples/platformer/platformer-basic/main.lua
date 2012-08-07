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

surfaceDeck:reserveSurfaceLists ( 1 )
surfaceDeck:reserveSurfaces ( 1, 4 )
surfaceDeck:setSurface ( 1, 1, -64, -64, 64, -64 )
surfaceDeck:setSurface ( 1, 2, 64, -64, 64, 64  )
surfaceDeck:setSurface ( 1, 3, 64, 64, -64, 64 )
surfaceDeck:setSurface ( 1, 4, -64, 64, -64, -64 )

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
layer:insertProp ( terrain )

platformer = MOAIPlatformerBody2D.new ()
layer:insertProp ( platformer )
platformer:setLoc ( 0, 64 )

local function main ()
	
	local x, y
	local keyboard = MOAIInputMgr.device.keyboard
	
	while true do
		
		x, y = 0, 0
		
		if keyboard:keyIsDown ( 'a' ) then
			x = -2
		end
		
		if keyboard:keyIsDown ( 'd' ) then
			x = 2
		end
		
		if keyboard:keyIsDown ( 'w' ) then
			y = 2
		end
		
		if keyboard:keyIsDown ( 's' ) then
			y = -2
		end
		
		platformer:setMove ( x, y )
		
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )
