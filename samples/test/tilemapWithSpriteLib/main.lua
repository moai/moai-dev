print ( "hello, moai!" )

scene = MOAILayer2D.new ()

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 0 )
scene:setViewport ( viewport )

partition = MOAIPartition.new ()
scene:setPartition ( partition )

camera = MOAITransform2D.new ()
scene:setCamera ( camera )

spriteLib = MOAIGfxQuadListDeck2D.new ()
spriteLib:setTexture ( "cathead.png" )

spriteLib:reserveUVRects ( 1 )
spriteLib:setUVRect ( 1, 0, 1, 1, 0 )

spriteLib:reserveQuads ( 2 )
spriteLib:setRect ( 1, -32, 32, 32, -32 )
spriteLib:setRect ( 2, -32, 64, 16, -16 )

spriteLib:reservePrims ( 2 )
spriteLib:setPrim ( 1, 1, 1 )
spriteLib:setPrim ( 2, 1, 2 )

spriteLib:reserveSprites ( 1 )
spriteLib:setSprite ( 1, 1, 2 )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setTile ( 0, 0, 0x00000000 )
grid:setTile ( 1, 0, 0x20000000 )
grid:setTile ( 0, 1, 0x60000000 )
grid:setTile ( 1, 1, 0x40000000 )

tilemap = MOAIProp2D.new ()
tilemap:setDeck ( spriteLib )
tilemap:setGrid ( grid )
partition:insertProp ( tilemap )

tilemap:moveRot ( 360, 1.5 )
tilemap:moveLoc ( -320, 0, 3 )

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )
