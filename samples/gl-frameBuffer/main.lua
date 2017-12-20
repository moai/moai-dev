----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

--local gfxQuad = MOAIGfxQuad2D.new ()
--gfxQuad:setTexture ( "moai.png" )
--gfxQuad:setRect ( -128, -128, 128, 128 )
--gfxQuad:setUVRect ( 1, 1, 0, 0 )

--local prop = MOAIProp2D.new ()
--prop:setDeck ( gfxQuad )
--layer:insertProp ( prop )

--prop:moveRot ( 360, 1.5 )

local frameBuffer = MOAIFrameBufferTexture.new ()
frameBuffer:init ( 256, 256 )

local offscreenViewport = MOAIViewport.new ()
offscreenViewport:setSize ( 256, 256 )
offscreenViewport:setScale ( 256, 256 )

local offscreenLayer = MOAIPartitionViewLayer.new ()
offscreenLayer:setViewport ( offscreenViewport )
offscreenLayer:setFrameBuffer ( frameBuffer )
offscreenLayer:setClearColor ( 1, 0, 0, 1 )
offscreenLayer:pushRenderPass ()

offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( '../resources/moai.png' )
offscreenProp:setPartition ( offscreenLayer )
offscreenProp:moveRot ( 0, 0, 720, 5 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( frameBuffer )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIGraphicsProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )
prop:moveRot ( 0, 0, -360, 5 )
