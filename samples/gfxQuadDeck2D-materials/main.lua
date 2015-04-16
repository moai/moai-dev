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

-- you can set a material batch on the deck or on the prop
-- the prop's will override the deck's

--gfxQuadDeck:reserveTextures ( 3 )
--gfxQuadDeck:setTexture ( 1, "../resources/moai.png" )
--gfxQuadDeck:setTexture ( 2, "../resources/test.png" )
--gfxQuadDeck:setTexture ( 3, "../resources/numbers.png" )
--gfxQuadDeck:setIndexBatchSize ( 4 )

gfxQuadDeck:reserve ( 12 )

-- by default, the material IDs will default to MOAIMaterialBatch.UNKNOWN
-- in this case, the prop's index will be used in tandem with the indexBatchSize
-- to determine the material ID. if an ID is set, that material will be used.

-- items 1, 5, and 9 below have their material's ID's set explicitely
-- the rest will use prop index / indexBatchSize

gfxQuadDeck:setRect ( 1, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 1, 0, 0.5, 0.5, 0 )
gfxQuadDeck:setMaterialID ( 1, 2 )

gfxQuadDeck:setRect ( 2, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 2, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 3, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 3, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 4, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 4, 0.5, 1, 1, 0.5 )


gfxQuadDeck:setRect ( 5, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 5, 0, 0.5, 0.5, 0 )
gfxQuadDeck:setMaterialID ( 5, 3 )

gfxQuadDeck:setRect ( 6, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 6, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 7, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 7, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 8, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 8, 0.5, 1, 1, 0.5 )


gfxQuadDeck:setRect ( 9, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 9, 0, 0.5, 0.5, 0 )
gfxQuadDeck:setMaterialID ( 9, 1 )

gfxQuadDeck:setRect ( 10, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 10, 0.5, 0.5, 1, 0 )

gfxQuadDeck:setRect ( 11, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 11, 0, 1, 0.5, 0.5 )

gfxQuadDeck:setRect ( 12, -64, -64, 64, 64 )
gfxQuadDeck:setUVRect ( 12, 0.5, 1, 1, 0.5 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuadDeck )

prop:setTexture ( 1, "../resources/moai.png" )
prop:setTexture ( 2, "../resources/test.png" )
prop:setTexture ( 3, "../resources/numbers.png" )
prop:setIndexBatchSize ( 4 )

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
