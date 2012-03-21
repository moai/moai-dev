function load(lua, png)
    local frames = dofile ( lua ).frames

    local tex = MOAITexture.new ()
    tex:load ( png )
    local xtex, ytex = tex:getSize ()

    -- Annotate the frame array with uv quads and geometry rects
    for i, frame in ipairs ( frames ) do
        -- convert frame.uvRect to frame.uvQuad to handle rotation
        local uv = frame.uvRect
        local q = {}
        if not frame.textureRotated then
            -- From Moai docs: "Vertex order is clockwise from upper left (xMin, yMax)"
            q.x0, q.y0 = uv.u0, uv.v0
            q.x1, q.y1 = uv.u1, uv.v0
            q.x2, q.y2 = uv.u1, uv.v1
            q.x3, q.y3 = uv.u0, uv.v1
        else
            -- Sprite data is rotated 90 degrees CW on the texture
            -- u0v0 is still the upper-left
            q.x3, q.y3 = uv.u0, uv.v0
            q.x0, q.y0 = uv.u1, uv.v0
            q.x1, q.y1 = uv.u1, uv.v1
            q.x2, q.y2 = uv.u0, uv.v1
        end
        frame.uvQuad = q

        -- convert frame.spriteColorRect and frame.spriteSourceSize
        -- to frame.geomRect.  Origin is at x0,y0 of original sprite
        local cr = frame.spriteColorRect
        local r = {}
        r.x0 = cr.x
        r.y0 = cr.y
        r.x1 = cr.x + cr.width
        r.y1 = cr.y + cr.height
        frame.geomRect = r
    end

    -- Construct the deck
    local deck = MOAIGfxQuadDeck2D.new ()
    deck:setTexture ( tex )
    deck:reserve ( #frames )
    local names = {}
    for i, frame in ipairs ( frames ) do
        local q = frame.uvQuad
        local r = frame.geomRect
        names[frame.name] = i
        deck:setUVQuad ( i, q.x0,q.y0, q.x1,q.y1, q.x2,q.y2, q.x3,q.y3 )
        deck:setRect ( i, r.x0,r.y0, r.x1,r.y1 )
    end

    return deck, names
end

MOAISim.openWindow ( "test", 512, 512 )

gfxQuads, names = load ( 'out.lua', 'out.png' )

viewport = MOAIViewport.new ()
viewport:setSize ( 512, 512 )
viewport:setScale ( 512, 512 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuads )
prop:setIndex ( names['cathead'] )
prop:setLoc ( -100, -100 )
layer:insertProp ( prop )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuads )
prop:setIndex ( names['two'] )
prop:setLoc ( 0, 0 )
layer:insertProp ( prop )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuads )
prop:setIndex ( names['one'] )
prop:setLoc ( 100, 100 )
layer:insertProp ( prop )
