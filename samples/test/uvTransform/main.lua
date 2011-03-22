print ( "hello, moai!" )

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

scene = MOAILayer2D.new ()
MOAISim.pushRenderPass ( scene )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

camera = MOAICamera2D.new ()
scene:setCamera ( camera )

uvTransform = MOAITransform2D.new ()
uvTransform:moveLoc ( 1, 0, 3 )
uvTransform:moveRot ( 360, 3 )
uvTransform:moveScl ( 1, 1, 3 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( texture )
sprite:setUVTransform ( uvTransform )
partition:insertPrim ( sprite )

sprite:moveRot ( -360, 2 )
sprite:moveScl ( 1.5, 1.5, 3 )

MOAISim.openWindow ( "cathead", 320, 480 )
