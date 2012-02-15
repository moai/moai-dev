----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, -512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The qquick brown fox jumps over the lazy dog.'

font = MOAIFont.new ()
font:load ( 'Dwarves.TTF' )
font:preloadGlyphs ( charcodes, 32, 163 )
font:writePages ()

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
--textbox:setTextSize ( font:getScale ())
textbox:setRect ( -128, -128, 128, 128 )
--textbox:setYFlip ( true )
textbox:setScl ( 0.5, 0.5 )
layer:insertProp ( textbox )
