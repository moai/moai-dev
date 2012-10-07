----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

camera = MOAICamera2D.new ()

layer = MOAILayer2D.new ()
layer:setCamera ( camera )
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

surfaceDeck = MOAISurfaceDeck2D.new ()

surfaceDeck:reserveSurfaceLists ( 1 )

surfaceDeck:reserveSurfaces ( 1, 4 )
surfaceDeck:setSurface ( 1, 1, 0.5, 0.5, -0.5, 0.5 )
surfaceDeck:setSurface ( 1, 2, -0.5, 0.5, -0.5, -0.5 )
surfaceDeck:setSurface ( 1, 3, -0.5, -0.5, 0.5, -0.5 )
surfaceDeck:setSurface ( 1, 4, 0.5, -0.5, 0.5, 0.5 )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )
grid:setRepeat ( true, false )

grid:setRow ( 1, 	0x00000001, 0x00000001 )
grid:setRow ( 2,	0x00000001, 0x00000001 )

terrain = MOAIProp.new ()
terrain:setDeck ( surfaceDeck )
terrain:setIndex ( 1 )
terrain:setGrid ( grid )
terrain:setLoc ( -62, -128 )
layer:insertProp ( terrain )

body = MOAIPlatformerBody2D.new ()
body:setFloorAngle ( 60 )
body:setCeilingAngle ( 0 )
layer:insertProp ( body )

dynamics = MOAIPlatformerDynamics2D.new ()
dynamics:setJumpParams ( 96, .25, 128, .7, 0.35 )
dynamics:setFallParams ( 96, .25, 128, .8, 0.3 )
dynamics:setBody ( body )
dynamics:start ()

camera:setParent ( body )

local function main ()
	
	local xAccel
	local xDrag
	local keyboard = MOAIInputMgr.device.keyboard
	local checkKey = function ( k ) return keyboard:keyIsDown ( k ) end
	
	while true do
		
		xAccel = 0
		xDrag = 1024
		
		if checkKey ( 'a' ) then
			xAccel = -512
			xDrag = 0
		end
		
		if checkKey ( 'd' ) then
			xAccel = 512
		xDrag = 0
		end
		
		if checkKey ( 'w' ) then
		end
		
		if checkKey ( 's' ) then
		end
		
		if keyboard:keyDown ( ' ' ) then
			dynamics:jump ()
		end
		
		dynamics:setWalkParams ( xAccel, 4096, xDrag )
		
		coroutine.yield ()
	end
end

local thread = MOAICoroutine.new ()
thread:run ( main )

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenWidth ( 1 )

	MOAIGfxDevice.setPenColor ( 0.5, 0.5, 0.5, 1 )
	dynamics:drawJumpHull ( 128 )

	MOAIGfxDevice.setPenColor ( 0.75, 0.75, 0.75, 1 )
	dynamics:drawJumpArc ( 128, 128 )
	
	MOAIGfxDevice.setPointSize ( 4 )
	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	dynamics:drawJumpPoints ( 128 )
	
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -160, -240, 160, 240 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
prop:setLoc ( -64, 0 )
--layer:insertProp ( prop )