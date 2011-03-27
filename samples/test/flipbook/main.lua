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
scene:insertPrim ( sprite )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 0, MOAIEaseType.FLAT )
curve:setKey ( 2, 0.25, 1, MOAIEaseType.FLAT )
curve:setKey ( 3, 0.50, 2, MOAIEaseType.FLAT )
curve:setKey ( 4, 0.75, 3, MOAIEaseType.FLAT )
curve:setKey ( 5, 1.00, 0, MOAIEaseType.FLAT )

anim = MOAIAnim:new ()
anim:reserveLinks ( 1 )
anim:setLink ( 1, curve, sprite, MOAIProp2D.ATTR_INDEX )

player = MOAIAnimDriver.new ()
player:setAnim ( anim )
player:setMode ( MOAITimer.LOOP )
player:start ()

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )