----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )
MOAISim.openWindow ( "cathead", 320, 480 )

MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_CELLS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

scene = MOAILayer2D.new ()
scene:showDebugLines ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

partition = MOAIPartition.new ()
partition:reserveLayers ( 3 )
partition:setLayer ( 1, 256, 4, 4 )
partition:setLayer ( 2, 128, 8, 8 )
partition:setLayer ( 3, 96, 96, 12 )
scene:setPartition ( partition )

camera = MOAITransform.new ()
scene:setCamera ( camera )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
partition:insertProp ( sprite )

sprite:setLoc ( -64, 64 )
sprite:moveLoc ( 128, -128, 3 )
sprite:moveScl ( -0.5, -0.5, 3 )
--sprite:moveRot ( 360, 3 )

MOAISim.pushRenderPass ( scene )
