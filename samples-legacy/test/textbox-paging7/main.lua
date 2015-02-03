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

font = MOAIFont.new ()
font:load ( 'arial-rounded.TTF' )

continue = false

function pageTest1 ()
	text = '1This is long text that will take multiple pages. This is long text that will take multiple 2pages. This is long text that will take multiple pages. This is long text that will take 3multiple pages. This is long text that will take multiple pages.'
	
	local textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	textbox:setRect ( -150, 100, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
		
	while 1 do
		continue = false
		repeat coroutine.yield () until continue
		
		textbox:nextPage ()
	end
end

function pageTest2 ()
	text = 'This text is curved. This text is curved. This text is curved. This text is curved. This text is curved. This text is curved.'
	
	textbox = MOAITextBox.new ()
	textbox:setString ( text )
	textbox:setFont ( font )
	textbox:setTextSize ( 12, 163 )
	font:setDefaultSize ( 12, 163 )
	textbox:setRect ( -150, 70, 150, 230 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )
		
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

-- change this to pageTest1 or pageTest2
thread:run ( pageTest2 )
