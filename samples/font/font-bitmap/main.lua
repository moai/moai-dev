----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog.'

font = MOAIFont.new ()

bitmapFontReader = MOAIBitmapFontReader.new ()
bitmapFontReader:loadPage ( 'FontVerdana18.png', charcodes, 16 )
font:setReader ( bitmapFontReader )

glyphCache = MOAIGlyphCache.new ()
glyphCache:setColorFormat ( MOAIImage.COLOR_FMT_RGBA_8888 )
font:setCache ( glyphCache )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 16 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

