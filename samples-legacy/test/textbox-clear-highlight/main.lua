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

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )

continue = false

function setHighlightTests ()
	while 1 do
		text = '1234567890 1234567890 1234567890 1234567890'
		
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setFont ( font1 )
		textbox:setTextSize ( 12, 163 )
		textbox:setRect ( -150, 70, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		textbox:setHighlight ( 12, 10, 1, 1, 0 )
		
		continue = false
		repeat coroutine.yield () until continue
		
		print ( "Highlights cleared" )
		textbox:clearHighlights ()
		
		continue = false
		repeat coroutine.yield () until continue
		
		print ( "Next page" )
		textbox:nextPage ()
		
		continue = false
		repeat coroutine.yield () until continue
		textbox:setReveal ( 0 )
	end
end

-- tests
function onKeyboardEvent ( key, down )
	if down and key == MOAIKeyCode.SPACE then
		continue = true
	end
end

MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setHighlightTests )
