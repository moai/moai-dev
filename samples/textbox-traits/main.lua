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
font:loadFromTTF ( 'Bastarda-K.ttf', charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 16, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

textbox:spool ()

color = MOAIColor.new ()
color:setColor ( 0, 0, 0, 1 )
color:seekColor ( 1, 0, 0, 1, 3, MOAIEaseType.EASE_IN )
textbox:setAttrLink ( MOAITextBox.INHERIT_COLOR, color, MOAIColor.COLOR_TRAIT )

