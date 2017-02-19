----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

VIEW_WIDTH, VIEW_HEIGHT = MOAIGfxMgr.getViewSize ()

viewport = MOAIViewport.new ()
viewport:setSize ( VIEW_WIDTH, VIEW_HEIGHT )
viewport:setScale ( 320, 320 * ( VIEW_HEIGHT / VIEW_WIDTH ))

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

prop = MOAIProp.new ()
prop:setDeck ( 'moai.png' )
prop:setPartition ( layer )
prop:moveRot ( 0, 0, 360, 5 )

prop = MOAIProp.new ()
prop:setDeck ( 'test.png' )
prop:setColor ( 1, 1, 1, 0 )
prop:setScl ( 2, 2, 1 )
prop:setPartition ( layer )
prop:moveRot ( 0, 0, -360, 5 )
