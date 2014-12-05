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

grid = MOAIGridFancy.new ()
grid:initHexGrid ( 8, 8, 32 )
grid:setRepeat ( true )
grid:fill ( 1 )
grid:fillScale ( 0.95 )

local color_stash = {}
local color_choices = 16

for i = 1, color_choices do
  grid:setPalette ( i, math.random (), math.random (), math.random (), 1.0 )
end
for x = 1, 8 do
  color_stash[x] = {}
  for y = 1, 8 do
    local color = math.random ( color_choices )
    color_stash[x][y] = color
    grid:setColor ( x, y, color )
    if y == 1 then
      local r, g, b, a = grid:getPalette ( color )
    end
  end
end

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "hex-tiles.png" )
tileDeck:setSize ( 1, 1, 1, .867187500 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -256, -256 )
prop:forceUpdate ()
layer:insertProp ( prop )

----------------------------------------------------------------
local xCoord = nil
local yCoord = nil

function onPointerEvent ( x, y )

	if xCoord then
	  if color_stash[xCoord][yCoord] then
	    grid:setColor ( xCoord, yCoord, color_stash[xCoord][yCoord] )
	  end
	  grid:setScale ( xCoord, yCoord, 0.95 )
	end
	x, y = layer:wndToWorld ( x, y )
	x, y = prop:worldToModel ( x, y )
	xCoord, yCoord = grid:locToCoord ( x, y )
	xCoord, yCoord = grid:wrapCoord ( xCoord, yCoord )
	
	if xCoord then
	  grid:setScale ( xCoord, yCoord, 0.85 )
	  local old_color = grid:getColor ( xCoord, yCoord )
	  color_stash[xCoord][yCoord] = old_color
	  local r, g, b, a = grid:getPalette ( old_color )
	  grid:setPalette ( color_choices + 1, g, b, r, 1.0 )
	  grid:setColor ( xCoord, yCoord, color_choices + 1 )
	end
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
onPointerEvent ( 0, 0 )
