----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
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

original = MOAIRegion.new ()

original:reservePolygons ( 1 )

original:reserveVertices ( 1, 6 )
original:setVertex ( 1, 1, -100, 100 )
original:setVertex ( 1, 2, -100, -100 )
original:setVertex ( 1, 3, 0, -50 )
original:setVertex ( 1, 4, 100, -100 )
original:setVertex ( 1, 5, 100, 100 )
original:setVertex ( 1, 6, 0, 50 )

original:bless ()

clip = MOAIRegion.new ()

clip:reservePolygons ( 1 )

clip:reserveVertices ( 1, 4 )
clip:setVertex ( 1, 1, -120, 0 )
clip:setVertex ( 1, 2, 120, 0 )
clip:setVertex ( 1, 3, 120, -120 )
clip:setVertex ( 1, 4, -120, -120 )
--[[
clip:setVertex ( 1, 1, -100, 100 )
clip:setVertex ( 1, 2, 100, 100 )
clip:setVertex ( 1, 3, 100, -100 )
clip:setVertex ( 1, 4, -100, -100 )
]]--

clip:bless ()

region = MOAIRegion.new ()

function onDraw ( index, xOff, yOff, xFlip, yFlip )
	region:drawDebug ()
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

local transform = MOAITransform.new ()

main = function ()

	local checkKey = function ( key )
		return MOAIInputMgr.device.keyboard:keyIsDown ( key )
	end

	local changed = true;

	while true do

		local xStep = 0
		local yStep = 0

		xStep = xStep + (( checkKey ( MOAIKeyCode.LEFT ) or checkKey ( MOAIKeyCode.NUM_4 )) and -1 or 0 )
		xStep = xStep + (( checkKey ( MOAIKeyCode.RIGHT ) or checkKey ( MOAIKeyCode.NUM_6 )) and 1 or 0 )

		if xStep ~= 0 then
			changed = true	
		end

		yStep = yStep + (( checkKey ( MOAIKeyCode.UP ) or checkKey ( MOAIKeyCode.NUM_8 )) and 1 or 0 )
		yStep = yStep + (( checkKey ( MOAIKeyCode.DOWN ) or checkKey ( MOAIKeyCode.NUM_2 )) and -1 or 0 )

		if yStep ~= 0 then
			changed = true	
		end

		if changed then

			transform:moveLoc ( xStep, yStep )
			region:clip ( original, clip, transform )

			changed = false
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
