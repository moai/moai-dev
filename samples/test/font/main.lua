----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ("CAN I HAZ LUABURGER?")

--MOAIDebugLines.setStyle ( MOAIDebugLines.TEXT_BOX, 1, 1, 1, 1, 1 )

layer = MOAILayer2D.new ()
--layer:showDebugLines ()
MOAISim.pushRenderPass ( layer )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )
viewport:setOffset ( -1, 1 )
layer:setViewport ( viewport )

charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?!"
--charcodes = "jk"

font = MOAIFont.new ()
font:loadFromTTF ( "Bastarda-K.ttf", charcodes, 12, 163 )

--test = "THIS IS A \n TEST WITH A LOT OF TEXT SO I CAN TRY AND FIGURE OUT WHY THIS THING IS LOOKING WEIRD EVERY TIME I GO BOOP BOOP BLAH BLAHBLAHBLAH BALH BALHA BALHLAB BALHAL bAlh \n BLHA bLHABLHABLAH LABLhABAL ABLh"
--test = "THIS IS A TEST WITH A LOT OF TEXT SO I CAN TRY AND FIGURE OUT WHY THIS THING IS LOOKING WEIRD"
--test = charcodes
test = "This is a test with a lot of text so I can try and figure out why this thing is looking weird every time..."
--test = "T.W. Lewis"

textbox = MOAITextBox.new ()
textbox:setString ( test )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale ())
textbox:setRect ( 0, 0, 320, 480 )
layer:insertProp ( textbox )

shader = MOAISimpleShader.new ()
shader:setColor ( 0, 0, 0, 1 )
shader:seekColor ( 1, 0, 0, 1, 1.5 )
textbox:setShader ( shader )

textbox:spool ()

MOAISim.openWindow ( "textbox", 320, 480 )
