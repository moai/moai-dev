 
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

local action = camera:moveLoc(10000,0,20,MOAIEaseType.LINEAR)
local perfPrint = 10
local function threadFunc ()
	while true do
		while action:isBusy () do
			coroutine.yield ()
			--if perfPrint == 10 then
				--print ( MOAISim.getPerformance ())
				--print ( camera:getWorldLoc ())
			--	perfPrint = 0
			--end
			--perfPrint = perfPrint + 1
		end
		print "RESTARTING - NOT A HITCH"
		camera:setLoc ( 0, 0 )
		action = camera:moveLoc(10000,0,20,MOAIEaseType.LINEAR)
	end
end
local thread = MOAIThread.new ()
thread:run ( threadFunc ) 
