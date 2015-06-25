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

	tess:pushPoly ( -50, 100, 50, 100, 0, 0 )
	tess:pushPoly ( 50, -100, -50, -100, 0, 0 )
	tess:pushPoly ( 0, 0, 100, -50, 100, 50 )
	tess:pushPoly ( 0, 0, -100, 50, -100, -50 )
	tess:pushPoly ( 25, -25, -25, -25, -25, 25, 25, 25 )

tess:finish ()

local region = MOAIRegion.new ()
tess:tesselate ( region )

local cursorX
local cursorY

local regionX
local regionY

local radius
local inside

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	region:drawDebug ()

	MOAIGfxDevice.setPenWidth ( 1 )

	if cursorX and regionX then
		MOAIGfxDevice.setPenColor ( 0, 0, 1, 1 )
		MOAIDraw.drawLine ( cursorX, cursorY, regionX, regionY )
	end

	if cursorX then

		if inside then
			MOAIGfxDevice.setPenColor ( 0, 0, 1, 1 )
			MOAIDraw.fillCircle ( cursorX, cursorY, radius )
		else
			MOAIGfxDevice.setPenColor ( 0, 1, 0, 1 )
			MOAIDraw.fillCircle ( cursorX, cursorY, radius )
		end
	end

	if regionX then
		MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
		MOAIDraw.fillCircle ( regionX, regionY, 3 )
	end
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

local prop = MOAIProp.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

prop:setColor ( 1, 0, 0, 1 )

main = function ()

	local x, y

	while true do

		local update = false

		if MOAIInputMgr.device.mouseLeft:isDown () then
			x, y = layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc ())
			update = true
		end

		if MOAIInputMgr.device.mouseRight:isDown () then
			prop:addRot ( 0, 0, -1 )
			update = true
		end

		if update and x then

			cursorX, cursorY = prop:worldToModel ( x, y )
			inside = region:pointInside ( cursorX, cursorY )

			local d, rX, rY = region:getDistance ( cursorX, cursorY )

			radius = ( d / 5 ) + 3

			if d then
				regionX, regionY = rX, rY
			end
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
