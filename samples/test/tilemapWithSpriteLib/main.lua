print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

camera = MOAITransform.new ()
scene:setCamera ( camera )

spriteLib = MOAIGfxQuadListDeck2D.new ()
spriteLib:setTexture ( "cathead.png" )

spriteLib:reserveUVRects ( 1 )
spriteLib:setUVRect ( 1, 0, 1, 1, 0 )

spriteLib:reserveQuads ( 2 )
spriteLib:setRect ( 1, -32, -32, 32, 32 )
spriteLib:setRect ( 2, -32, -16, 16, -64 )

spriteLib:reservePrims ( 2 )
spriteLib:setPrim ( 1, 1, 1 )
spriteLib:setPrim ( 2, 1, 2 )

spriteLib:reserveSprites ( 1 )
spriteLib:setSprite ( 1, 1, 2 )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setTile ( 1, 1, 0x00000001 )
grid:setTile ( 2, 1, 0x20000001 )
grid:setTile ( 1, 2, 0x60000001 )
grid:setTile ( 2, 2, 0x40000001 )

tilemap = MOAIProp2D.new ()
tilemap:setDeck ( spriteLib )
tilemap:setGrid ( grid )
partition:insertProp ( tilemap )

tilemap:moveRot ( 360, 1.5 )
tilemap:moveLoc ( -320, 0, 3 )

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )
