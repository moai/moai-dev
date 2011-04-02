print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

gfxQuadDeck = MOAIGfxQuadDeck2D.new ()
gfxQuadDeck:setTexture ( "cathead.png" )
gfxQuadDeck:reserve ( 1 )
gfxQuadDeck:setRect ( 1, -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadDeck )
layer:insertProp ( prop )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "test", 320, 480 )
