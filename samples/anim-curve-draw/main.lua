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

function makeCurve ( ease )

	local curve = MOAIAnimCurve.new ()
	curve:reserveKeys ( 2 )
	curve:setKey ( 1, 0, 0, ease )
	curve:setKey ( 2, 256, 256 )
	
	return curve
end

-- Determine curve types via reflection
local curveTypes = {}
for name, value in pairs ( MOAIEaseType ) do
	if type ( value ) == "number" then
		table.insert ( curveTypes, { name = name, value = value })
	end
end
table.sort ( curveTypes, function ( a, b ) return a.value < b.value end )

local prevIndex = 0
local curveIndex = 1

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.drawAnimCurve ( makeCurve ( curveTypes [ curveIndex ].value ), 100 )
	if curveIndex ~= prevIndex then
		print ( string.format ( "%i of %i: %s", curveIndex, #curveTypes, curveTypes [ curveIndex ].name ))
		prevIndex = curveIndex
	end
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -128, -128, 128, 128 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
prop:setLoc ( -128, -128 )
layer:insertProp ( prop )

print ( "Press space to go to the next ease type, backspace to go back." )

function onKeyboardEvent ( key, down )

	if down == true then
		if key == 32 then -- space
			curveIndex = curveIndex + 1
			if curveIndex > #curveTypes then curveIndex = 1 end
		elseif key == 8 then -- backspace
			curveIndex = curveIndex - 1
			if curveIndex == 0 then curveIndex = #curveTypes end
		end
	end
end

MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardEvent )

