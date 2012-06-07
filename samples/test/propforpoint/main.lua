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

-- create prop
local prop = MOAIProp2D.new ()
-- prop:setDeck ( gfxQuad )
prop:setFrame ( -128, -128, 128, 128 )
prop.name = "sprite"
partition:insertProp ( prop )

mouseX = 0
mouseY = 0

priority = 5

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
		print ( pick )
		
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

MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )

