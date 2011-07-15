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
text = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas diam augue, laoreet vitae dapibus in, sodales in ligula. Nullam dapibus velit vel lectus pretium at ultricies massa bibendum. Nulla cursus suscipit malesuada. Donec sit amet nisl ac sem venenatis dictum.'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF', charcodes, 12, 163 )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

textbox:spool ()

