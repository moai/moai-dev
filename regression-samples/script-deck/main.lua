local width, height = 320, 480

MOAISim.openWindow ( "test", width, height )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local r, g, b, a = 1,1,1,1
local deck = MOAIScriptDeck.new()
deck:setRect(0, 0, width, height)
deck:setDrawCallback(function()
	MOAIGfxDevice.setPenColor(r, g, b, a)
	MOAIDraw.fillRect(0, 0, width, height)
end)
local prop = MOAIGraphicsProp.new()
prop:setDeck(deck)
prop:setPriority(-1)  -- force background
layer:insertProp(prop)