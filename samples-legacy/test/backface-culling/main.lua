----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )
MOAIGfxDevice.setClearDepth ( true )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setSortMode ( MOAILayer2D.SORT_NONE ) -- don't need layer sort
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setCullMode ( MOAIProp2D.CULL_BACK )
layer:insertProp ( prop )

prop:moveRot ( 0, 720, 0, 3 )
prop:moveLoc ( 0, 0, 128, 3 )

camera = MOAICamera.new ()
camera:setLoc ( 0, 0, camera:getFocalLength ( 320 ))
layer:setCamera ( camera )