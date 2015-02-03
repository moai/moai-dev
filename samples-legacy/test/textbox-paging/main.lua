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
font1:preloadGlyphs ( charcodes, 24 )

font2 = MOAIFont.new ()
font2:load ( 'Dwarves.TTF' )
font2:preloadGlyphs ( charcodes, 24 )

font3 = MOAIFont.new ()
font3:load ( 'EBOLA-KI.TTF' )
font3:preloadGlyphs ( charcodes, 36 )

continue = false

function setStyleTests ()
	while 1 do
		text = '.\n.\n.\n.\n.\n.\n.\n.\n.\n.\n.\n.\n.\n.\
			. . . . . . . . . . . . . . . Proin <yellow>suscipit, mauris <green>nec volutpat <blue>congue, orci sapien posuere arcu,</></> vitae</> tempor massa neque nec metus'
		
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
		
		local style2 = MOAITextStyle.new ()
		style2:setColor ( 0, 1, 0, 1 )
		style2:setFont ( font2 )
		style2:setSize ( 24 )
		textbox:setStyle ( 'green', style2 )
		
		local style3 = MOAITextStyle.new ()
		style3:setColor ( 0, 0, 1, .5 )
		style3:setFont ( font3 )
		style3:setSize ( 36 )
		textbox:setStyle ( 'blue', style3 )
		
		local style4 = MOAITextStyle.new ()
		style4:setColor ( 1, 1, 0, 1 )
		style4:setFont ( font1 )
		style4:setSize ( 12 )
		textbox:setStyle ( 'yellow', style4 )
		
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
	if down and key == MOAIKeyCode.SPACE then
		continue = true
	end
end

MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardEvent )
thread = MOAIThread.new ()
thread:run ( setStyleTests )
