----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

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

multitexture = MOAIMultiTexture.new ()
multitexture:reserve ( 3 )

mask = MOAIImageTexture.new ()
mask:init ( 64, 64, MOAIImage.COLOR_FMT_A_8 )
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
gfxQuad:setRect ( 0, 0, 256, 256 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setPiv ( 128, 128 )
prop:setShader ( shader )
layer:insertProp ( prop )

brush = MOAIImage.new ()
brush:init ( 16, 16, MOAIImage.COLOR_FMT_A_8 )
brush:fillCircle ( 8, 8, 7, 0, 0, 0, 0.25 )
brush:fillCircle ( 8, 8, 6, 0, 0, 0, 0.5 )
brush:fillCircle ( 8, 8, 5, 0, 0, 0, 0.75 )
brush:fillCircle ( 8, 8, 4, 0, 0, 0, 1 )

function stamp ( x, y, erase )

	local w, h = brush:getSize ()
	
	x, y = x / 4, y / 4

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

MOAIInputMgr.device.pointer:setCallback ( onMouse )
MOAIInputMgr.device.mouseLeft:setCallback ( onMouse )
MOAIInputMgr.device.mouseRight:setCallback ( onMouse )
