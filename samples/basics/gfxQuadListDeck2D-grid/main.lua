----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:setSize ( 2, 2, 64, 64 )

grid:setRow ( 1, 	0x00000001, 0x20000001 )
grid:setRow ( 2,	0x40000001, 0x60000001 )

gfxQuadListDeck = MOAIGfxQuadListDeck2D.new ()
gfxQuadListDeck:setTexture ( "test.png" )

gfxQuadListDeck:reserveQuads ( 4 )
gfxQuadListDeck:setRect ( 1, -0.5, 0, 0, 0.5 )
gfxQuadListDeck:setRect ( 2, 0, 0, 0.5, 0.5 )
gfxQuadListDeck:setRect ( 3, -0.5, -0.5, 0, 0 )
gfxQuadListDeck:setRect ( 4, 0, -0.5, 0.5, 0 )

gfxQuadListDeck:reserveUVQuads ( 4 )
gfxQuadListDeck:setUVRect ( 1, 0.0, 0.5, 0.5, 0.0 )
gfxQuadListDeck:setUVRect ( 2, 0.5, 0.5, 1.0, 0.0 )
gfxQuadListDeck:setUVRect ( 3, 0.0, 1.0, 0.5, 0.5 )
gfxQuadListDeck:setUVRect ( 4, 0.5, 1.0, 1.0, 0.5 )

gfxQuadListDeck:reservePairs ( 4 )
gfxQuadListDeck:setPair ( 1, 1, 1 )
gfxQuadListDeck:setPair ( 2, 2, 2 )
gfxQuadListDeck:setPair ( 3, 3, 3 )
gfxQuadListDeck:setPair ( 4, 4, 4 )

gfxQuadListDeck:reserveLists ( 4 )
gfxQuadListDeck:setList ( 1, 1, 4 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadListDeck )
prop:setGrid ( grid )
layer:insertProp ( prop )
