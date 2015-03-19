----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

require ( 'cube' )

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, 200 )
layer:setCamera ( camera )

local cube = cube.makeCube ( 16 )

makeProp = function ( x, y, z )

	local prop = MOAIProp.new ()
	prop:setDeck ( cube )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	prop:setLoc ( x, y, z )
	layer:insertProp ( prop )

	return prop
end

makeProp ( 0, 0, 0 )
--makeProp ( 0, 0, 64 )
--makeProp ( 0, 0, -64 )

local keyNames = {}
for name, value in pairs ( MOAIKeyCode ) do
	if type( value ) == "number" then
		keyNames [ value ] = name
	end
end

checkKey = function ( key )
	return MOAIInputMgr.device.keyboard:keyIsDown ( key )
end

main = function ()

	while true do

		if checkKey ( 'h' ) then
			camera:setLoc ( 0, 0, 200 )
			camera:setRot ( 0, 0, 0 )
		end

		local xMov, yMov, zMov = 0, 0, 0

		local left = checkKey ( MOAIKeyCode.LEFT ) or checkKey ( MOAIKeyCode.NUM_4 )
		local right = checkKey ( MOAIKeyCode.RIGHT ) or checkKey ( MOAIKeyCode.NUM_6 )

		local up = checkKey ( MOAIKeyCode.NUM_8 ) or checkKey ( ']' )
		local down = checkKey ( MOAIKeyCode.NUM_2 ) or checkKey ( '[' )

		xMov = xMov + ( left and -1 or 0 )
		xMov = xMov + ( right and 1 or 0 )

		yMov = yMov + ( up and 1 or 0 )
		yMov = yMov + ( down and -1 or 0 )

		zMov = zMov + ( checkKey ( MOAIKeyCode.UP ) and -1 or 0 )
		zMov = zMov + ( checkKey ( MOAIKeyCode.DOWN ) and 1 or 0 )

		if xMov ~= 0 or yMov ~= 0 or zMov ~= 0 then
			local x, y, z = camera:getLoc ()
			camera:setLoc ( x + xMov, y + yMov, z + zMov )
		end

		if checkKey ( 'c' ) then
			camera:lookAt ( 0, 0, 0 )
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
