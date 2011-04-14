----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 960, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

brushlib = MOAIGfxQuadDeck2D.new ()
brushlib:setTexture ( "../test.png" )

brushlib:reserve ( 9 )
brushlib:setRect ( 1, -32, -32, 32, 32 )
brushlib:setUVRect ( 1, 0, .5, .5, 0 )
brushlib:setRect ( 4, -64, -64, 64, 64 )
brushlib:setUVRect ( 4, 0, 1, 1, 0 )

prop = MOAIProp2D.new ()
prop:setDeck ( brushlib )
layer:insertProp ( prop )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 9 )
curve:setKey ( 1, 0, 1, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.5, 2, MOAIEaseType.FLAT )
curve:setKey ( 3, 1, 3, MOAIEaseType.FLAT )
curve:setKey ( 4, 1.5, 4, MOAIEaseType.FLAT )
curve:setKey ( 5, 2, 5, MOAIEaseType.FLAT )
curve:setKey ( 6, 2.5, 6, MOAIEaseType.FLAT )
curve:setKey ( 7, 3, 7, MOAIEaseType.FLAT )
curve:setKey ( 8, 3.5, 8, MOAIEaseType.FLAT )
curve:setKey ( 9, 4, 9, MOAIEaseType.FLAT )

anim = MOAIAnim.new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, prop, MOAIProp2D.ATTR_INDEX )
anim:setMode ( MOAITimer.LOOP )
anim:start ()

MOAISim.openWindow ( "stress test", 960, 640 )
