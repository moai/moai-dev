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

MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_LAYOUT, 1, 0, 0, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

font = MOAIFont.new ()
font:load ( 'arial-rounded.TTF' )

text = '1234567890 1234567890 1234567890 1234567890 1234567890 1234567890'
	
local textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, 70, -50, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )
