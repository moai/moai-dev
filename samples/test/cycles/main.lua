print ( "hello, moai!" )

--sprite1 = MOAIProp2D.new ()
--sprite2 = MOAIProp2D.new ()

--sprite1:setParent ( sprite2 )
--sprite2:setParent ( sprite1 )


layer = MOAILayer2D.new ()
sprite = MOAIProp2D.new ()
transform = MOAITransform2D.new ()

layer:insertProp ( sprite )
layer:insertProp ( sprite )
sprite:setParent ( layer )
sprite:setParent ( transform )

MOAISim.openWindow ( "cycles", 64, 64 )


