----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

layer = MOAILayer2D.new ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
layer:setViewport ( viewport )

partition = MOAIPartition.new ()
layer:setPartition ( partition )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

function addSprite ( x, y, xScl, yScl, name )
	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	prop:setPriority ( priority )
	prop:setLoc ( x, y )
	prop:setScl ( xScl, yScl )
	prop.name = name
	partition:insertProp ( prop )
end

addSprite ( -64, 64, 0.5, 0.5, "sprite1" )
addSprite ( 64, 64, 0.5, 0.5, "sprite2" )
addSprite ( 0, 0, 0.5, 0.5, "sprite3" )
addSprite ( 64, -64, 0.5, 0.5, "sprite4" )
addSprite ( -64, -64, 0.5, 0.5, "sprite5" )

mouseX = 0
mouseY = 0

priority = 5

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

function pointerCallback ( x, y )
	
	local oldX = mouseX
	local oldY = mouseY
	
	mouseX, mouseY = layer:wndToWorld ( x, y )
	
	if pick then
		pick:addLoc ( mouseX - oldX, mouseY - oldY )
	end
end
		
function clickCallback ( down )
	
	if down then
		
		pick = partition:propForPoint ( mouseX, mouseY )
		
		if pick then
			print ( pick.name )
			pick:setPriority ( priority )
			priority = priority + 1
			pick:moveScl ( 0.25, 0.25, 0.125, MOAIEaseType.EASE_IN )
		end
	else
		if pick then
			pick:moveScl ( -0.25, -0.25, 0.125, MOAIEaseType.EASE_IN )
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

layer:setLoc ( 50, 100 )
