----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

-- set up an offscreen buffer to render to
frameBuffer = MOAIFrameBufferTexture.new ()
frameBuffer:init ( 256, 256 )

offscreenViewport = MOAIViewport.new ()
offscreenViewport:setSize ( 256, 256 )
offscreenViewport:setScale ( 256, 256 )

-- need a layer to render the offscreen scene
offscreenLayer = MOAIPartitionViewLayer.new ()
offscreenLayer:setViewport ( offscreenViewport )
offscreenLayer:setFrameBuffer ( frameBuffer ) -- render to our offscreen buffer
offscreenLayer:setClearColor ( 1, 0, 0, 1 )
offscreenLayer:pushRenderPass () -- add the layer to the render queue

-- add a prop to render
offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( '../resources/moai.png' )
offscreenProp:setPartition ( offscreenLayer )
offscreenProp:moveRot ( 0, 0, 720, 5 )

-- not set up a layer and render pass to render to the default/window frame buffer
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

-- no special framebuffer is specified for layer, so it will render to the window's frame buffer
layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass () -- add it to the render queue; will render *after* the offscreen scene

-- deck to render a quad using the framebuffer texture
gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( frameBuffer )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

-- prop to render the framebuffer texture deck
prop = MOAIGraphicsProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )
prop:moveRot ( 0, 0, -360, 5 )
