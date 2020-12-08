----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

TEXTURE_256_256 = MOAIRenderResourcePool.addFactory (

	function ()
		return MOAITexture.new ():init ( 256, 256 )
	end
)

COLOR_FBO_256_256 = MOAIRenderResourcePool.addFactory (

	function ()
		return MOAIFrameBuffer.new ()
	end,

	function ( frameBuffer )
		frameBuffer:setAttachment ( MOAIRenderResourcePool.provision ( TEXTURE_256_256 ))
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

-- prop to render the framebuffer texture deck
prop = MOAIGraphicsProp.new ():setDeck ( gfxQuad )
prop:moveRot ( 0, 0, -360, 5 )

node = MOAIRenderNode.new ()

function render ( draw )

	local frameBuffer = MOAIRenderResourcePool.provision ( COLOR_FBO_256_256 )

	draw
		:setFrameBuffer ( frameBuffer )
		:setViewRect ( offscreenViewport )
		:clearSurface ()
		:setViewProj ( offscreenViewport )

	offscreenProp:render ()

	draw
		:setTexture ( frameBuffer:getAttachment ()) -- set this here to retain texture
		:setFrameBuffer ()
		:setViewRect ( viewport )
		:clearSurface ()
		:setViewProj ( viewport )

	prop:render ()
end

MOAIGfxMgr.setRender ( render )
