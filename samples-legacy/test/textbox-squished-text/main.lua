----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!@#$%^&*():;'/-+\""
text = 'The quick brown fox jumps over the lazy dog. 0123456789'

font = MOAIFont.new ()
font:load ( 'SCREEBIE.TTF' )
font:preloadGlyphs ( charcodes, 24 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 24 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
