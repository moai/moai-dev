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

font = MOAIFont.new ()
font:load ( 'arial-rounded.TTF' )

text = 'This </>is <green>some test</> text to print <red>out while testing</> different styles.'
--text = 'This is some test text to print out while testing different styles.'

local textbox = MOAITextBox.new ()

textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

local style1 = MOAITextStyle.new ()
style1:setColor ( 1, 1, 1, 1 )
style1:setFont ( font )
style1:setSize ( 24 )
textbox:setStyle ( style1 )

local style2 = MOAITextStyle.new ()
style2:setColor ( 0, 1, 0, 1 )
style2:setFont ( font )
style2:setSize ( 24 )
textbox:setStyle ( 'green', style2 )

local style2 = MOAITextStyle.new ()
style2:setColor ( 1, 0, 0, 1 )
style2:setFont ( font )
style2:setSize ( 24 )
textbox:setStyle ( 'red', style2 )

textbox:setString ( text )
