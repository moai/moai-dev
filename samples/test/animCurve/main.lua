print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

scene:setViewport ( viewport )
MOAISim.pushRenderPass ( scene )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( texture )
partition:insertPrim ( sprite )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, 360 )

sprite:setAttrLink ( MOAISprite2D.ATTR_Z_ROT, curve, MOAIAnimCurve.ATTR_VALUE )

timer = MOAITimer.new ()
timer:setSpan ( 0, curve:getLength ())
timer:start ()

curve:setAttrLink ( MOAIAnimCurve.ATTR_TIME, timer, MOAITimer.ATTR_TIME )

MOAISim.openWindow ( "cathead", 320, 480 )
