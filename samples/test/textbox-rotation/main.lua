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

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'

font = MOAIFont.new ()
font:load ( 'arial-rounded.TTF' )
font:preloadGlyphs ( charcodes, 12 )

text = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris semper orci dolor, vel interdum velit. Sed pharetra nulla a sem venenatis at molestie nulla gravida. Pellentesque viverra iaculis tempus. In semper condimentum ligula, ut interdum sem tincidunt sed. Duis eget dui nunc.'

local textbox = MOAITextBox.new ()
textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
layer:insertProp ( textbox )

textbox:moveRot ( 5, 5, 0, 5 )
