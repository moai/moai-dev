print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

texture = MOAITexture.new ()
texture:load ( "cathead.png" )
texture:setRect ( -64, -64, 64, 64 )

shader1 = MOAISimpleShader.new ()
shader1:setColor ( 0, 0, 0, 0 )
shader1:seekColor ( 1, 1, 1, 1, 1.5 )

shader2 = MOAISimpleShader.new ()
shader2:setParent ( shader1 );
shader2:seekColor ( 1, 0, 0, 1, 1.5 )

sprite = MOAISprite2D.new ()
sprite:setGfxSource ( texture )
sprite:setShader ( shader2 )
sprite:moveRot ( 360, 1.5 )

layer:insertPrim ( sprite )
MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "cathead", 320, 480 )
