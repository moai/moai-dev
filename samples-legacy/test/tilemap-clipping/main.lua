MOAISim.openWindow( "Tilemap" , 960, 640 )

-- Set up viewport
viewport = MOAIViewport.new ()
viewport:setSize ( 960 , 640 )
viewport:setScale ( 30, -20 )

-- Using these settings eliminates horizontal clipping at the right edge of the
-- screen, however it will also stretch the viewport at 480x320 (iPhone 3GS res).
-- Doing that also comes across as a big time hack :)
-- viewport:setSize ( 1024 , 640 )
-- viewport:setScale ( 32, -20 )

viewport:setOffset ( -1, 1 )

-- Set up layer
layer = MOAILayer2D.new ()
camera = MOAICamera2D.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )

renderTable = { layer }
MOAIRenderMgr.setRenderTable ( renderTable )

-- Sizing and offsets
bledSize = 66               -- Size of a tile with a bleed
tileSize = 64               -- Actual tile size
offsetX, offsetY = 1, 1     -- Offset to compensate for bleed
cellSize = 2                -- Size of a tile cell in world units

-- Load up a background for some nice contrast
bgTex = MOAITexture.new ()
bgTex:load ( "bg.png" )

bgQuad = MOAIGfxQuad2D.new ()
bgQuad:setTexture ( bgTex )
bgQuad:setRect ( 0, 0, 960, 640 )

bg = MOAIProp2D.new ()
bg:setDeck ( bgQuad )
layer:insertProp ( bg )

-- Tilemap texture and data produced with TexturePacker
-- Load up texture
tex = MOAITexture.new ()
tex:load ( "tiles-hd.png" )
w, h = tex:getSize ()

-- Load up data
tileData = dofile ( "tiles-hd.lua" )
local function getUvQuad ( frame )

    local uv = frame.uvRect
    local q = {}

    if not frame.textureRotated then

        q.x0, q.y0 = uv.u0, uv.v1
        q.x1, q.y1 = uv.u1, uv.v1
        q.x2, q.y2 = uv.u1, uv.v0
        q.x3, q.y3 = uv.u0, uv.v0

    else

        q.x0, q.y0 = uv.u0, uv.v0
        q.x1, q.y1 = uv.u0, uv.v1
        q.x2, q.y2 = uv.u1, uv.v1
        q.x3, q.y3 = uv.u1, uv.v0

    end

    -- This is a bit of hack because our actual asset management
    -- code does some more junk to this after this function is called

    local quad = {

        u0 = q.x0,
        v0 = q.y3,
        u1 = q.x2,
        v1 = q.y1

    }

    return quad

end

frame = tileData.frames[1]
uvQuad = getUvQuad ( frame )

-- Size of the tilemap in tiles
tmapWidth = frame.spriteSourceSize.width / bledSize
tmapHeight = frame.spriteSourceSize.height / bledSize

-- Set up the deck
tileDeck = MOAITileDeck2D.new ()
tileDeck:setTexture ( tex )

tileDeck:setRect ( 0, 0, uvQuad.u1, -uvQuad.v1 )

-- See: http://getmoai.com/forums/white-lines-rendering-between-tilemap-borders-t502/?sid=6c10010761bb66710dd04d7a437865d1#p2377
tileDeck:setSize (
    tmapWidth, tmapHeight,                          -- Tile map size in tiles
    bledSize / w, bledSize / h,                     -- Tile size with bleed
    offsetX  / w, offsetY  / h,                     -- Tile gutter.
    tileSize / w, tileSize / h                      -- Actual tile size
)

-- Set up the grid
gridOffsetX, gridOffsetY = -1, 1 -- I suspect this is the root of the issue

grid = MOAIGrid.new ()
grid:setSize (
    60, 40,                                                  -- Grid dimensions in cells
    cellSize, cellSize,                                      -- Cell width in world units
    gridOffsetX / uvQuad.u1, gridOffsetY / uvQuad.v1,        -- Cell offset in texture space
    cellSize / uvQuad.u1, cellSize / uvQuad.v1               -- Tile size in texture space
)

-- Make a row
row = {}
for i=1,60 do

    row[#row + 1] = 11

end

-- Set all the rows
for i=1,40 do

    grid:setRow ( i, unpack ( row ) )

end

map = MOAIProp2D.new ()
map:setDeck ( tileDeck )
map:setGrid ( grid )
map:setLoc ( 0, 0 )

layer:insertProp ( map )

-- WASD moves the camera
-- LK scales
-- 0 restores default position and scale
function onKeyboard ( key, down )

    cx, cy = camera:getLoc ()
    moveAmt = 30 / 960

    if key == MOAIKeyCode.D then

        camera:setLoc ( cx + moveAmt, cy )

    elseif key == MOAIKeyCode.A then

        camera:setLoc ( cx - moveAmt, cy )

    elseif key == MOAIKeyCode.W then

        camera:setLoc ( cx, cy + moveAmt )

    elseif key == MOAIKeyCode.S then

        camera:setLoc ( cx, cy - moveAmt )

    end

    bg:setLoc ( camera:getLoc () )

    sx, sy = camera:getScl ()
    if key == MOAIKeyCode.L then

        camera:setScl ( sx + 0.1, sy + 0.1 )

    elseif key == MOAIKeyCode.K then

        camera:setScl ( sx - 0.1, sy - 0.1 )

    end

    if key == MOAIKeyCode.DIGIT_0 then

        camera:setLoc ( 0, 0 )
        camera:setScl ( 1, 1 )

    end

    print ( "[Camera Position] " .. cx .. ", " .. cy )
    print ( "[Camera Scale] " .. sx .. ", " .. sy )
    print ( "" )

end

if MOAIInputMgr.device.keyboard then

    MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboard )

end