print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAITransform.new ()

scene:setViewport ( viewport )
scene:setCamera ( camera )
MOAISim.pushRenderPass ( scene )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
scene:insertProp ( sprite )

camera:moveLoc ( 128, 0, 3 )
camera:moveRot ( 180, 3 )
camera:moveScl ( 1, 1, 3 )

MOAISim.openWindow ( "cathead", 320, 480 )
