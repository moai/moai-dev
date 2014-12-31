----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

VIEW_WIDTH = 512
VIEW_HEIGHT = 512

if MOAIAppIOS then
	VIEW_WIDTH, VIEW_HEIGHT = MOAIGfxDevice.getViewSize ()
end

minPow2 = function ( num )
	local n = 1
	while n < num do n = n * 2 end
	return n
end

PROP_WIDTH = minPow2 ( VIEW_WIDTH )
PROP_HEIGHT = minPow2 ( VIEW_HEIGHT )

CANVAS_WIDTH = 512
CANVAS_HEIGHT = 512

BRUSH_SIZE = 64

MOAISim.openWindow ( "test", VIEW_WIDTH, VIEW_HEIGHT )

viewport = MOAIViewport.new ()
viewport:setSize ( VIEW_WIDTH, VIEW_HEIGHT )
viewport:setScale ( VIEW_WIDTH, -VIEW_HEIGHT )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

draw = MOAIImage.new ()
draw:load ( 'moai.png' )
draw = draw:resize ( CANVAS_WIDTH, CANVAS_HEIGHT )

erase = MOAIImage.new ()
erase:load ( 'numbers.png' )
erase = erase:resize ( CANVAS_WIDTH, CANVAS_HEIGHT )

brush = MOAIImage.new ()
brush:init ( 32 )
brush:fillCircle ( 16, 16, 15, 1, 1, 1, 0.5 )
brush:fillCircle ( 16, 16, 10, 0, 1, 1, 1 )
brush = brush:resize ( BRUSH_SIZE )

canvas = MOAIImageTexture.new ()
canvas:load ( erase )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( canvas )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )

layer:insertProp ( prop )

gfxQuad:setRect ( 0, 0, PROP_WIDTH, PROP_HEIGHT )
prop:setPiv ( PROP_WIDTH / 2, PROP_HEIGHT / 2 )	

function getPointerLocInProp ( x, y )

	local x, y = prop:worldToModel ( layer:wndToWorld ( x, y ))
	return x, y
end

function stamp ( x, y, source )

	print ( 'STAMP', x, y )

	local w, h = BRUSH_SIZE, BRUSH_SIZE
	x, y = x * ( CANVAS_WIDTH / PROP_WIDTH ), y *  ( CANVAS_HEIGHT / PROP_HEIGHT )

	local x1 = x - ( w * 0.5 )
	local y1 = y - ( h * 0.5 )
	local x2 = x1 + w
	local y2 = y1 + h

	-- copy only the RGB channels from the source image
	brush:copyRect ( source, x1, y1, x2, y2, 0, 0, w, h,
		MOAIImage.FILTER_LINEAR,
		MOAIImage.BLEND_FACTOR_1110,
		MOAIImage.BLEND_FACTOR_0001,
		MOAIImage.BLEND_EQ_ADD
	)

	-- blend the brush with the dest image using the brush's alpha channel
	canvas:copyRect ( brush, 0, 0, w, h, x1, y1, x2, y2,
		MOAIImage.FILTER_LINEAR,
		MOAIImage.BLEND_FACTOR_SRC_ALPHA,
		MOAIImage.BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		MOAIImage.BLEND_EQ_ADD
	)
	canvas:invalidate ( x1, y1, x2, y2 )
end

function onMouse ()

	local x, y = getPointerLocInProp ( MOAIInputMgr.device.pointer:getLoc ())

	if MOAIInputMgr.device.mouseLeft:isDown () then
		stamp ( x, y, draw )
	end

	if MOAIInputMgr.device.mouseRight:isDown () then
		stamp ( x, y, erase )
	end
end

if MOAIInputMgr.device.pointer then
	MOAIInputMgr.device.pointer:setCallback ( onMouse )
	MOAIInputMgr.device.mouseLeft:setCallback ( onMouse )
	MOAIInputMgr.device.mouseRight:setCallback ( onMouse )
end

function onTouch ()
	local touch = MOAIInputMgr.device.touch
	local x, y = getPointerLocInProp ( touch:getCenterLoc ())
	stamp ( x, y, touch:countTouches () == 1 and draw or erase )
end

if MOAIInputMgr.device.touch then
	MOAIInputMgr.device.touch:setCallback ( onTouch )
end
