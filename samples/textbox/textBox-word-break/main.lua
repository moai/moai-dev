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
text = 'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.'
--text = 'ABCDEFGHIJKLMNOP'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 32 )

local textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 32 )
textbox:setRect ( -150, -240, 150, 240 )
textbox:setYFlip ( true )
textbox:setWordBreak ( MOAITextBox.WORD_BREAK_CHAR )
layer:insertProp ( textbox )

