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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -32, -32, 32, 32 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc ( -64, 128 )
prop1:moveLoc ( 128, 0, 1.5 )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:moveRot ( 360, 1.5 )
layer:insertProp ( prop2 )

driver = MOAIEaseDriver.new ()
driver:reserveLinks ( 2 )
driver:setLink ( 1, prop2, MOAITransform.ATTR_X_LOC, prop1, MOAITransform.ATTR_X_LOC )
driver:setLink ( 2, prop2, MOAITransform.ATTR_Y_LOC, prop1, MOAITransform.ATTR_Y_LOC )
driver:setSpan ( 1.5 )
driver:start ()
