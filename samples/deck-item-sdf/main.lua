----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

WIN_WIDTH = 512
WIN_HEIGHT = 512

MOAISim.openWindow ( "test", WIN_WIDTH, WIN_HEIGHT )

-- makes a layer showing a single deck item
-- if view size is omitted, layer will default to fit the deck item (with padding)
makeLayerWithDeckItem = function ( deck, idx, pad, viewWidth, viewHeight )

	idx = idx or 1
	pad = pad or 0

	local xMin, yMin, zMin, xMax, yMax, zMax = deck:getBounds ( idx )

	xMin = xMin - pad
	yMin = yMin - pad
	xMax = xMax + pad
	yMax = yMax + pad

	local width = xMax - xMin
	local height = yMax - yMin

	viewWidth = viewWidth or width
	viewHeight = viewHeight or height

	local viewport = MOAIViewport.new ()
	viewport:setSize ( viewWidth, viewHeight )
	viewport:setScale ( viewWidth, viewHeight )

	local layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	
	local prop = MOAIProp2D.new ()
	prop:setDeck ( deck )
	prop:setIndex ( idx )
	layer:insertProp ( prop )

	return layer, prop, width, height
end

-- renders a deck item to a (padded) offscreen buffer then capturs and returns an image
makeImageFromDeckItem = function ( deck, idx, pad )

	local layer, prop, width, height = makeLayerWithDeckItem ( deck, idx, pad )

	local frameBuffer = MOAIFrameBufferTexture.new ()
	frameBuffer:setRenderTable ({ layer })
	frameBuffer:init ( width, height )
	frameBuffer:setClearColor ( 0, 0, 0, 0 )
	MOAIRenderMgr.setBufferTable ({ frameBuffer })

	frameBuffer.grabNextFrame ( frameBuffer, MOAIImage.new ())

	while frameBuffer:isPendingGrab () do coroutine:yield () end

	local image = frameBuffer:getGrabbedImage ()
	assert ( image )

	return image, width, height
end

-- creates a (padded) sdf image from a deck item
makeSdfFromDeckItem = function ( deck, idx, pad )

	local image, width, height = makeImageFromDeckItem ( deck, idx, pad )
	image:generateSDFAA ( 0, 0, width, height, pad )

	return image, width, height
end

main = function ()

	local texture = MOAITexture.new ()
	texture:load ( '../resources/moai-alpha.png' )
	while texture:getResourceState () ~= MOAITexture.STATE_READY_TO_BIND do coroutine:yield () end

	-- load the deck
	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( texture )
	gfxQuad:setRect ( -128, -128, 128, 128 )
	gfxQuad:setUVRect ( 0, 1, 1, 0 )

	local image, width, height = makeSdfFromDeckItem ( gfxQuad, 1, 24 )

	gfxQuad:setTexture ( image )
	local hWidth, hHeight = width * 0.5, height * 0.5
	gfxQuad:setRect ( -hWidth, -hHeight, hWidth, hHeight )

	local layer, prop = makeLayerWithDeckItem ( gfxQuad, 1, 0, WIN_WIDTH, WIN_HEIGHT )
	layer:setClearColor ( 1, 1, 1, 1 )

	MOAISim.pushRenderPass ( layer )

end

thread = MOAICoroutine.new ()
thread:run ( main )
