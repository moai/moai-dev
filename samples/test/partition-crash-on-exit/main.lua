----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 800, 800 )

MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_CELLS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

viewport = MOAIViewport.new ()
viewport:setSize ( 800, 800 )
viewport:setScale ( 800, 800 )

partition = MOAIPartition.new ()
partition:reserveLayers ( 3 )
partition:setLayer ( 1, 256, 4, 4 )
partition:setLayer ( 2, 128, 8, 8 )
partition:setLayer ( 3, 96, 12, 12 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setPartition ( partition )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

partition:setPlane ( MOAIPartition.PLANE_XZ )
