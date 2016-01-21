--https://en.wikipedia.org/wiki/Kernel_(image_processing)

image = MOAIImage.new ()
image:load ( '../resources/moai.png' )

--[[
local kernel = {
	{ 1, 2, 1 },
	{ 2, 4, 2 },
	{ 1, 2, 1 },
}
]]--

--[[
local kernel = {
	{ 0, -1, 0 },
	{ -1, 5, -1 },
	{ 0, -1, 0 },
}
]]--

--[[
local kernel = {
	{ -1, -1, -1 },
	{ -1, 8, -1 },
	{ -1, -1, -1 },
}
]]--

--local kernel = { 1, 2, 4, 8, 4, 2, 1 }

local kernel = MOAIImage.calculateGaussianKernel ( 16 )

image = image:convolve ( kernel, true )

local width, height = image:getSize ()

local hWidth, hHeight = width / 2, height / 2

MOAISim.openWindow ( "test", width, height )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width, -height )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )


gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( image )
gfxQuad:setRect ( -hWidth, -hHeight, hWidth, hHeight )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )