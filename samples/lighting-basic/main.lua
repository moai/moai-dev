----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

lightFormat = MOAILightFormat.new ()
lightFormat:reserveTextures ( 1 )

light = MOAILight.new ()
light:setFormat ( lightFormat )
light:setTexture ( 1, '../resources/moai.png' )

prop = MOAIProp.new ()
prop:setDeck ( '../resources/moai.png' )
--prop:setLightSetArray ( lightSetArray )
prop:setPartition ( layer )
prop:moveRot ( 0, 0, 360, 5 )
