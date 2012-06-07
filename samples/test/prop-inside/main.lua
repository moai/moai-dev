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

local prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( -64, 64 )
partition:insertProp ( prop )

mouseX = 0
mouseY = 0

function pointerCallback ( x, y )
	mouseX, mouseY = layer:wndToWorld ( x, y )
end
		
function clickCallback ( down )
	
	if down then
		print ( prop:inside ( mouseX, mouseY, 0, -1 ))
	end
end

MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
