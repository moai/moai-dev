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
text = 'The <style>quick</> brown fox jumps over the lazy dog.'

font = MOAIFont.new ()
font:load ( 'Dwarves.TTF' )
font:preloadGlyphs ( charcodes, 24 )
font:preloadGlyphs ( charcodes, 32 )
font:preloadGlyphs ( charcodes, 42 )

style = MOAITextStyle.new ()
style:setFont ( font )
style:setSize ( 32 )

textbox = MOAITextBox.new ()

textbox:setFont ( font )
textbox:setTextSize ( 24 )
textbox:setStyle ( 'style', style )

textbox:setString ( text )
textbox:spool ()
textbox:setRect ( -128, -128, 128, 128 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
layer:insertProp ( textbox )

function onMouseLeftEvent ( down )

	if down == true then
		style:setSize ( 42 )
	else
		style:setSize ( 32 )
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )

function onMouseRightEvent ( down )

	if down == true then
		style:setColor ( 1, 0, 0, 1 )
	else
		style:setColor ( 1, 1, 1, 1 )
	end
end

MOAIInputMgr.device.mouseRight:setCallback ( onMouseRightEvent )
