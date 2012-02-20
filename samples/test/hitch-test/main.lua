 

local screenWidth, screenHeight = 960,640

MOAISim.openWindow ( "scrolltest", screenWidth, screenHeight)

local viewport = MOAIViewport.new()

viewport:setSize(screenWidth, screenHeight)

viewport:setScale(screenWidth, screenHeight)

viewport:setOffset(-1, -1)

 

local tex = MOAITexture.new ()

tex:load ('pixelletters.png')

local deck = MOAITileDeck2D.new()

deck:setTexture(tex)

deck:setSize(1,1)

local w,h = tex:getSize()

deck:setRect(0,0,w,h)

local layer = MOAILayer2D.new()

layer:setViewport (viewport)

local camera = MOAITransform.new()

layer:setCamera(camera)

MOAISim.pushRenderPass (layer)

 

for i=1,10 do

        local sprite = MOAIProp2D.new()    

        sprite:setDeck(deck)

        sprite:setIndex(1)

        layer:insertProp(sprite)

        sprite:setLoc((i-1)*w,0)

end

 

camera:moveLoc(10000,0,20,MOAIEaseType.LINEAR)

 
