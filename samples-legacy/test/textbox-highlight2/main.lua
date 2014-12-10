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

text = '123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789'

local textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font1 )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, 70, 130, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

textbox:setHighlight ( 1, 5, 1, 0, 0 )
textbox:setHighlight ( 5, 5, 1, 0, 0 )
