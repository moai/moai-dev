----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

MOAIGfxDevice.setClearColor ( 1, 1, 1, 1 )
viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, -512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brownfox jumps over the lazy dog.'

font = MOAIFont.new ()
print ( "test lua" )
font:loadFromBMFont ( 'CopperPlateGothic.fnt' )
font:load ( 'Dwarves.TTF' )
print ( "test lua2" )
--font:preloadGlyphs ( charcodes, 24 )
--font:preloadGlyphs ( charcodes, 32 )
font:preloadGlyphs ( charcodes, 64 )
print ( "test lua3" )

function newStyle ( font, size )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( size )
	return style;
end

textbox = MOAITextBox.new ()

--textbox:setStyle ( newStyle ( font, 8 ))
textbox:setStyle ( newStyle ( font, 64 ))
--textbox:setStyle ( 'foo', newStyle ( font, 32 ))
--textbox:setStyle ( 'bar', newStyle ( font, 42 ))
--textbox:setStyle ( newStyle ( font, 16 ))

textbox:setString ( text )
textbox:spool ()
textbox:setRect ( -128, -128, 128, 128 )
textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
textbox:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.DECK2D_SHADER ))
--textbox:setYFlip ( true )
--textbox:setScl ( 0.5, 0.5 )
layer:insertProp ( textbox )
print ( "test lua4" )
--textbox:moveLoc ( -512, 0, 0, 3 )
