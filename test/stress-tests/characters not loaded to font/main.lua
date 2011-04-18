----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 960, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!"
font = MOAIFont.new ()
font:loadFromTTF ( "../ArcadeClassic.ttf", charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setRect ( -480, -320, 480, 320 )
textbox:setScl ( 1, -1 )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale () )
-- textbox:setString ( "This is a font stress test for Moai!  Ignore the spacing!" )
textbox:setString ( "This is a font stress test for Moai!  Ignore the spacing!  The characters used after this are not supported by the font ,.()@/*+-=_'\"" )
layer:insertProp ( textbox )

MOAISim.openWindow ( "stress test", 960, 640 )
