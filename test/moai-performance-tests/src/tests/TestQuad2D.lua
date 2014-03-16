module(..., package.seeall)

PROP_SIZE = 6000

function start()
    print("Start:TestQuad2D")

    local viewport = MOAIViewport.new ()
    viewport:setSize ( SCREEN_WIDTH, SCREEN_HEIGHT )
    viewport:setScale ( SCREEN_WIDTH, SCREEN_HEIGHT )

    local layer = MOAILayer.new ()
    layer:setViewport ( viewport )
    layer:setSortMode ( MOAILayer.SORT_NONE )

    MOAIGfxDevice.getFrameBuffer():setRenderTable({ layer })

    local deck = MOAIGfxQuad2D.new ()
    deck:setTexture ( "assets/moai.png" )
    deck:setRect ( -32, -32, 32, 32 )
    deck:setUVRect ( 0, 0, 1, 1 )

    for i = 1, PROP_SIZE do
        local x = math.random(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2)
        local y = math.random(-SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2)

        local prop = MOAIProp2D.new ()
        prop:setDeck ( deck )
        prop:setLoc ( x, y )
        layer:insertProp ( prop )
    end
end

function stop()
    MOAIGfxDevice.getFrameBuffer():setRenderTable({})
end