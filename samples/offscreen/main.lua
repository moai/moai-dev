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

prop = MOAIProp.new ()
prop:setPartition ( layer )
prop:moveRot ( 0, 0, 360, 5 )

function onDraw ( index )

    MOAIDraw.pushGfxState ()

    print ( 'OFFSCREEN SETUP' )

    -- set up an offscreen buffer to render to
    local frameBuffer = MOAIFrameBufferTexture.new ()
    frameBuffer:init ( 256, 256 )

    local offscreenViewport = MOAIViewport.new ()
    offscreenViewport:setSize ( 256, 256 )
    offscreenViewport:setScale ( 256, -256 )

    -- need a layer to render the offscreen scene
    local offscreenLayer = MOAIPartitionViewLayer.new ()
    offscreenLayer:setViewport ( offscreenViewport )
    offscreenLayer:setFrameBuffer ( frameBuffer )

    -- add a prop to render
    local offscreenProp = MOAIProp.new ()
    offscreenProp:setDeck ( '../resources/moai.png' )
    offscreenProp:setPartition ( offscreenLayer )
    offscreenProp:forceUpdate ();
    
    print ( 'OFFSCREEN DRAW' )

    offscreenLayer:draw ()

    print ( 'SET DECK' )

    MOAIDraw.popGfxState ()

    prop:setDeck ( frameBuffer )
end

drawDeck = MOAIDrawDeck.new ()
drawDeck:setBounds ()
drawDeck:setDrawCallback ( onDraw )

prop:setDeck ( drawDeck )
