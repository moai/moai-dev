----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )

continue = false

function setStyleTests ()
	while 1 do
		text = '1This is long text that will take multiple pages. This is long text that will take 2multiple pages. This is long text that will take multiple pages. This is long text that 3will take multiple pages. This is long text that will take multiple pages.'
		
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setFont ( font )
		textbox:setTextSize ( 12, 163 )
		textbox:setRect ( -150, -230, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		local style = MOAITextStyle.new ()
		style:setColor ( 1, 1, 1, 1 )
		style:setFont ( font1 )
		style:setSize ( 24 )
		textbox:setStyle ( style )
		
		continue = false
		repeat coroutine.yield () until continue
		
		textbox:nextPage ()
		
		continue = false
		repeat coroutine.yield () until continue
		textbox:setReveal ( 0 )
	end
end

-- tests
function onKeyboardEvent ( key, down )
	if down then
		if key == 32 then -- continue
			continue = true
		else
			return
		end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setStyleTests )
