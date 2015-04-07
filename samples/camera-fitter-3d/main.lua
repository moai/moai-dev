----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

require ( 'cube' )

--MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_MODEL_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 960, 480 )

--MOAIGfxDevice.getFrameBuffer ():setClearDepth ( true )

XMIN = 20
XMAX = 940

YMIN = 20
YMAX = 460

viewport = MOAIViewport.new ()
viewport:setSize ( XMIN, YMIN, XMAX, YMAX )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setClearColor ( 1, 1, 1, 1 )
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 480 )
viewport:setScale ( 960, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

camera = MOAICamera.new ()
camera:setFieldOfView ( 45 )
camera:setLoc ( 0, 0, 200 )
layer:setCamera ( camera )

local cube = cube.makeCube ( 32 )

makeProp = function ( x, y, z )

	local prop = MOAIProp.new ()
	prop:setDeck ( cube )
	prop:setCullMode ( MOAIGraphicsProp.CULL_BACK )
	prop:setLoc ( x, y, z )
	layer:insertProp ( prop )

	return prop
end

orbit = function ( x, y, angle )

	angle = math.rad ( angle )

	local s = math.sin ( angle )
	local c = math.cos ( angle )

	return (( x * c ) - ( y * s )), (( x * s ) + ( y * c ))
end

props = {
	makeProp ( 0, 0, 0 ),
	makeProp ( 64, 0, 0 ),
	makeProp ( 128, 0, 0 ),
	makeProp ( 192, 0, 0 ),
}

points = {
	{ x = 0, y = 0, z = 0, r = 16 },
	{ x = 64, y = 0, z = 0, r = 16 },
	{ x = 128, y = 0, z = 0, r = 16 },
	{ x = 192, y = 0, z = 0, r = 16 },
}

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

	local rotX = 0
	local rotY = 0

	while true do

		if checkKey ( 'h' ) then
			camera:setLoc ( 0, 0, 200 )
			camera:setRot ( 0, 0, 0 )
		end
		
		local left = checkKey ( MOAIKeyCode.LEFT ) or checkKey ( MOAIKeyCode.NUM_4 )
		local right = checkKey ( MOAIKeyCode.RIGHT ) or checkKey ( MOAIKeyCode.NUM_6 )

		local up = checkKey ( MOAIKeyCode.UP ) or checkKey ( MOAIKeyCode.NUM_8 )
		local down = checkKey ( MOAIKeyCode.DOWN ) or checkKey ( MOAIKeyCode.NUM_2 )

		local xMov = 0
		xMov = xMov + ( up and -1 or 0 )
		xMov = xMov + ( down and 1 or 0 )

		local yMov = 0
		yMov = yMov + ( left and -1 or 0 )
		yMov = yMov + ( right and 1 or 0 )

		if xMov ~= 0 or yMov ~= 0 then

			rotX = ( rotX + xMov ) % 360
			rotY = ( rotY + yMov ) % 360

			camera:setLoc ( 0, 0, 0 )
			camera:setRot ( rotX, rotY, 0 )

			-- boundaries for the fit rect must not cross the center line of the layer's viewport
			local x, y, z = layer:getFitting3D ( props, XMIN, YMIN, XMAX, YMAX )
			camera:setLoc ( x, y, z )

			--print ( rot, x, z )
		end

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
