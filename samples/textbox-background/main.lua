----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

MOAIDebugLinesMgr.setStyle ( MOAITextLabel.DEBUG_DRAW_TEXT_LABEL_LIMITS, 2, 1, 1, 1, 1 )

function newStyle ( font, size, r, g, b, a )
	local style = MOAITextStyle.new ()
	style:setFont ( font )
	style:setSize ( size )
	style:setColor ( r or 1, g or 1, b or 1, a or 1 )
	return style;
end

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

text = 'aaa bbb ccc ddd eee fff ggg hhh iii jjj kkk lll mmm nnn ooo ppp qqq rrr sss ttt uuu vvv www xxx yyy zzz'

font = MOAIFont.new ()
font:loadFromTTF ( '../resources/fonts/arial-rounded.TTF' )

label = MOAITextLabel.new ()

label:setDeck ( '../resources/numbers.png' )

label:setString ( text )
label:setFont ( font )
label:setTextSize ( 32 )
label:setLineSpacing ( 0 )
label:setYFlip ( true )
label:setRect ( -96, -160, 96, 160 )
label:setRectLimits ( true, true )
label:setMargins ( 32, 32, 32, 32 )
label:setAlignment ( MOAITextLabel.CENTER_JUSTIFY, MOAITextLabel.TOP_JUSTIFY )
label:setOverrunRules ( MOAITextLabel.OVERRUN_SPLIT_WORD, MOAITextLabel.OVERRUN_MOVE_WORD )
label:setPartition ( layer )
