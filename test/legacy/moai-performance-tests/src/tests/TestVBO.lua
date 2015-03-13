module(..., package.seeall)

PROP_SIZE = 6000

function start()
    print("Start:TestVBO")

    local viewport = MOAIViewport.new ()
    viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )
    viewport:setScale ( SCREEN_WIDTH, SCREEN_HEIGHT )

    local layer = MOAILayer.new ()
    layer:setViewport ( viewport )
    layer:setSortMode ( MOAILayer.SORT_NONE )

    MOAIGfxDevice.getFrameBuffer():setRenderTable({ layer })

    initBuffers(PROP_SIZE)

    for i = 1, PROP_SIZE do
        local x = math.random(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2)
        local y = math.random(-SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2)
        addSprite(x, y)
    end

    vbo:bless()

    mesh = MOAIMesh.new ()
    mesh:setTexture ( "assets/moai.png" )
    mesh:setVertexBuffer ( vbo )
    mesh:setIndexBuffer ( ibo )
    mesh:setPrimType ( MOAIMesh.GL_TRIANGLES )

    prop = MOAIProp.new ()
    prop:setDeck ( mesh )
    layer:insertProp ( prop )
end

function initBuffers(numSprites)
    local vertexFormat = MOAIVertexFormat.new ()

    vertexFormat:declareCoord ( 1, MOAIVertexFormat.GL_FLOAT, 3 )
    vertexFormat:declareUV ( 2, MOAIVertexFormat.GL_FLOAT, 2 )
    vertexFormat:declareColor ( 3, MOAIVertexFormat.GL_UNSIGNED_BYTE )

    vbo = MOAIVertexBuffer.new ()
    vbo:setFormat ( vertexFormat )
    vbo:reserveVerts ( 4 * numSprites )

    ibo = MOAIIndexBuffer.new ()
    ibo:reserve ( 6 * numSprites )

    curSprite = 0
end

function stop()
    MOAIGfxDevice.getFrameBuffer():setRenderTable({})
end

function addSprite(x, y)
    vbo:writeFloat ( x - 32, y + 32, 0 )
    vbo:writeFloat ( 0, 0 )
    vbo:writeColor32 ( 1, 1, 1, 1 )

    vbo:writeFloat ( x + 32, y + 32, 0 )
    vbo:writeFloat ( 1, 0 )
    vbo:writeColor32 ( 1, 1, 1, 1 )

    vbo:writeFloat ( x + 32, y - 32, 0 )
    vbo:writeFloat ( 1, 1 )
    vbo:writeColor32 ( 1, 1, 1, 1 )

    vbo:writeFloat ( x - 32, y - 32, 0 )
    vbo:writeFloat ( 0, 1 )
    vbo:writeColor32 ( 1, 1, 1, 1 )

    ibo:setIndex ( curSprite * 6 + 1, curSprite * 4 + 1 )
    ibo:setIndex ( curSprite * 6 + 2, curSprite * 4 + 4 )
    ibo:setIndex ( curSprite * 6 + 3, curSprite * 4 + 3 )
    ibo:setIndex ( curSprite * 6 + 4, curSprite * 4 + 1 )
    ibo:setIndex ( curSprite * 6 + 5, curSprite * 4 + 3 )
    ibo:setIndex ( curSprite * 6 + 6, curSprite * 4 + 2 )

    curSprite = curSprite + 1
end
