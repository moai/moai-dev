----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- build the font
charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog.'

-- load and show the font
MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- load the font image back in
image = MOAIImage.new ()
image:load ( 'font.png' )

-- read the font in from the file
file = io.open ( 'font.lua', 'r' )
font = loadstring ( file:read ( '*all' ))()
file:close ()

-- set the font image
font:setImage ( image )

-- font now ready for use
textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setShader ( MOAIShaderMgr.getShader ( MOAIShaderMgr.DECK2D_SHADER ))
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
