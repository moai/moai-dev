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
viewport:setScale ( 51, -51 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The <foo>quick</> brown <bar>fox</> jumps over the lazy dog.'

font = MOAIFont.new ()
font:load ( 'Dwarves.TTF' )
font:preloadGlyphs ( charcodes, 24/10 )
font:preloadGlyphs ( charcodes, 32/10 )
font:preloadGlyphs ( charcodes, 42/10 )

function newStyle ( font, size )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
    style:setScale( 1/10 )
	style:setSize ( size, 72*10 )
	return style;
end

function createTextbox()
	local textbox = MOAITextBox.new ()
	
	--textbox:setStyle ( newStyle ( font, 8 ))
	textbox:setStyle ( newStyle ( font, 24/10 ))
	textbox:setStyle ( 'foo', newStyle ( font, 32/10 ))
	textbox:setStyle ( 'bar', newStyle ( font, 42/10 ))
	--textbox:setStyle ( newStyle ( font, 16 ))
	
	textbox:setString ( text )
	textbox:spool ()
	textbox:setRect ( -128/10, -128/10, 128/10, 128/10 )
	textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
	--textbox:setYFlip ( true )
	--textbox:setScl ( 0.5, 0.5 )
	layer:insertProp ( textbox )
	--textbox:moveLoc ( -512, 0, 0, 3 )
	return textbox
end

textboxSnap = createTextbox()
textboxNoSnap = createTextbox()
textboxNoSnap:setSnapToViewportScale(false)
textboxNoSnap:setColor(1, 0, 0, 1)
