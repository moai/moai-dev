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
grid:initHexGrid ( 8, 8, 32 )
grid:setRepeat ( true )

grid:fill(1)

tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( "hex-tiles.png" )
tileDeck:setSize ( 1, 1, 1, .867187500 )

prop = MOAIProp2D.new ()
prop:setDeck ( tileDeck )
prop:setGrid ( grid )
prop:setLoc ( -256, -256 )
prop:forceUpdate ()
layer:insertProp ( prop )

color_stash = {}

for x = 1, 8 do
  color_stash[x] = {}
  for y = 1, 8 do
    grid:setColor(x, y, math.random(), math.random(), math.random())
    grid:setScale(x, y, 0.95)
    color_stash[x][y] = { grid:getColor(x, y) }
  end
end

----------------------------------------------------------------
local xCoord = nil
local yCoord = nil

function onPointerEvent ( x, y )

	if xCoord then
	  if color_stash[xCoord][yCoord] then
	    local r, g, b = unpack ( color_stash[xCoord][yCoord] )
	    grid:setColor(xCoord, yCoord, r, g, b)
	  else
	    print(string.format("no color for %d, %d", xCoord, yCoord))
	  end
	  grid:setScale(xCoord, yCoord, 0.95)
	end
	x, y = layer:wndToWorld ( x, y )
	x, y = prop:worldToModel ( x, y )
	xCoord, yCoord = grid:locToCoord ( x, y )
	xCoord, yCoord = grid:wrapCoord ( xCoord, yCoord )
	
	if xCoord then
	  grid:setScale(xCoord, yCoord, 0.85)
	  if color_stash[xCoord][yCoord] then
	    local r, g, b = unpack ( color_stash[xCoord][yCoord] )
	    grid:setColor(xCoord, yCoord, g, b, r)
	  else
	    print(string.format("no color for %d, %d", xCoord, yCoord))
	  end
	end
end

MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
onPointerEvent ( 0, 0 )
