----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 512, 512 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local databuff = MOAIDataBuffer.new ()
databuff:load ( 'moai.png' )
databuff:deflate ()
databuff:inflate ()
databuff:save ( 'moai2.png' )

-- currently crashes if you try to use moai2.png
-- gfxQuad = MOAIGfxQuad2D.new ()
-- gfxQuad:setTexture ( 'moai2.png' )
-- gfxQuad:setRect ( -128, -128, 128, 128 )
-- gfxQuad:setUVRect ( 0, 0, 1, 1 )

-- prop = MOAIProp2D.new ()
-- prop:setDeck ( gfxQuad )
-- layer:insertProp ( prop )