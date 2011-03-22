print ( "hello, moai!" )

--sprite1 = MOAISprite2D.new ()
--sprite2 = MOAISprite2D.new ()

--sprite1:setParent ( sprite2 )
--sprite2:setParent ( sprite1 )


layer = MOAILayer2D.new ()
sprite = MOAISprite2D.new ()
transform = MOAITransform2D.new ()

layer:insertPrim ( sprite )
layer:insertPrim ( sprite )
sprite:setParent ( layer )
sprite:setParent ( transform )

MOAISim.openWindow ( "cycles", 64, 64 )


