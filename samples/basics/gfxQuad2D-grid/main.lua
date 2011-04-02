print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000001 )
grid:setRow ( 2,	0x40000001, 0x60000001 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
prop:setGrid ( grid )
layer:insertProp ( prop )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "test", 320, 480 )
