----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

require ( 'geometry' )
require ( 'shaders' )

WIN_WIDTH = 320
WIN_HEIGHT = 480

MOAISim.openWindow ( "test", WIN_WIDTH, WIN_HEIGHT )

offscreenViewport = MOAIViewport.new ()
offscreenViewport:setViewSize ( WIN_WIDTH, WIN_HEIGHT )
offscreenViewport:setViewScale ( WIN_WIDTH, WIN_HEIGHT )

offscreenCamera = MOAICamera.new ()
offscreenCamera:setType ( MOAICamera.CAMERA_TYPE_3D )
offscreenCamera:setLoc ( 0, 0, offscreenCamera:getFocalLength ( WIN_WIDTH ))

offscreenLayer = MOAIPartitionViewLayer.new ()
offscreenLayer:setViewport ( offscreenViewport )
offscreenLayer:setCamera ( offscreenCamera )

offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( makeCube ( 128, '../resources/moai.png' ))
offscreenProp:moveRot ( 360, 360, 0, 6 )
offscreenProp:setPartition ( offscreenLayer )

function makeFBO ( width, height, withDepth )

    local buffer = MOAIFrameBuffer.new ();

    local color = MOAITexture.new ():init ( width, height )
    color:setFilter ( MOAITexture.FILTER_LINEAR, MOAITexture.FILTER_LINEAR )
    buffer:addAttachment ( color, MOAIFrameBuffer.COLOR )
    buffer.color = color

    if ( withDepth ) then

        local depth = MOAITexture.new ():init ( width, height )
        depth:setInternalFormat ( MOAITexture.PIXEL_FORMAT_DEPTH_COMPONENT16 )
        depth:setPixelType ( MOAITexture.PIXEL_TYPE_FLOAT )

        buffer:addAttachment ( color, MOAIFrameBuffer.DEPTH )
        buffer.depth = depth
    end

    return buffer
end

mainFB      = makeFBO (  WIN_WIDTH, WIN_HEIGHT )

gaussFB0    = makeFBO (  WIN_WIDTH / 4, WIN_HEIGHT / 4, false )
gaussFB1    = makeFBO (  WIN_WIDTH / 4, WIN_HEIGHT / 4, false )

function render ( draw )

	draw:setFrameBuffer ( mainFB )
	draw:clearSurface ()
	offscreenLayer:render ()

    draw:setFrameBuffer ( gaussFB0 )
    draw:clearSurface ()
    draw:setTexture ( mainFB.color )
    draw:compose ( emissiveShader )

	draw:setFrameBuffer ()
	draw:setTexture ( gaussFB0.color )
	draw:compose ( deck2DShader )
end

MOAIGfxMgr.pushRenderable ( render )
