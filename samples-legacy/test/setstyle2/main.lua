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

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )
font1:preloadGlyphs ( charcodes, 12 )

font2 = MOAIFont.new ()
font2:load ( 'Dwarves.TTF' )
font2:preloadGlyphs ( charcodes, 24 )

continue = false

function setStyleTests ()
	while 1 do
		text = 'This is some test text to print out while testing different styles.'
		
		print ( "Set the default style" )
		
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setRect ( -150, 70, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		local style1 = MOAITextStyle.new ()
		style1:setColor ( 1, 0, 0, 1 )
		style1:setFont ( font1 )
		style1:setSize ( 12 )
		textbox:setStyle ( style1 )
		
		continue = false
		repeat coroutine.yield () until continue
		print ( "Change the font and size of the default style without calling setStyle" )
		
		style1:setFont ( font2 )
		style1:setSize ( 24 )
		
		continue = false
		repeat coroutine.yield () until continue
		print ( "Call setStyle to make an unused style" )
		
		textbox:setStyle ( 'red', style1 )
		
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
thread:run ( setStyleTests )
