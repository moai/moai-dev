----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_GLYPHS, 1, 0, 0, 1, 1 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

renderGlyph = function ( font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	print ( 'GLYPH:', font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	image:fillRect ( xMin, yMin, xMax, yMax, 0, 0, 1, 1 )
	reader:setPenColor ( 0, 1, 1, 1 )
	reader:renderGlyph ( image, x, y, MOAIImage.BLEND_FACTOR_ONE, MOAIImage.BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, MOAIImage.BLEND_EQ_ADD )
end

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:setListener ( MOAIFont.EVENT_RENDER_GLYPH, renderGlyph )

cache = font:getCache ()
cache:setColorFormat ( MOAIImage.COLOR_FMT_RGBA_8888 )
cache:setPadding ( 20, 20 )

font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 50 )

image = font:getImage ()
image:writePNG ( 'font.png' )

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setText ( 'A B C D E' )
textbox:setFont ( font )
textbox:setRect ( -150, -230, 150, 230 )

textbox:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.DECK2D_SHADER ))
textbox:setBlendMode ( MOAIGraphicsProp.ZGL_BLEND_FACTOR_ONE, MOAIGraphicsProp.ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA )

textbox:setYFlip ( true )

style = textbox:getStyle ()
style:setPadding ( 10, 10 )

layer:insertProp ( textbox )
