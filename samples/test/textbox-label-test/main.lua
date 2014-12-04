----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX_BASELINES, 1, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

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

print("------------------------------");
print("getDims:", label:getDims());
print("getRect:", label:getRect());
print("getBounds:", label:getBounds());
print("getTextBounds:", label:getTextBounds());

label = MOAITextLabel.new ()
label:setString ( text )
label:setFont ( font )
--label:setRect(0, 0, 240, 50)
label:setBounds(0, 0, 0, 240, 50, 0)
label:setLoc ( -120, 50, 0 )
label:setTextSize ( 32 )
label:setYFlip ( true )
layer:insertProp ( label )

print("------------------------------");
print("getDims:", label:getDims());
print("getRect:", label:getRect());
print("getBounds:", label:getBounds());
print("getTextBounds:", label:getTextBounds());

label = MOAITextLabel.new ()
label:setString ( text )
label:setFont ( font )
label:setRect(-120, -25, 120, 25)
label:setLoc (0, 140, 0 )
label:setTextSize ( 32 )
label:setYFlip ( true )
label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.BASELINE_JUSTIFY )
layer:insertProp ( label )

print("------------------------------");
print("getDims:", label:getDims());
print("getRect:", label:getRect());
print("getBounds:", label:getBounds());
print("getTextBounds:", label:getTextBounds());

