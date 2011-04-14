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

charcodes = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõö÷øùúûüışÿ"
font = MOAIFont.new ()
font2 = MOAIFont.new ()
font:loadFromTTF ( "../ArcadeClassic.ttf", charcodes, 16, 163 )
font2:loadFromTTF ( "../perpetua.ttf", charcodes, 16, 163 )

textbox = MOAITextBox.new ()
textbox:setRect ( -480, -320, 0, 320 )
textbox:setScl ( 1, -1 )
textbox:setFont ( font )
textbox:setTextSize ( font:getScale () )
textbox:setString ( "This is a font stress test for Moai!\n  Unicode follows\n ¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ ĞÑÒÓÔÕÖ×ØÙÚÛÜİŞ ßàáâãäåæçèéêëìíîï ğñòóôõö÷øùúûüışÿ " )
textbox2 = MOAITextBox.new ()
textbox2:setRect ( 0, -320, 480, 320 )
textbox2:setScl ( 1, -1 )
textbox2:setFont ( font2 )
textbox2:setTextSize ( font2:getScale () )
textbox2:setString ( "This is a font stress test for Moai!\n  Unicode follows\n ¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ ĞÑÒÓÔÕÖ×ØÙÚÛÜİŞ ßàáâãäåæçèéêëìíîï ğñòóôõö÷øùúûüışÿ " )
layer:insertProp ( textbox )
layer:insertProp ( textbox2 )

MOAISim.openWindow ( "stress test", 960, 640 )
