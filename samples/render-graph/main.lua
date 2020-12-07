----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

-- set up an offscreen texture to render to
texture = MOAITexture.new ()
texture:init ( 256, 256 )

-- set up an offscreen buffer to render to
frameBuffer = MOAIFrameBuffer.new ()
frameBuffer:setAttachment ( texture )

offscreenViewport = MOAIViewport.new ()
offscreenViewport:setSize ( 256, 256 )
offscreenViewport:setScale ( 256, 256 )

-- add a prop to render
offscreenProp = MOAIProp.new ()
offscreenProp:setDeck ( '../resources/moai.png' )
offscreenProp:moveRot ( 0, 0, 720, 5 )

-- now set up a layer and render pass to render to the default/window frame buffer
viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

-- deck to render a quad using the framebuffer texture
gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:gfx ():setTexture ( texture )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

-- prop to render the framebuffer texture deck
prop = MOAIGraphicsProp.new ()
prop:setDeck ( gfxQuad )
prop:moveRot ( 0, 0, -360, 5 )

node = MOAIRenderNode.new ()

function render ( draw )

	draw
		:setFrameBuffer ( frameBuffer )
		:setViewRect ( offscreenViewport )
		:setScissorRect ( offscreenViewport )
		:clearSurface ()
		:setViewProj ( offscreenViewport )

	offscreenProp:render ()

	draw
		:setFrameBuffer ()
		:setViewRect ( viewport )
		:setScissorRect ( viewport )
		:clearSurface ()
		:setViewProj ( viewport )

	prop:render ()

	-- layer:render ( context, draw )

	-- local fb = context:grabFramebuffer ( '' )
	-- draw:pushFramebuffer ( fb )
	-- draw:clearFramebuffer ()
	-- layer:render ( context, draw )
	-- draw:popFramebuffer ()
	-- context:render ( fb, draw )
	-- context:release ( fb )

	-- grab temp fb
	-- load color channel shader (R)
	-- render to fb
	-- render fb frame
	-- release temp fb

	-- grab temp fb
	-- load color channel shader (G)
	-- render to fb
	-- render fb frame
	-- release temp fb

	-- grab temp fb
	-- load color channel shader (B)
	-- render to fb
	-- render fb frame
	-- release temp fb

	-- node:render ()
end

MOAIGfxMgr.setRender ( render )
