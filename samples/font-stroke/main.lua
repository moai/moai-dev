----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

function newStyle ( font, size, r, g, b, a )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( size )
	style:setColor ( r or 1, g or 1, b or 1, a or 1 )
	return style;
end

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

renderGlyph = function ( font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	print ( 'GLYPH:', font, reader, image, code, x, y, xMin, yMin, xMax, yMax )
	
	reader:setBlendMode ( MOAIImage.BLEND_EQ_ADD, MOAIImage.BLEND_FACTOR_ONE, MOAIImage.BLEND_FACTOR_ONE_MINUS_SRC_ALPHA )

	reader:setPenColor ( 0, 1, 1, 1 )
	reader:renderGlyph ( image, x, y )

	reader:setPenColor ( 1, 1, 1, 1 )
	reader:strokeGlyph ( image, x, y, 2 )
end

font = MOAIFont.new ()
font:setListener ( MOAIFont.EVENT_RENDER_GLYPH, renderGlyph )

cache = font:getCache ()
cache:setColorFormat ( MOAIImage.COLOR_FMT_RGBA_8888 )
cache:setPadding ( 4 )

font:loadFromTTF ( '../resources/fonts/arial-rounded.TTF' )

label = MOAITextLabel.new ()
label:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.DECK2D_SHADER ))
label:setBlendMode ( MOAIGraphicsProp.ZGL_BLEND_FACTOR_ONE, MOAIGraphicsProp.ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA )
label:setString ( 'ABC' )
label:setFont ( font )
label:setTextSize ( 128 )
label:setLineSpacing ( 0 )
label:setAlignment ( MOAITextLabel.CENTER_JUSTIFY, MOAITextLabel.CENTER_JUSTIFY )
label:setOverrunRules ( MOAITextLabel.OVERRUN_SPLIT_WORD, MOAITextLabel.OVERRUN_MOVE_WORD )

style = label:getStyle ()
style:setPadding ( 4 )

label:setPartition ( layer )
