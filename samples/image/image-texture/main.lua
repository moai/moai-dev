----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAIImageTexture.new ()
texture:load("IMG_0168.JPG")
texture1a = texture:resizeCanvas(0,0,2048,2048)
texture2 = texture1a:resize(256,256, MOAIImage.FILTER_NEAREST)

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture2 )
gfxQuad:setRect ( -60, -60, 60, 60 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

