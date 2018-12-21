----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

--set up the world layer
worldLayer = MOAIPartitionViewLayer.new ()
worldLayer:setViewport ( viewport )
worldLayer:pushRenderPass ()

-- set up the world camera
camera = MOAICamera.new ()
camera:moveLoc ( 0, -192, 0, 3 )
worldLayer:setCamera ( camera )

-- add a prop to the world
gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( worldLayer )

prop:moveRot ( 0, 0, 360, 3 )
prop:moveLoc ( 64, 0, 0, 3 )

--set up the hud layer
hudLayer = MOAIPartitionViewLayer.new ()
hudLayer:setViewport ( viewport )
hudLayer:pushRenderPass ()

-- create the bridge
pinTransform = MOAIPinTransform.new ()
pinTransform:init ( worldLayer, hudLayer )
pinTransform:setParent ( prop )

-- create the pin graphic and attach it to the pin transform
gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( "../resources/moai.png" )
gfxQuad:setRect ( -32, -32, 32, 32 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

pin = MOAIProp.new ()
pin:setDeck ( gfxQuad )
pin:setAttrLink ( MOAIProp.INHERIT_LOC, pinTransform, MOAIProp.TRANSFORM_TRAIT )
pin:setPartition ( hudLayer )
