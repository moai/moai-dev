----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

TEXTURE_256_256 = MOAIPool.addFactory (

	function ()
		return MOAITexture.new ():init ( 256, 256 )
	end
)

COLOR_FBO_256_256 = MOAIPool.addFactory (

	function ()
		return MOAIFrameBuffer.new ()
	end,

	function ( frameBuffer, scope )
		frameBuffer:setAttachment ( MOAIPool.provision ( TEXTURE_256_256, scope ))
	end
)

offscreenViewport = MOAIViewport.new ()
	:setSize ( 256, 256 )
	:setScale ( 256, 256 )

-- add a prop to render
offscreenProp = MOAIProp.new ():setDeck ( '../resources/moai.png' )
offscreenProp:moveRot ( 0, 0, 720, 5 )

-- now set up a layer and render pass to render to the default/window frame buffer
viewport = MOAIViewport.new ()
	:setSize ( 320, 480 )
	:setScale ( 320, 480 )

-- deck to render a quad using the framebuffer texture
gfxQuad = MOAISpriteDeck2D.new ()
	:setRect ( -128, -128, 128, 128 )
	:setUVRect ( 0, 0, 1, 1 )

partition = MOAIPartition.new ()

-- prop to render the framebuffer texture deck
prop = MOAIGraphicsProp.new ():setDeck ( gfxQuad )
prop:moveRot ( 0, 0, -360, 5 )
prop:setPartition ( partition )

function render ( draw, node, phase )

	local frameBuffer = MOAIPool.provision ( COLOR_FBO_256_256, node:localScope ())

	draw
		:setFrameBuffer ( frameBuffer )
		:setViewRect ( offscreenViewport )
		:clearSurface ()
		:setViewProj ( offscreenViewport )

	offscreenProp:render ()

	draw
		:setFrameBuffer ()
		:setViewRect ( viewport )
		:clearSurface ()
		:setViewProj ( viewport )
		:setTexture ( frameBuffer:getAttachment ())

	prop:render ()
end

node = MOAIRenderNode.new ():setRender ( render )
MOAIGfxMgr.setRender ( node )
