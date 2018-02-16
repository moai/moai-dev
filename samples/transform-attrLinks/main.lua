----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop1 = MOAIProp.new ()
prop1:setDeck ( gfxQuad )
prop1:setLoc ( 0, 64 )
prop1::setPartition ( layer )

prop2 = MOAIProp.new ()
prop2:setDeck ( gfxQuad )
prop2:setLoc ( 0, -64 )
prop2::setPartition ( layer )

prop3 = MOAIProp.new ()
prop3:setDeck ( gfxQuad )
prop3:setLoc ( 0, -64 )
prop3::setPartition ( layer )

prop2:setParent ( prop1 )

prop3:setAttrLink ( MOAITransform.ATTR_X_LOC, prop2, MOAITransform.ATTR_WORLD_X_LOC )
prop3:setAttrLink ( MOAITransform.ATTR_Z_ROT, prop2, MOAITransform.ATTR_Z_ROT )
prop3:setAttrLink ( MOAITransform.ATTR_X_SCL, prop2, MOAITransform.ATTR_WORLD_Y_SCL )

prop1:moveLoc ( 64, 64, 1.5 )
prop2:moveScl ( 0, 0.5, 1.5 )
prop2:moveRot ( -360, 1.5 )
