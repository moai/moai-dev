----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

tileLib = MOAITileDeck2D:new ()
tileLib:setTexture ( "numbers.png" )
tileLib:setSize ( 8, 8 )
tileLib:setRect ( -16, -16, 16, 16 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileLib )
layer:insertProp ( prop )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 9 )
curve:setKey ( 1, 0.00, 1, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.25, 2, MOAIEaseType.FLAT )
curve:setKey ( 3, 0.50, 3, MOAIEaseType.FLAT )
curve:setKey ( 4, 0.75, 4, MOAIEaseType.FLAT )
curve:setKey ( 5, 1.00, 5, MOAIEaseType.FLAT )
curve:setKey ( 6, 1.25, 6, MOAIEaseType.FLAT )
curve:setKey ( 7, 1.50, 7, MOAIEaseType.FLAT )
curve:setKey ( 8, 1.75, 8, MOAIEaseType.FLAT )
curve:setKey ( 9, 2.00, 9, MOAIEaseType.FLAT )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, prop, MOAIProp2D.ATTR_INDEX )
anim:setMode ( MOAITimer.PING_PONG )
anim:setSpan ( 0.25, 1.5 )
anim:setTime ( 0.25 )
anim:start ()

