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

original = MOAIRegion.new ()

--[[
original:reservePolygons ( 1 )

original:reserveVertices ( 1, 4 )
original:setVertex ( 1, 1, -100, -100 )
original:setVertex ( 1, 2, 100, 100 )
original:setVertex ( 1, 3, -100, 100 )
original:setVertex ( 1, 4, 100, -100 )
]]--

--[[
original:reserveVertices ( 1, 10 )
original:setVertex ( 1, 1, -100, 100 )
original:setVertex ( 1, 2, -100, 0 )
original:setVertex ( 1, 3, -100, -100 )
original:setVertex ( 1, 4, 0, -50 )
original:setVertex ( 1, 5, 0, -50 )
original:setVertex ( 1, 6, 100, -100 )
original:setVertex ( 1, 7, 100, 0 )
original:setVertex ( 1, 8, 100, 100 )
original:setVertex ( 1, 9, 0, 50 )
original:setVertex ( 1, 10, 0, 50 )
]]--

--[[
original:reserveVertices ( 1, 6 )
original:setVertex ( 1, 1, -100, -100 )
original:setVertex ( 1, 2, 0, -50 )
original:setVertex ( 1, 3, 100, -100 )
original:setVertex ( 1, 4, 100, 100 )
original:setVertex ( 1, 5, 0, 50 )
original:setVertex ( 1, 6, -100, 100 )
]]--

--[[
poly, x, y = 1, 0, -100
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 0,		y + 100 )
original:setVertex ( poly, 3, 	x + 10,		y + 10 )
original:setVertex ( poly, 4, 	x + 100,	y + 0 )
]]--
--[[
poly, x, y = 1, -100, -200
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 100,	y + 0 )
original:setVertex ( poly, 3, 	x + 0,		y + 100 )
original:setVertex ( poly, 4, 	x + 100,	y + 100 )
]]--

original:reservePolygons ( 5 )

poly, x, y = 1, -100, 0
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 100,	y + 0 )
original:setVertex ( poly, 3, 	x + 100,	y + 100 )
original:setVertex ( poly, 4, 	x + 0,		y + 100 )

poly, x, y = 2, 0, 0
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 0,		y + 100 )
original:setVertex ( poly, 3, 	x + 100,	y + 100 )
original:setVertex ( poly, 4, 	x + 100,	y + 0 )

poly, x, y = 3, -100, -100
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 100,	y + 0 )
original:setVertex ( poly, 3, 	x + 10,		y + 10 )
original:setVertex ( poly, 4, 	x + 0,		y + 100 )

poly, x, y = 4, 0, -100
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 0,		y + 100 )
original:setVertex ( poly, 3, 	x + 10,		y + 10 )
original:setVertex ( poly, 4, 	x + 100,	y + 0 )

poly, x, y = 5, -100, -200
original:reserveVertices ( poly, 4 )
original:setVertex ( poly, 1, 	x + 0,		y + 0 )
original:setVertex ( poly, 2, 	x + 100,	y + 0 )
original:setVertex ( poly, 3, 	x + 0,		y + 100 )
original:setVertex ( poly, 4, 	x + 100,	y + 100 )

original:bless ()

function onDraw ( index, xOff, yOff, xFlip, yFlip )
	region:drawDebug ()
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

region = MOAIRegion.new ()

local a = 0 -- 315
local d = 0

main = function ()

	local checkKey = function ( key )
		return MOAIInputMgr.device.keyboard:keyIsDown ( key )
	end

	local clip = true;

	while true do

		local aStep = 0
		local dStep = 0

		aStep = aStep + (( checkKey ( MOAIKeyCode.LEFT ) or checkKey ( MOAIKeyCode.NUM_4 )) and -1 or 0 )
		aStep = aStep + (( checkKey ( MOAIKeyCode.RIGHT ) or checkKey ( MOAIKeyCode.NUM_6 )) and 1 or 0 )

		if aStep ~= 0 then
			a = ( a + aStep ) % 360
			clip = true	
		end

		dStep = dStep + (( checkKey ( MOAIKeyCode.UP ) or checkKey ( MOAIKeyCode.NUM_8 )) and -1 or 0 )
		dStep = dStep + (( checkKey ( MOAIKeyCode.DOWN ) or checkKey ( MOAIKeyCode.NUM_2 )) and 1 or 0 )

		if dStep ~= 0 then
			d = d + dStep
			clip = true	
		end

		if clip then

			local r = math.rad ( a )
			xn = -math.sin ( r )
			yn = math.cos ( r )

			--print ( 'clip', d, a )
			region:clipToPlane ( original, xn, yn, d )

			clip = false
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
