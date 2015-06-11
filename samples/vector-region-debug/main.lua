--==============================================================
-- Copyright (c) 2013 Point Inside, Inc.
-- All Rights Reserved.
-- http://pointinside.com
--==============================================================

MOAISim.openWindow ( "test", 640, 480 )
MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

tess = MOAIVectorTesselator.new ()

tess:setCircleResolution ( 32 )

tess:setFillStyle ( MOAIVectorTesselator.FILL_SOLID )
tess:setFillColor ( 0.6, 0.75, 1.0, 1.0 )

tess:pushCombo ()

	tess:pushPoly ( 150, -150, -150, -150, -150, 150, 150, 150 )

	tess:pushPoly ( 50, -50, -50, -50, -50, 50, 50, 50 )

	--[[
	tess:pushPoly ( -50, 100, 50, 100, 0, 0 )
	tess:pushPoly ( 50, -100, -50, -100, 0, 0 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 0, 0, -100, 50, -100, -50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )
	]]--
tess:finish ()

local region = MOAIRegion.new ()
tess:tesselate ( region )

function onDraw ( index, xOff, yOff, xFlip, yFlip )
	region:drawDebug ()
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

local prop = MOAIProp.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

prop:setColor ( 1, 0, 0, 1 )

function clickCallback ( down )
	
	if down then
		
		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = prop:worldToModel ( layer:wndToWorld ( x, y ))
		if region:pointInside ( x, y ) then
			prop:addRot ( 0, 0, 5 )
		end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
