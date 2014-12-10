----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

gFrameBuffer = MOAIGfxDevice.getFrameBuffer ()
gFrameBuffer:setClearColor ( 1, 1, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

charcodes = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()&/-'
text = 'The quick brown fox jumps over the lazy dog.'

textbox = MOAITextBox.new ()
font = MOAIFont.new ()
font:setFilter ( MOAITexture.GL_LINEAR_MIPMAP_LINEAR, MOAITexture.GL_LINEAR_MIPMAP_LINEAR )
font:loadFromTTF ( 'times.ttf', charcodes, 12, 163 )

textbox:setString ( text )
textbox:setFont ( font )
textbox:setTextSize ( 12, 163 )
textbox:setRect ( -150, -230, 150, 230 )
textbox:setYFlip ( true )
textbox:setColor ( 0, 0, 0, 1 )
textbox:seekScl ( 0.25, 0.25, 1, 5 )
layer:insertProp ( textbox )

