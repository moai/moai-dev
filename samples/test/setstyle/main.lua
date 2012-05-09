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

font2 = MOAIFont.new ()
font2:load ( 'Dwarves.TTF' )

font3 = MOAIFont.new ()
font3:load ( 'EBOLA-KI.TTF' )

continue = false

function setStyleTests ()
	while 1 do
		text = 'This is <green>some test</> text to print out while testing different styles.'
		
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setRect ( -150, -230, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		local style1 = MOAITextStyle.new ()
		style1:setColor ( 1, 0, 0, 1 )
		style1:setFont ( font1 )
		style1:setSize ( 24 )
		textbox:setStyle ( style1 )
		
		local style2 = MOAITextStyle.new ()
		style2:setColor ( 0, 1, 0, 1 )
		style2:setFont ( font2 )
		style2:setSize ( 24 )
		textbox:setStyle ( 'green', style2 )
		
		continue = false
		repeat coroutine.yield () until continue
		print ( "switch to broken text" )
		
		style1:setFont ( font2 )
		
		continue = false
		repeat coroutine.yield () until continue
		print ( "switch to broken text" )
		
		style1:setFont ( font1 )
		
		continue = false
		repeat coroutine.yield () until continue
		print ( "switch to original" )
		textbox:setReveal ( 0 )
	end
end

-- tests
function onKeyboardEvent ( key, down )
	if down == true then
		if key == 32 then -- continue
			continue = true
		end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setStyleTests )
