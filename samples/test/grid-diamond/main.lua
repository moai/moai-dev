----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

grid = MOAIGrid.new ()
grid:initDiamondGrid ( 4, 4, 128, 64 )
grid:setRepeat ( true )

grid:setRow ( 1, 	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 2,	0x03, 0x04, 0x03, 0x04 )
grid:setRow ( 3,	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 4,	0x03, 0x04, 0x03, 0x04 )

gfxQuadDeck = MOAIGfxQuadDeck2D.new ()
gfxQuadDeck:setTexture ( "cathead.png" )
gfxQuadDeck:reserve ( 1 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "diamond-tiles.png" )
tileDeck:setSize ( 4, 4 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -256, -256 )
layer:insertProp ( prop )

prop:moveRot ( 360, 10 )
prop:moveLoc ( 512, 0, 10 )

----------------------------------------------------------------
local xCoord = 0
local yCoord = 0

function onPointerEvent ( x, y )

	grid:clearTileFlags ( xCoord, yCoord, MOAIGrid.TILE_HIDE )
	x, y = layer:wndToWorld ( x, y )
	x, y = prop:worldToModel ( x, y )
	xCoord, yCoord = grid:locToCoord ( x, y )
	xCoord, yCoord = grid:wrapCoord ( xCoord, yCoord )
	grid:setTileFlags ( xCoord, yCoord, MOAIGrid.TILE_HIDE )
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )