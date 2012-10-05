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

surfaceDeck:reserveSurfaces ( 1, 1 )
surfaceDeck:setSurface ( 1, 1, 256, 0, -256, 0 )

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
terrain:setIndex ( 1 )
layer:insertProp ( terrain )

body = MOAIPlatformerBody2D.new ()
body:setFloorAngle ( 60 )
body:setCeilingAngle ( 0 )
layer:insertProp ( body )

dynamics = MOAIPlatformerDynamics2D.new ()
dynamics:setJumpParams ( 64, 0, 128, .5, 1 )
dynamics:setFallParams ( 128, 0, 192, .5, 1 )
dynamics:setBody ( body )
dynamics:start ()

local function main ()
	
	local xAccel
	local xDrag
	local keyboard = MOAIInputMgr.device.keyboard
	local checkKey = function ( k ) return keyboard:keyIsDown ( k ) end
	
	while true do
		
		xAccel = 0
		xDrag = 512
		
		if checkKey ( 'a' ) then
			xAccel = -256
			xDrag = 0
		end
		
		if checkKey ( 'd' ) then
			xAccel = 256
		xDrag = 0
		end
		
		if checkKey ( 'w' ) then
		end
		
		if checkKey ( 's' ) then
		end
		
		if keyboard:keyDown ( ' ' ) then
			print ( 'jump' )
			dynamics:jump ()
		end
		
		dynamics:setWalkParams ( xAccel, 4096, xDrag )
		
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )
