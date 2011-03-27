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

brushlib = MOAIGfxQuadDeck2D.new ()
brushlib:setTexture ( "cathead.png" )

brushlib:reserveBrushes ( 1 )
brushlib:setRect ( 1, -32, 64, 16, -16 )
brushlib:setUVRect ( 1, 0, 0, 1, 1 )

grid = MOAIGrid.new ()
grid:setSize ( 8, 8, 32, 32 )

grid:setTile ( 0, 0, 0x00000000 )
grid:setTile ( 1, 0, 0x20000000 )
grid:setTile ( 0, 1, 0x60000000 )
grid:setTile ( 1, 1, 0x40000000 )

tilemap = MOAIProp2D.new ()
tilemap:setDeck ( brushlib )
tilemap:setGrid ( grid )
tilemap:setRepeat ( true )
partition:insertPrim ( tilemap )

tilemap:moveRot ( 360, 1.5 )
tilemap:moveLoc ( -320, 0, 3 )

MOAISim.pushRenderPass ( scene )
MOAISim.openWindow ( "cathead", 320, 480 )
