print ( "hello, moai!" )

layer = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 1280, 480 )
viewport:setScale ( 1280, 480 )

layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite1 = MOAIProp2D.new ()
sprite1:setDeck ( texture )
layer:insertPrim ( sprite1 )

sprite2 = MOAIProp2D.new ()
sprite2:setDeck ( texture )
layer:insertPrim ( sprite2 )

anim = MOAIAnim.new ()
anim:reserveLinks ( 2 )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, 64 )

anim:setLink ( 1, curve, sprite1, MOAIProp2D.ATTR_X_LOC )

curve = MOAIAnimCurve.new ()
curve:reserveKeys ( 2 )
curve:setKey ( 1, 0, 0 )
curve:setKey ( 2, 1.5, 64 )

anim:setLink ( 2, curve, sprite2, MOAIProp2D.ATTR_X_LOC, true )

animDriver = MOAIAnimDriver.new ()
animDriver:setAnim ( anim )
animDriver:setMode ( MOAITimer.LOOP )
animDriver:start ()

MOAISim.openWindow ( "cathead", 1280, 480 )
