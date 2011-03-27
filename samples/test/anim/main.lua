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

prop1 = MOAIProp2D.new ()
prop1:setDeck ( texture )
layer:insertProp ( prop1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( texture )
layer:insertProp ( prop2 )

curve1 = MOAIAnimCurve.new ()
curve1:reserveKeys ( 2 )
curve1:setKey ( 1, 0, 0 )
curve1:setKey ( 2, 1.5, 64 )

curve2 = MOAIAnimCurve.new ()
curve2:reserveKeys ( 3 )
curve2:setKey ( 1, 0, 0 )
curve2:setKey ( 2, 0.75, 64 )
curve2:setKey ( 3, 1.5, 0 )

anim = MOAIAnim.new ()
anim:reserveLinks ( 3 )
anim:setLink ( 1, curve1, prop1, MOAIProp2D.ATTR_X_LOC )
anim:setLink ( 2, curve2, prop1, MOAIProp2D.ATTR_Y_LOC )
anim:setLink ( 3, curve1, prop2, MOAIProp2D.ATTR_X_LOC, true )
anim:setMode ( MOAITimer.LOOP )
anim:start ()

MOAISim.openWindow ( "cathead", 1280, 480 )
