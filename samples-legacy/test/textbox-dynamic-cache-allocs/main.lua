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

function glyphAlloc ()
	text = "Hello 123456789 qwertyuiopasdfghjklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM"
	local textbox = MOAITextBox.new ()
	textbox:setFont ( font )
	textbox:setString ( "as" )
	textbox:setTextSize ( 64 )
	textbox:setWordBreak ( MOAITextBox.WORD_BREAK_CHAR )
	textbox:setRect ( -160, -240, 160, 240 )
	textbox:setYFlip ( true )
	layer:insertProp ( textbox )

	for i = 1, #text do
    	textbox:setString ( text:sub ( 1, i ))
    	coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( glyphAlloc )
