----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

COLOR_FBO_256_256 = MOAIPool.addFactory (
	function ()
		local frameBuffer = MOAIFrameBuffer.new ();
		frameBuffer:setAttachment ( MOAITexture.new ():init ( 256, 256 ));
		return frameBuffer;
	end
)

offscreenViewport = MOAIViewport.new ()
offscreenViewport:setViewSize ( 256, 256 )
offscreenViewport:setViewScale ( 256, 256 )

-- add a prop to render
offscreenProp = MOAIProp.new ():setDeck ( '../resources/moai.png' )
offscreenProp:moveRot ( 0, 0, 720, 5 )

-- now set up a layer and render pass to render to the default/window frame buffer
viewport = MOAIViewport.new ()
viewport:setViewSize ( 320, 480 )
viewport:setViewScale ( 320, 480 )

-- deck to render a quad using the framebuffer texture
gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

partition = MOAIPartition.new ()

-- prop to render the framebuffer texture deck
prop = MOAIGraphicsProp.new ():setDeck ( gfxQuad )
prop:moveRot ( 0, 0, -360, 5 )
prop:setPartition ( partition )

function render ( draw )

	local scope = MOAIPool.provision ( MOAIScope )

	local frameBuffer = MOAIPool.provision ( COLOR_FBO_256_256, scope )

	draw:setFrameBuffer ( frameBuffer )
	draw:setViewRect ( offscreenViewport )
	draw:clearSurface ()
	draw:setViewProj ( offscreenViewport )

	offscreenProp:render ()

	draw:setFrameBuffer ()
	draw:setViewRect ( viewport )
	draw:clearSurface ()
	draw:setViewProj ( viewport )
	draw:setTexture ( frameBuffer:getAttachment ())

	local partitionResults = MOAIPool.provision ( MOAIPartitionResultBuffer, scope )
	partition:hullsForViewProj ( partitionResults, viewport, nil, MOAIRenderNode.getRenderTypeID ())

	partitionResults:render ()

	MOAIPool.remit ( scope )
end

node = MOAIRenderNode.new ():setRender ( render )
-- MOAIGfxMgr.setRenderRoot ( node )
MOAIGfxMgr.pushRenderNode ( node )
