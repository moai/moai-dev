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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( 'cathead-rgb24-interlaced.png', MOAIImage.PREMULTIPLY_ALPHA )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( 0, 64 )
prop:moveRot ( 360, 1.5 )
layer:insertProp ( prop )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( 'cathead-rgb8-interlaced.png', MOAIImage.TRUECOLOR + MOAIImage.QUANTIZE )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setLoc ( 0, -64 )
prop:moveRot ( -360, 1.5 )
layer:insertProp ( prop )
