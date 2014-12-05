----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( 'test', 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

idx = 1

formats = {
	{ name = 'COLOR_FMT_RGB_888', id = MOAIImage.COLOR_FMT_RGB_888 },
	{ name = 'COLOR_FMT_RGB_565', id = MOAIImage.COLOR_FMT_RGB_565 },
	{ name = 'COLOR_FMT_RGBA_5551', id = MOAIImage.COLOR_FMT_RGBA_5551 },
	{ name = 'COLOR_FMT_RGBA_4444', id = MOAIImage.COLOR_FMT_RGBA_4444 },
	{ name = 'COLOR_FMT_RGBA_8888', id = MOAIImage.COLOR_FMT_RGBA_8888 },
}

texture = MOAITexture.new ()
texture:setReloader ( function ()

	local format = formats [ idx ];
	print ( 'FORMAT', format.name )

	local image = MOAIImage.new ()
	image:load ( 'colorwheel.png' )
	image = image:convert ( format.id )
	image = image:convert ( MOAIImage.COLOR_FMT_RGBA_8888 )

	texture:load ( image )

	idx = idx < #formats and idx + 1 or 1
end )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

----------------------------------------------------------------
function onMouseLeftEvent ( down )
	if not down then
		MOAIGfxResourceMgr.purgeResources ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )