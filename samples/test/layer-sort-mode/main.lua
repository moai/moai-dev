----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------
 
MOAISim.openWindow ( "test", 320, 480 )
 
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )
 
camera = MOAICamera.new()
camera:setLoc(0, 0, camera:getFocalLength(320))
 
layer = MOAILayer.new ()
layer:setCamera(camera)
layer:setViewport ( viewport )
layer:setSortMode(MOAILayer.SORT_Z_ASCENDING)
MOAISim.pushRenderPass ( layer )
 
gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )
 
prop1 = MOAIProp.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc(0, 0, 0)
layer:insertProp ( prop1 )
 
prop2 = MOAIProp.new()
prop2:setDeck ( gfxQuad )
prop2:setLoc(50, 50, -100)
layer:insertProp ( prop2 )
 