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

VIEW_WIDTH = minPow2 ( VIEW_WIDTH )
VIEW_HEIGHT = minPow2 ( VIEW_HEIGHT )

CANVAS_WIDTH = 2048
CANVAS_HEIGHT = 2048

MASK_WIDTH = 128
MASK_HEIGHT = 128

BRUSH_SIZE = 16

MOAISim.openWindow ( "test", VIEW_WIDTH, VIEW_HEIGHT )

viewport = MOAIViewport.new ()
viewport:setSize ( VIEW_WIDTH, VIEW_HEIGHT )
viewport:setScale ( VIEW_WIDTH, -VIEW_HEIGHT )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

file = assert ( io.open ( 'shader.vsh', mode ))
vsh = file:read ( '*all' )
file:close ()

file = assert ( io.open ( 'shader.fsh', mode ))
fsh = file:read ( '*all' )
file:close ()

program = MOAIShaderProgram.new ()

program:setVertexAttribute ( 1, 'position' )
program:setVertexAttribute ( 2, 'uv' )
program:setVertexAttribute ( 3, 'color' )

program:reserveUniforms ( 3 )
program:declareUniformSampler ( 1, 'sampler0', 1 )
program:declareUniformSampler ( 2, 'sampler1', 2 )
program:declareUniformSampler ( 3, 'sampler2', 3 )

program:load ( vsh, fsh )

shader = MOAIShader.new ()
shader:setProgram ( program )

draw = MOAIImage.new ()
draw:load ( 'moai.png' )
draw = draw:resize ( CANVAS_WIDTH, CANVAS_HEIGHT )

erase = MOAIImage.new ()
erase:load ( 'numbers.png' )
erase = erase:resize ( CANVAS_WIDTH, CANVAS_HEIGHT )

brush = MOAIImage.new ()
brush:init ( 16, 16, MOAIImage.COLOR_FMT_A_8 )
brush:fillCircle ( 8, 8, 7, 0, 0, 0, 0.25 )
brush:fillCircle ( 8, 8, 6, 0, 0, 0, 0.5 )
brush:fillCircle ( 8, 8, 5, 0, 0, 0, 0.75 )
brush:fillCircle ( 8, 8, 4, 0, 0, 0, 1 )
brush = brush:resize ( BRUSH_SIZE )

multitexture = MOAIMultiTexture.new ()
multitexture:reserve ( 3 )

mask = MOAIImageTexture.new ()
mask:init ( MASK_WIDTH, MASK_HEIGHT, MOAIImage.COLOR_FMT_A_8 )
mask:setFilter ( MOAITexture.GL_LINEAR )
multitexture:setTexture ( 1, mask )

texture1 = MOAITexture.new ()
texture1:load ( "moai.png" )
texture1:setFilter ( MOAITexture.GL_LINEAR )
multitexture:setTexture ( 2, texture1 )

texture2 = MOAITexture.new ()
texture2:load ( "numbers.png" )
texture2:setFilter ( MOAITexture.GL_LINEAR )
multitexture:setTexture ( 3, texture2 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( multitexture )
gfxQuad:setRect ( 0, 0, VIEW_WIDTH, VIEW_HEIGHT )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setPiv ( VIEW_WIDTH / 2, VIEW_HEIGHT / 2 )	
prop:setShader ( shader )
layer:insertProp ( prop )

function stamp ( x, y, erase )

	local w, h = BRUSH_SIZE, BRUSH_SIZE
	x, y = x * ( MASK_WIDTH / VIEW_WIDTH ), y *  ( MASK_HEIGHT / VIEW_HEIGHT )

	local x1 = x - ( w * 0.5 )
	local y1 = y - ( h * 0.5 )
	local x2 = x1 + w
	local y2 = y1 + h

	-- blend the brush with the dest image using the brush's alpha channel
	mask:copyRect ( brush, 0, 0, w, h, x1, y1, x2, y2,
		MOAIImage.FILTER_LINEAR,
		MOAIImage.BLEND_FACTOR_ONE,
		MOAIImage.BLEND_FACTOR_ONE,
		erase and MOAIImage.BLEND_EQ_SUB_INV or MOAIImage.BLEND_EQ_ADD
	)
	mask:invalidate ( x1, y1, x2, y2 )

	local r, g, b, a = mask:average ()
	print ( string.format ( 'MASK: %d%%', math.floor (( a * 100 ) + 0.5 )))
end

function onMouse ()

	local x, y = prop:worldToModel ( layer:wndToWorld ( MOAIInputMgr.device.pointer:getLoc ()))

	if MOAIInputMgr.device.mouseLeft:isDown () then
		stamp ( x, y, false )
	end

	if MOAIInputMgr.device.mouseRight:isDown () then
		stamp ( x, y, true )
	end
end

if MOAIInputMgr.device.pointer then
	MOAIInputMgr.device.pointer:setCallback ( onMouse )
	MOAIInputMgr.device.mouseLeft:setCallback ( onMouse )
	MOAIInputMgr.device.mouseRight:setCallback ( onMouse )
end

function onTouch ()
	local touch = MOAIInputMgr.device.touch
	local x, y = prop:worldToModel ( layer:wndToWorld ( touch:getCenterLoc ()))
	stamp ( x, y, touch:countTouches () > 1 )
end

if MOAIInputMgr.device.touch then
	MOAIInputMgr.device.touch:setCallback ( onTouch )
end
