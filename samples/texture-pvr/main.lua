----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 256, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 256, 512 )
viewport:setScale ( 256, -512 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

pvrs = {

	'cathead.pvr',

	--'zipline-ogl-a-8.pvr',
	--'zipline-ogl-ai-88.pvr',
	--'zipline-ogl-bgra-8888.pvr',
	--'zipline-ogl-i-8.pvr',

	'zipline-ogl-pvrtc-2bpp.pvr',
	'zipline-ogl-pvrtc-4bpp.pvr',
	'zipline-ogl-rgb-565.pvr',
	'zipline-ogl-rgb-888.pvr',
	'zipline-ogl-rgba-4444.pvr',
	'zipline-ogl-rgba-5551.pvr',
	'zipline-ogl-rgba-8888.pvr',
}

texQuadFromFile = MOAIGfxQuad2D.new ()
texQuadFromFile:setRect ( -128, -128, 128, 128 )
texQuadFromFile:setUVRect ( 0, 0, 1, 1 )

texQuadFromImage = MOAIGfxQuad2D.new ()
texQuadFromImage:setRect ( -128, -128, 128, 128 )
texQuadFromImage:setUVRect ( 0, 0, 1, 1 )

local currentTexture = nil
local nextTexture = function ()

	currentTexture = ( currentTexture or 0 ) + 1
	currentTexture = currentTexture <= #pvrs and currentTexture or 1

	local filename = pvrs [ currentTexture ]
	print ( 'SHOWING:', filename )

	texQuadFromFile:setTexture ( filename )

	-- do this to force software loading of PVR (and decompression)
	local image = MOAIImage.new ()
	image:load ( filename )

	texQuadFromImage:setTexture ( image )
end

prop1 = MOAIProp.new ()
prop1:setDeck ( texQuadFromFile )
prop1:setLoc ( 0, -128 )
prop1::setPartition ( layer )

prop2 = MOAIProp.new ()
prop2:setDeck ( texQuadFromImage )
prop2:setLoc ( 0, 128 )
prop2::setPartition ( layer )

nextTexture ()

if MOAIInputMgr.device.mouseLeft then

	local onMouseButton = function ( down )
		if down == true then
			nextTexture ()
		end
	end

	MOAIInputMgr.device.mouseLeft:setCallback ( onMouseButton )
end

if MOAIInputMgr.device.touch then

	local onTouch = function ( eventType, idx, x, y, tapCount )
		if eventType == MOAITouchSensor.TOUCH_DOWN then
			nextTexture ()
		end
	end

	MOAIInputMgr.device.touch:setCallback ( onTouch )
end
