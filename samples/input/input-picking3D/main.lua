----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 480
dofile ( "cube.lua" )

MOAISim.openWindow ( "test", SCREEN_WIDTH, SCREEN_HEIGHT )

MOAIGfxDevice.setClearDepth ( true )
MOAIGfxDevice.setClearColor ( 0, 0, 0, 1 )

layer = MOAILayer.new ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )
viewport:setScale ( SCREEN_WIDTH, SCREEN_HEIGHT )
layer:setViewport ( viewport )

partition = MOAIPartition.new ()
layer:setPartition ( partition )

function addCube ( x, y, z, name )
	local prop = makeCube ( 16 )
	prop:setLoc ( x, y, z )
	prop.name = name
	partition:insertProp ( prop )
end

--add out of order to test sorting
addCube ( 0, 0, -20, "sprite1" )
addCube ( 0, 0, 20, "sprite5" )
addCube ( 0, 0, 0, "sprite3" )
addCube ( 0, 0, -10, "sprite2" )
addCube ( 0, 0, 10, "sprite4" )

mouseX = 0
mouseY = 0
mouseZ = 0

originX = 0
originY = 0
originZ = 0

directionX = 0
directionY = 0
directionZ = 0

priority = 5

camera = MOAICamera.new ()
cameraZ = camera:getFocalLength ( SCREEN_WIDTH )
camera:setLoc ( 0, 0, cameraZ )
layer:setCamera ( camera )

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

function pointerCallback ( x, y )
	
	local oldX = originX
	local oldY = originY
	
	originX, originY, originZ, directionX, directionY, directionZ  = layer:wndToWorld ( x, y, 0 )
	
	if pick then
		pick:addLoc ( (originX - oldX)*5, (originY - oldY)*5, 0 )
	end
end

function toTable ( ... )
	return arg
end

function clickCallback ( down )
	
	if down then
		
		--pick = partition:propForRay ( originX, originY, originZ, directionX, directionY, directionZ )
		pickList = toTable ( partition:propListForRay ( originX, originY, originZ, directionX, directionY, directionZ ))
		print ( pickList )
		for k,v in pairs( pickList ) do print(k,v) end
		
		pick = pickList[1]
		
		if pick then
			print ( pick.name )
			pick:moveScl ( 0.25, 0.25, 0.25, 0.125, MOAIEaseType.EASE_IN )
		end
	else
		if pick then
			pick:moveScl ( -0.25, -0.25, -0.25, 0.125, MOAIEaseType.EASE_IN )
			pick = nil
		end
	end
end

if MOAIInputMgr.device.pointer then
	
	-- mouse input
	MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
	MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
else

	-- touch input
	MOAIInputMgr.device.touch:setCallback ( 
	
		function ( eventType, idx, x, y, tapCount )

			pointerCallback ( x, y )
		
			if eventType == MOAITouchSensor.TOUCH_DOWN then
				clickCallback ( true )
			elseif eventType == MOAITouchSensor.TOUCH_UP then
				clickCallback ( false )
			end
		end
	)
end
