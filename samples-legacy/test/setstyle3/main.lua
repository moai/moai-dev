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

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )
font1:preloadGlyphs ( charcodes, 12 )

font2 = MOAIFont.new ()
font2:load ( 'Dwarves.TTF' )
font2:preloadGlyphs ( charcodes, 24 )

font3 = MOAIFont.new ()
font3:load ( 'EBOLA-KI.TTF' )
font3:preloadGlyphs ( charcodes, 36 )

continue = false

function setStyleTests ()
	while 1 do
		text = 'This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</> This is <green>some test</> text to <yellow><blue>print out while</> testing different styles.</>'
		
		local textbox = MOAITextBox.new ()
		textbox:setString ( text )
		textbox:setRect ( -150, 70, 150, 230 )
		textbox:setYFlip ( true )
		layer:insertProp ( textbox )
		
		local style1 = MOAITextStyle.new ()
		style1:setFont ( font1 )
		style1:setSize ( 12 )
		textbox:setStyle ( style1 )
		textbox:setStyle ( 'red', style1 )
		
		local style2 = MOAITextStyle.new ()
		style2:setFont ( font2 )
		style2:setSize ( 24 )
		textbox:setStyle ( 'green', style2 )
		
		local style3 = MOAITextStyle.new ()
		style3:setFont ( font3 )
		style3:setSize ( 36 )
		textbox:setStyle ( 'blue', style3 )
		
		local style4 = MOAITextStyle.new ()
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
