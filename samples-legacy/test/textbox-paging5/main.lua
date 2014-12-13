----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

font = MOAIFont.new ()
font:load ( 'arial-rounded.TTF' )

function setStyleTests ()
	text = '\n\n\n\n\n\n\n\n\n\n\n\n\n\nProin suscipit, \nmauris nec \nvolutpat'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setRect ( -150, -230, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
	
	local style = MOAITextStyle.new ()
	style:setColor ( 1, 1, 1, 1 )
	style:setFont ( font )
	style:setSize ( 24 )
	textbox:setStyle ( style )
	
	while 1 do
		continue = false
		repeat coroutine.yield () until continue
		
		textbox:nextPage ()
	end
end

function onKeyboardEvent ( key, down )
	if down and key == MOAIKeyCode.SPACE then
		continue = true
	end
end

MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setStyleTests )
