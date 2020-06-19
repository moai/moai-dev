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

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( '../resources/test.png' )
tileDeck:setSize ( 2, 2 )
tileDeck:setRect ( -64, -64, 64, 64 )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( tileDeck )
prop:setPartition ( layer )

curve = MOAIAnimCurveIndex.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 1)
curve:setKey ( 2, 0.25, 2 )
curve:setKey ( 3, 0.50, 3 )
curve:setKey ( 4, 0.75, 4 )
curve:setKey ( 5, 1.00, 1 )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, prop, MOAIGraphicsProp.ATTR_INDEX )
anim:setMode ( MOAITimer.LOOP )
anim:start ()