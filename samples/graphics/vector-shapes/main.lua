----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

vectorImage = MOAIVectorImage.new ()
vectorImage:reserveShapes ( 1 )
vectorImage:setPolygon ( 1, nil, {
	50,		-50,
	-50,	-50,
	-50,	50,
	50,		50,
}, {
	-50,	100,
	50,		100,
	0,		0,
}, {
	50,		-100,
	-50,	-100,
	0,		0,
}, {
	0,		0,
	100,	-50,
	100,	50,
}, {
	0,		0,
	-100,	50,
	-100,	-50,
}, {
	25,		-25,
	-25,	-25,
	-25,	25,
	25,		25,
})

vectorImage:setFillStyle ( 1, MOAIVectorImage.FILL_SOLID )
vectorImage:setFillColor ( 1, 1, 0, 0, 1 )

vectorImage:setLineStyle ( 1, MOAIVectorImage.LINE_VECTOR )
vectorImage:setLineColor ( 1, 1, 1, 1, 1 )

function onDraw ( index, xOff, yOff, xFlip, yFlip )

	MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
	MOAIGfxDevice.setPenWidth ( 2 )
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -64, -64, 64, 64 )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )

prop = MOAIProp2D.new ()
prop:setDeck ( vectorImage )
prop:setCullMode ( MOAIProp.CULL_FRONT )
layer:insertProp ( prop )

prop:moveRot ( -360, 1.5 )
