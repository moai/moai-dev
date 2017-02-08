----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

text = 'This is a label.'

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF' )

label = MOAITextLabel.new ()
label:setString ( text )
label:setFont ( font )
label:setTextSize ( 32 )
label:setYFlip ( true )
label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )
layer:insertProp ( label )
