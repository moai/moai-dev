print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

camera = MOAICamera2D.new ()

scene:setViewport ( viewport )
scene:setCamera ( camera )
MOAISim.pushRenderPass ( scene )

sprite = MOAISprite2D.new ()

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

sprite:setGfxSource ( texture )

partition = MOAIPartition.new ()
partition:insertPrim ( sprite )
scene:setPartition ( partition )

camera:moveLoc ( 128, 0, 3 )
camera:moveRot ( 180, 3 )
camera:moveScl ( 1, 1, 3 )

MOAISim.openWindow ( "cathead", 320, 480 )
