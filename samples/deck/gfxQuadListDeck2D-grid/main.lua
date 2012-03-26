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
gridW = 64
gridH = 64
gridNumW = 4
gridNumH = 2
grid:setSize ( gridNumW, gridNumH, gridW, gridH )

grid:setRow ( 1, 	0x00000001, 0x00000001, 0x00000001, 0x00000001 )
grid:setRow ( 2,	0x00000002, 0x00000001, 0x00000001, 0x00000001 )

gfxQuadListDeck = MOAIGfxQuadListDeck2D.new ()
gfxQuadListDeck:setTexture ( "test.png" )

gfxQuadListDeck:reserveQuads ( 5 )
gfxQuadListDeck:setRect ( 1, -0.5, 0, 0, 0.5 )
gfxQuadListDeck:setRect ( 2, 0, 0, 0.5, 0.5 )
gfxQuadListDeck:setRect ( 3, -0.5, -0.5, 0, 0 )
gfxQuadListDeck:setRect ( 4, 0, -0.5, 0.5, 0 )
gfxQuadListDeck:setRect ( 5, -1.0, -1.0, 1.0, 1.0 )
--gfxQuadListDeck:setRect ( 5, -0.5, -0.5, 0.5, 0.5 )

gfxQuadListDeck:reserveUVQuads ( 4 )
gfxQuadListDeck:setUVRect ( 1, 0.0, 0.5, 0.5, 0.0 ) --A
gfxQuadListDeck:setUVRect ( 2, 0.5, 0.5, 1.0, 0.0 ) --B
gfxQuadListDeck:setUVRect ( 3, 0.0, 1.0, 0.5, 0.5 ) --C
gfxQuadListDeck:setUVRect ( 4, 0.5, 1.0, 1.0, 0.5 ) --D

gfxQuadListDeck:reservePairs ( 5 )
gfxQuadListDeck:setPair ( 1, 1, 1 )
gfxQuadListDeck:setPair ( 2, 2, 2 )
gfxQuadListDeck:setPair ( 3, 3, 3 )
gfxQuadListDeck:setPair ( 4, 4, 4 )
gfxQuadListDeck:setPair ( 5, 3, 5 )

gfxQuadListDeck:reserveLists ( 4 )
gfxQuadListDeck:setList ( 1, 1, 4 )
gfxQuadListDeck:setList ( 2, 5, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadListDeck )
prop:setGrid ( grid )
layer:insertProp ( prop )

function onPointerEvent ( x, y )
	local wx,wy = layer:wndToWorld( x,y )
	prop:setLoc ( wx - ( gridW * gridNumW )/2, wy - ( gridH * gridNumH )/2 )
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
