----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL, 1, 1, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_BASELINES, 1, 1, 0, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_LINES_LAYOUT_BOUNDS, 1, 0, 0, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog.'

font = MOAIFont.new ()

bitmapFontReader = MOAIBitmapFontReader.new ()
bitmapFontReader:loadPage ( 'FontVerdana18.png', charcodes, 16 )
font:setReader ( bitmapFontReader )

glyphCache = MOAIDynamicGlyphCache.new ()
glyphCache:setColorFormat ( MOAIImage.COLOR_FMT_RGBA_8888 )
font:setCache ( glyphCache )

label = MOAITextLabel.new ()
label:setString ( text )
label:setFont ( font )
label:setTextSize ( 16 )
label:setRect ( -150, -230, 150, 230 )
label:setRectLimits ( true, true )
label:setAlignment ( MOAITextLabel.CENTER_JUSTIFY, MOAITextLabel.CENTER_JUSTIFY )
label:setYFlip ( true )
label:setPartition ( layer )

