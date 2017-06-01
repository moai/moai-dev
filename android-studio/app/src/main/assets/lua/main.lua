----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

prop = MOAIProp.new ()
prop:setDeck ( 'moai.png' )
prop:setLayer ( layer )
prop:moveRot ( 0, 0, 360, 5 )

prop = MOAIProp.new ()
prop:setDeck ( 'test.png' )
prop:setColor ( 1, 1, 1, 0 )
prop:setScl ( 2, 2, 1 )
prop:setLayer ( layer )
prop:moveRot ( 0, 0, -360, 5 )