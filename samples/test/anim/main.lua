print ( "hello, moai!" )

layer = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 1280, 480 )
viewport:setScale ( 1280, 480 )

layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite1 = MOAISprite2D.new ()
sprite1:setGfxSource ( texture )
layer:insertPrim ( sprite1 )

sprite2 = MOAISprite2D.new ()
sprite2:setGfxSource ( texture )
layer:insertPrim ( sprite2 )

anim = MOAIAnim.new ()
anim:reserveCurves ( 2 )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, 64 )
anim:setCurve ( 1, curve )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, -64 )
anim:setCurve ( 2, curve )

animPlayer = MOAIAnimPlayer.new ()

animPlayer:setAnim ( anim )
animPlayer:reserveLinks ( 2 )

animPlayer:setLink ( 1, 1, sprite1, MOAISprite2D.ATTR_X_LOC )
animPlayer:setLink ( 2, 1, sprite2, MOAISprite2D.ATTR_X_LOC, true )

animPlayer:setMode ( MOAITimer.LOOP )
animPlayer:start ()

MOAISim.openWindow ( "cathead", 1280, 480 )
