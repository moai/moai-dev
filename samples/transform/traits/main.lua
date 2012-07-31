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

color = MOAIColor.new ()
color:setColor ( 1, 0, 0, 1 )
color:seekColor ( 0, 1, 1, 1, 1.5 )

root = MOAIProp2D.new ()
root:setDeck ( gfxQuad )
root:setLoc ( 0, -64 )
root:setFrame ( -128, -128, 128, 128 )
root:moveRot ( 360, 1.5 )
root:moveLoc ( 0, 128, 1.5 )
root:moveScl ( 0, -2, 1.5 )
root:setAttrLink ( MOAIProp2D.INHERIT_COLOR, color, MOAIColor.COLOR_TRAIT )
layer:insertProp ( root )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setFrame ( -64, -64, 64, 64 )
prop:setLoc ( 0.5, 0.5 )
prop:setAttrLink ( MOAIProp2D.ATTR_PARTITION, root, MOAIProp2D.ATTR_PARTITION )
prop:setAttrLink ( MOAIProp2D.INHERIT_LOC, root, MOAIProp2D.TRANSFORM_TRAIT )
prop:setAttrLink ( MOAIProp2D.INHERIT_COLOR, root, MOAIProp2D.COLOR_TRAIT )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setFrame ( -0.25, -0.25, 0.25, 0.25 )
prop:setLoc ( -0.5, -0.5 )
prop:setAttrLink ( MOAIProp2D.ATTR_PARTITION, root, MOAIProp2D.ATTR_PARTITION )
prop:setAttrLink ( MOAIProp2D.INHERIT_TRANSFORM, root, MOAIProp2D.TRANSFORM_TRAIT )
