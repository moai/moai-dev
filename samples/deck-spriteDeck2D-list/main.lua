----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( "../resources/test.png" )

spriteDeck:reserveQuads ( 4 )
spriteDeck:setRect ( 1, -64, 0, 0, 64 )
spriteDeck:setRect ( 2, 0, 0, 64, 64 )
spriteDeck:setRect ( 3, -64, -64, 0, 0 )
spriteDeck:setRect ( 4, 0, -64, 64, 0 )

spriteDeck:reserveUVQuads ( 4 )
spriteDeck:setUVRect ( 1, 0.0, 0.5, 0.5, 0.0 )
spriteDeck:setUVRect ( 2, 0.5, 0.5, 1.0, 0.0 )
spriteDeck:setUVRect ( 3, 0.0, 1.0, 0.5, 0.5 )
spriteDeck:setUVRect ( 4, 0.5, 1.0, 1.0, 0.5 )

spriteDeck:reserveSprites ( 4 )
spriteDeck:setSprite ( 1, 1, 1 )
spriteDeck:setSprite ( 2, 2, 2 )
spriteDeck:setSprite ( 3, 3, 3 )
spriteDeck:setSprite ( 4, 4, 4 )

spriteDeck:reserveSpriteLists ( 1 )
spriteDeck:setSpriteList ( 1, 1, 4 )

prop = MOAIProp.new ()
prop:setDeck ( spriteDeck )
prop:setLoc ( 64, 64, 0 )
prop:setPartition ( layer )
