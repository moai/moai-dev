----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

draw = MOAIImage.new ()
draw:load ( 'moai.png' )

erase = MOAIImage.new ()
erase:load ( 'numbers.png' )

brush = MOAIImage.new ()
brush:init ( 32, 32 )

brush:fillCircle ( 16, 16, 15, 1, 1, 1, 0.5 )
brush:fillCircle ( 16, 16, 10, 0, 1, 1, 1 )

canvas = MOAIImageTexture.new ()
canvas:load ( erase )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( canvas )
gfxQuad:setRect ( 0, 0, 256, 256 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setPiv ( 128, 128 )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

function getPointerLocInProp ()

	local x, y = prop:worldToModel ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc ()))
	return x, y
end

function stamp ( x, y, source )

	local w, h = brush:getSize ()

	local x1 = x - ( w * 0.5 )
	local y1 = y - ( h * 0.5 )
	local x2 = x1 + w
	local y2 = y1 + h

	brush:copyRect ( source, x1, y1, x2, y2, 0, 0, w, h,
		MOAIImage.FILTER_LINEAR,
		MOAIImage.BLEND_FACTOR_1110,
		MOAIImage.BLEND_FACTOR_0001,
		MOAIImage.BLEND_EQ_ADD
	)

	canvas:copyRect ( brush, 0, 0, w, h, x1, y1, x2, y2,
		MOAIImage.FILTER_LINEAR,
		MOAIImage.BLEND_FACTOR_SRC_ALPHA,
		MOAIImage.BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		MOAIImage.BLEND_EQ_ADD
	)
	canvas:invalidate ( x1, y1, x2, y2 )
end

function onMouse ()

	local x, y = getPointerLocInProp ()

	--x, y = 16, 0

	if MOAIInputMgr.device.mouseLeft:isDown () then
		stamp ( x, y, draw )
	end

	if MOAIInputMgr.device.mouseRight:isDown () then
		stamp ( x, y, erase )
	end
end

MOAIInputMgr.device.pointer:setCallback ( onMouse )
MOAIInputMgr.device.mouseLeft:setCallback ( onMouse )
MOAIInputMgr.device.mouseRight:setCallback ( onMouse )
