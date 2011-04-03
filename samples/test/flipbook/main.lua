----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
scene:setViewport ( viewport )

tileLib = MOAITileDeck2D:new ()
tileLib:setTexture ( "numbers.png" )
tileLib:setSize ( 8, 8 )
tileLib:setRect ( -16, -16, 16, 16 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( tileLib )
scene:insertProp ( sprite )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 1, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.25, 2, MOAIEaseType.FLAT )
curve:setKey ( 3, 0.50, 3, MOAIEaseType.FLAT )
curve:setKey ( 4, 0.75, 4, MOAIEaseType.FLAT )
curve:setKey ( 5, 1.00, 1, MOAIEaseType.FLAT )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, sprite, MOAIProp2D.ATTR_INDEX )
anim:setMode ( MOAITimer.LOOP )
anim:start ()

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )