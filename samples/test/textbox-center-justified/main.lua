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

font1 = MOAIFont.new ()
font1:load ( 'arial-rounded.TTF' )

text = 'This text is center justified. This text is center justified.'
local textbox2 = MOAITextBox.new ()
textbox2:setString ( text )
textbox2:setFont ( font1 )
textbox2:setTextSize ( 12, 163 )
textbox2:setRect ( -150, -90, 150, 70 )
textbox2:setAlignment ( MOAITextBox.CENTER_JUSTIFY )
textbox2:setYFlip ( true )
layer:insertProp ( textbox2 )
