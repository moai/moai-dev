----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc ( 0, 64 )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
prop2:setLoc ( 0, -64 )
layer:insertProp ( prop2 )

prop3 = MOAIProp2D.new ()
prop3:setDeck ( gfxQuad )
prop3:setLoc ( 0, -64 )
layer:insertProp ( prop3 )

prop2:setParent ( prop1 )

prop3:setAttrLink ( MOAITransform.ATTR_X_LOC, prop2, MOAITransform.ATTR_WORLD_X_LOC )
prop3:setAttrLink ( MOAITransform.ATTR_Z_ROT, prop2, MOAITransform.ATTR_Z_ROT )
prop3:setAttrLink ( MOAITransform.ATTR_X_SCL, prop2, MOAITransform.ATTR_WORLD_Y_SCL )

prop1:moveLoc ( 64, 64, 1.5 )
prop2:moveScl ( 0, 0.5, 1.5 )
prop2:moveRot ( -360, 1.5 )
