----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_PARTITION_CELLS, 2, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLinesMgr.setStyle ( MOAIProp.DEBUG_DRAW_WORLD_BOUNDS, 4, 1, 0, 1 )

MOAIDebugLinesMgr.setStyle ( MOAICamera.DEBUG_DRAW_RETICLE, 1, 0.5, 0.5, 1, 0.5 )
MOAIDebugLinesMgr.setStyle ( MOAICamera.DEBUG_DRAW_FRAME, 4, 1, 0, 0, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAICamera.new ()

--camera:moveRot ( 0, 0, 45, 5 )

partition = MOAIPartition.new ()
partition:reserveLevels ( 3 )
partition:setLevel ( 1, 256, 4, 4 )
partition:setLevel ( 2, 128, 8, 8 )
partition:setLevel ( 3, 96, 96, 12 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
layer:setLayerPartition ( partition )
layer:pushRenderPass ()

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIProp.new ()
prop:setDeck ( spriteDeck )
prop:setPartition ( layer )

prop:setLoc ( -64, 64, 0 )
prop:moveLoc ( 128, -128, 0, 3 )
prop:moveScl ( -0.5, -0.5, 0, 3 )

debugCamera = MOAICamera.new ()
debugCamera:setType ( MOAICamera.CAMERA_TYPE_3D )
debugCamera:setLoc ( 320, 0, debugCamera:getFocalLength ( 320 ))
debugCamera:setRot ( 0, 45, 0 )

layer:setDebugCamera ( debugCamera )
