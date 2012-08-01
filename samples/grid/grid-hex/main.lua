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
grid:initHexGrid ( 4, 4, 64 )
grid:setRepeat ( true )

grid:setRow ( 1, 	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 2,	0x03, 0x04, 0x03, 0x04 )
grid:setRow ( 3,	0x01, 0x02, 0x01, 0x02 )
grid:setRow ( 4,	0x03, 0x04, 0x03, 0x04 )

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "hex-tiles.png" )
tileDeck:setSize ( 4, 4, 0.25, 0.216796875 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -256, -256 )
prop:forceUpdate ()
layer:insertProp ( prop )

----------------------------------------------------------------
cursor = MOAIProp2D.new ()
cursor:setDeck ( tileDeck )
cursor:setScl ( grid:getTileSize ())
cursor:addScl ( -32 )
layer:insertProp ( cursor )

local xCoord = 0
local yCoord = 0

function onPointerEvent ( x, y )

	grid:clearTileFlags ( xCoord, yCoord, MOAIGrid.TILE_HIDE )
	x, y = layer:wndToWorld ( x, y )
	x, y = prop:worldToModel ( x, y )
	xCoord, yCoord = grid:locToCoord ( x, y )
	
	x, y = grid:getTileLoc ( xCoord, yCoord, MOAIGrid.TILE_CENTER )
	x, y = prop:modelToWorld ( x, y )
	cursor:setLoc ( x, y )
	
	xCoord, yCoord = grid:wrapCoord ( xCoord, yCoord )
	cursor:setIndex ( grid:getTile ( xCoord, yCoord ))
	
	grid:setTileFlags ( xCoord, yCoord, MOAIGrid.TILE_HIDE )
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
onPointerEvent ( 0, 0 )