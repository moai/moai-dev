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

local grid_rows = 8
local grid_columns = 8
grid = MOAIGrid.new ()
grid:initHexGrid ( grid_columns, grid_rows, 32 )
grid:setRepeat ( false )

for c = 1, grid_columns do
	for r = 1, grid_rows do
		grid:setTile ( c, r, ((c + r) % 4) + 1 )
	end
end

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "hex-tiles.png" )
tileDeck:setSize ( 4, 4, 0.25, 0.216796875 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -256, -256 )
prop:forceUpdate ()
layer:insertProp ( prop )

cursor = MOAIProp2D.new ()
cursor:setDeck ( tileDeck )
cursor:setScl ( grid:getTileSize ())
cursor:addScl ( -10 )
layer:insertProp ( cursor )

font = MOAIFont.new ()
font:load ( "arial-rounded.TTF" )

for c = 1, grid_columns do
	for r = 1, grid_rows do
		local x, y = grid:getTileLoc(c, r)
		x, y = prop:modelToWorld(x, y)
		textbox = MOAITextBox.new ()
		textbox:setFont ( font )
		textbox:setTextSize ( 20 )
		textbox:setRect ( -20, -20, 20, 20 )
		textbox:setLoc ( x, y )
		textbox:setYFlip ( true )
		textbox:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
		layer:insertProp ( textbox )

		textbox:setString ( string.format("%d,%d", c, r) )
	end
end


----------------------------------------------------------------
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
