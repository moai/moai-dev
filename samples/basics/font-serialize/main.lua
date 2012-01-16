----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- build the font
charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog.'

font = MOAIFont.new ()
font:loadFromTTF ( 'times.ttf', charcodes, 12, 163 )

-- yank out the font image
image = font:getImage ()
image:writePNG ( 'font.png' )

-- add the font to the serializer
serializer = MOAISerializer.new ()
serializer:serialize ( font )

-- serialize out to a file
file = io.open ( 'font.lua', 'w' )
file:write ( serializer:exportToString ())
file:close ()

-- nothing up our sleeve...
image = nil
font = nil
file = nil

MOAISim.forceGarbageCollection ()

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
image:load ( 'font.png', 0 )

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
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
