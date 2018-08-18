--[[

  @copyright (c) 2017 Upstart Illustration LLC. All rights reserved.
  ]]

local priorityOn = false

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

mainLayer = MOAILayer.new ()
mainLayer:setViewport ( viewport )
if priorityOn then
    mainLayer:setSortMode ( MOAILayer.SORT_PRIORITY_ASCENDING )
end
MOAISim.pushRenderPass ( mainLayer )

-- Ground
ground = MOAIGrid.new ()
ground:setSize ( 2, 2, 32, 32 )
ground:setRow ( 1, 2, 2 )
ground:setRow ( 2, 2, 2 )
ground:setRepeat ( false, false )

bridge = MOAIGrid.new ()
bridge:setSize ( 2, 2, 32, 32 )
bridge:setRow ( 1, 3, 0 )
bridge:setRow ( 2, 0, 0 )
bridge:setRepeat ( false, false )

sky = MOAIGrid.new ()
sky:setSize ( 2, 2, 32, 32 )
sky:setRow ( 1, 0, 10 )
sky:setRow ( 2, 0, 0 )
sky:setRepeat ( false, false )

deck = MOAITileDeck2D:new ()
deck:setTexture ( "numbers.png" )
deck:setSize ( 8, 8 )
deck:setRect ( -0.5, -0.5, 0.5, 0.5 )

function addGrid(grid, priority)
    local prop = MOAIProp.new ()
    prop:setDeck ( deck )
    prop:setGrid ( grid )
    prop:addLoc ( -32, -32 )
    if priorityOn then
        prop:setPriority( priority )
    end
    prop:setPartition ( mainLayer )
end

addGrid ( ground, 1 )
addGrid ( bridge, 3 )

-- Props
layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:setSortMode ( MOAILayer.SORT_Y_ASCENDING )
if priorityOn then
    layer:setPriority ( 2 )
end

deck2 = MOAITileDeck2D:new ()
deck2:setTexture ( "numbers.png" )
deck2:setSize ( 8, 8 )
deck2:setRect ( -16, -16, 16, 16 )

function addProp ( x, y, index )
    local prop = MOAIProp.new ()
    prop:setDeck ( deck2 )
    prop:setIndex ( index )
    prop:addLoc ( x, y )
    prop:setPartition ( layer )
end

addProp ( 0,   0, 4 )
addProp ( 0, -10, 5 )
addProp ( 0, -20, 6 )
addProp ( 0, -30, 7 )

addGrid ( sky, 4 )
layer:setPartition ( mainLayer )

