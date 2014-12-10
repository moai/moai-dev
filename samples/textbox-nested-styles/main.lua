----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, -512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The <foo>quick brown <bar>fox</> jumps</> over <c:f70>the</> lazy dog.'

font = MOAIFont.new ()
font:load ( 'Dwarves.TTF' )
font:preloadGlyphs ( charcodes, 24 )
font:preloadGlyphs ( charcodes, 32 )
font:preloadGlyphs ( charcodes, 42 )

function newStyle ( font, size, r, g, b )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( size )
	style:setColor ( r, g, b, 1 )
	return style;
end

textbox = MOAITextBox.new ()

textbox:setStyle ( newStyle ( font, 24, 1, 1, 1 ))
textbox:setStyle ( 'foo', newStyle ( font, 32, 0, 1, 0 ))
textbox:setStyle ( 'bar', newStyle ( font, 42, 1, 0, 0 ))

textbox:setString ( text )
textbox:spool ()
textbox:setRect ( -128, -128, 128, 128 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( textbox )
