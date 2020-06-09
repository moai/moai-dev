----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

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

function onDraw ( draw, index, xOff, yOff, xFlip, yFlip )

	draw:setPenColor ( 1, 0, 0, 1 )
    draw:drawAnimCurve ( makeCurve ( curveTypes [ curveIndex ].value ), 100 )
	if curveIndex ~= prevIndex then
		print ( string.format ( "%i of %i: %s", curveIndex, #curveTypes, curveTypes [ curveIndex ].name ))
		prevIndex = curveIndex
	end
end

drawDeck = MOAIDrawDeck.new ()
drawDeck:setDrawCallback ( onDraw )

prop = MOAIProp.new ()
prop:setDeck ( drawDeck )
prop:setLoc ( -128, -128 )
prop:setPartition ( layer )

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

