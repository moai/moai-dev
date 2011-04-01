print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 480 )
viewport:setScale ( 480, 480 )
--viewport:setRotation ( 90 )

camera = MOAITransform.new ()
camera:moveLoc ( 240, 0, 4 )

layer1 = MOAILayer2D.new ()
layer1:setViewport ( viewport )
layer1:setCamera ( camera )
layer1:setParallax ( 0.5, 1 )
MOAISim.pushRenderPass ( layer1 )

layer2 = MOAILayer2D.new ()
layer2:setViewport ( viewport )
layer2:setCamera ( camera )
MOAISim.pushRenderPass ( layer2 )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
sprite:setScl ( 0.75, 0.75 )
layer1:insertProp ( sprite )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
layer2:insertProp ( sprite )

MOAISim.setClearColor ( 1, 0, 0, 1 )
MOAISim.openWindow ( "cathead", 480, 480 )

