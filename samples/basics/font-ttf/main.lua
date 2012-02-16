----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, -512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The <foo>quick</> brown <bar>fox</> jumps over the lazy dog.'

font = MOAIFont.new ()
font:load ( 'Dwarves.TTF' )
font:preloadGlyphs ( charcodes, 8 )
font:preloadGlyphs ( charcodes, 12 )
font:preloadGlyphs ( charcodes, 16 )
font:writePages ()

function newStyle ( font, size )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( size )
	return style;
end

textbox = MOAITextBox.new ()

--textbox:setStyle ( newStyle ( font, 8 ))
textbox:setStyle ( newStyle ( font, 12 ))
textbox:setStyle ( 'foo', newStyle ( font, 16 ))
textbox:setStyle ( 'bar', newStyle ( font, 8 ))
--textbox:setStyle ( newStyle ( font, 16 ))

textbox:setString ( text )
textbox:setRect ( -512, -128, 512, 128 )
--textbox:setYFlip ( true )
--textbox:setScl ( 0.5, 0.5 )
layer:insertProp ( textbox )
textbox:moveLoc ( -512, 0, 0, 3 )
