----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuadDeck = MOAIGfxQuadDeck2D.new ()

gfxQuadDeck:reserveTextures ( 3 )
gfxQuadDeck:setTexture ( 1, "../resources/moai.png" )
gfxQuadDeck:setTexture ( 2, "../resources/test.png" )
gfxQuadDeck:setTexture ( 3, "../resources/numbers.png" )
gfxQuadDeck:setTextureBatchSize ( 4 )

gfxQuadDeck:reserve ( 12 )

gfxQuadDeck:setRect ( 1, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 1, 0, 0.5, 0.5, 0 )

gfxQuadDeck:setRect ( 2, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 2, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 3, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 3, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 4, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 4, 0.5, 1, 1, 0.5 )


gfxQuadDeck:setRect ( 5, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 5, 0, 0.5, 0.5, 0 )

gfxQuadDeck:setRect ( 6, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 6, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 7, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 7, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 8, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 8, 0.5, 1, 1, 0.5 )


gfxQuadDeck:setRect ( 9, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 9, 0, 0.5, 0.5, 0 )

gfxQuadDeck:setRect ( 10, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 10, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 11, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 11, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 12, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 12, 0.5, 1, 1, 0.5 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadDeck )
layer:insertProp ( prop )

index = 1

function onMouseLeftEvent ( down )
	if down == true then
		index = ( index % 12 ) + 1
		print ( index )
		prop:setIndex ( index )
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )
