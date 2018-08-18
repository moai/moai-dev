----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL, 1, 0, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_LIMITS, 2, 1, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_BASELINES, 1, 1, 0, 1, 1 )
--MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_GLYPHS, 1, 0.5, 0, 0, 1 )
--MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_GLYPH_BOUNDS, 2, 1, 1, 0, 1 )
--MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_LINES_GLYPH_BOUNDS, 1, 1, 0, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_LINES_LAYOUT_BOUNDS, 1, 1, 1, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_MARGINS, 1, 0, 1, 1, 1 )


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

--text = '<foo>A</><bar>B</><baz>C</>\n<baz>D</><bar>E</><foo>F</>'

--text = 'abcdefghijklmnop'
--text = 'abcdefg hijklmnop qrstuv wxyz 0123456789 aaaaaa bbbbbb cccccc dddddd eeeeee'
text = 'aaa bbb ccc ddd eee fff ggg hhh iii jjj kkk lll mmm nnn ooo ppp qqq rrr sss ttt uuu vvv www xxx yyy zzz'
--text = 'g\n\n\nqyjp'

font = MOAIFont.new ()
font:loadFromTTF ( '../resources/fonts/arial-rounded.TTF' )

label = MOAITextLabel.new ()

label:setStyle ( 'foo', newStyle ( font, 24 ))
label:setStyle ( 'bar', newStyle ( font, 32 ))
label:setStyle ( 'baz', newStyle ( font, 42 ))

label:setString ( text )
label:setFont ( font )
label:setTextSize ( 32 )
label:setLineSpacing ( 0 )
--label:setYFlip ( true )
--label:setRect ( -128, -128, 128, 128 )
--label:setRect ( -64, -128, 64, 128 )
label:setRect ( -96, -160, 96, 160 )
label:setRectLimits ( true, true )
label:setMargins ( 32, 32, 32, 32 )
label:setAlignment ( MOAITextLabel.CENTER_JUSTIFY, MOAITextLabel.TOP_JUSTIFY )
label:setOverrunRules ( MOAITextLabel.OVERRUN_SPLIT_WORD, MOAITextLabel.OVERRUN_MOVE_WORD )
label:setPartition ( layer )

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( '../resources/tiny-x.png' )
gfxQuad:setRect ( -4, -4, 4, 4 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

local prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )
