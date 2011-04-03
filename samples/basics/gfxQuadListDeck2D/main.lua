----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )

gfxQuadListDeck = MOAIGfxQuadListDeck2D.new ()
gfxQuadListDeck:setTexture ( "test.png" )

gfxQuadListDeck:reserveQuads ( 4 )
gfxQuadListDeck:setRect ( 1, -64, 0, 0, 64 )
gfxQuadListDeck:setRect ( 2, 0, 0, 64, 64 )
gfxQuadListDeck:setRect ( 3, -64, -64, 0, 0 )
gfxQuadListDeck:setRect ( 4, 0, -64, 64, 0 )

gfxQuadListDeck:reserveUVRects ( 4 )
gfxQuadListDeck:setUVRect ( 1, 0.0, 0.5, 0.5, 0.0 )
gfxQuadListDeck:setUVRect ( 2, 0.5, 0.5, 1.0, 0.0 )
gfxQuadListDeck:setUVRect ( 3, 0.0, 1.0, 0.5, 0.5 )
gfxQuadListDeck:setUVRect ( 4, 0.5, 1.0, 1.0, 0.5 )

gfxQuadListDeck:reservePrims ( 4 )
gfxQuadListDeck:setPrim ( 1, 1, 1 )
gfxQuadListDeck:setPrim ( 2, 2, 2 )
gfxQuadListDeck:setPrim ( 3, 3, 3 )
gfxQuadListDeck:setPrim ( 4, 4, 4 )

gfxQuadListDeck:reserveLists ( 4 )
gfxQuadListDeck:setList ( 1, 1, 4 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadListDeck )
layer:insertProp ( prop )

MOAISim.pushRenderPass ( layer )
MOAISim.openWindow ( "test", 320, 480 )
