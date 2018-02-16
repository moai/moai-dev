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

color = MOAIColor.new ()
color:setColor ( 1, 0, 0, 1 )
color:seekColor ( 0, 1, 1, 1, 1.5 )

root = MOAIProp.new ()
root:setDeck ( gfxQuad )
root:setScl ( 256, 256 )
root:moveRot ( 360, 1.5 )
root:moveScl ( 0, -128, 1.5 )
root:setAttrLink ( MOAIProp.INHERIT_COLOR, color, MOAIColor.COLOR_TRAIT )
root:setPartition ( layer )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setScl ( 128, 128 )
prop:setLoc ( 0.5, 0.5 )
prop:setAttrLink ( MOAIProp.ATTR_PARTITION, root, MOAIProp.ATTR_PARTITION )
prop:setAttrLink ( MOAIProp.INHERIT_LOC, root, MOAIProp.TRANSFORM_TRAIT )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setScl ( 0.5, 0.5 )
prop:setLoc ( -0.5, -0.5 )
prop:setAttrLink ( MOAIProp.ATTR_PARTITION, root, MOAIProp.ATTR_PARTITION )
prop:setAttrLink ( MOAIProp.INHERIT_COLOR, root, MOAIProp.COLOR_TRAIT )
prop:setAttrLink ( MOAIProp.INHERIT_TRANSFORM, root, MOAIProp.TRANSFORM_TRAIT )
