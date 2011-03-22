print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

----------------------------------------------------------------
texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveSprites ( 256 )
system:setGfxSource ( texture )

system:pushSprite ( 0, 0 )

layer:insertPrim ( system )
