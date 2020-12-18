----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( 'test', 480, 320 )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 320 )
viewport:setScale ( 480, 320 )

camera = MOAICamera.new ()
camera:moveLoc ( 240, 0, 0, 4 )

layer1 = MOAIPartitionViewLayer.new ()
layer1:setViewport ( viewport )
layer1:setCamera ( camera )
layer1:setParallax ( 0.5, 1 )
layer1:pushRenderPass ()

layer2 = MOAIPartitionViewLayer.new ()
layer2:setViewport ( viewport )
layer2:setCamera ( camera )
layer2:pushRenderPass ()

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:gfx ():setTexture ( '../resources/moai.png' )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setScl ( 0.75, 0.75 )
prop:setPartition ( layer1 )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer2 )
