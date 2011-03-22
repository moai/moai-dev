scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )
scene:setViewport ( viewport )

tileLib = MOAITileLibrary:new ()
tileLib:setTexture ( "numbers.png" )
tileLib:setSize ( 8, 8 )
tileLib:setRect ( -16, -16, 16, 16 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( tileLib )
scene:insertPrim ( sprite )

curve = MOAIAnimCurve.new ()

curve:reserveKeys ( 5 )
curve:setKey ( 1, 0.00, 0, MOAITransition.FLAT )
curve:setKey ( 2, 0.25, 1, MOAITransition.FLAT )
curve:setKey ( 3, 0.50, 2, MOAITransition.FLAT )
curve:setKey ( 4, 0.75, 3, MOAITransition.FLAT )
curve:setKey ( 5, 1.00, 0, MOAITransition.FLAT )

anim = MOAIAnim:new ()
anim:reserveCurves ( 1 )
anim:setCurve ( 1, curve )

player = MOAIAnimPlayer.new ()
player:reserveLinks ( 1 )
player:setLink ( 1, 1, sprite, MOAISprite2D.ATTR_CONTENT_ADDR )
player:setAnim ( anim )
player:setMode ( MOAITimer.LOOP )
player:start ()

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )