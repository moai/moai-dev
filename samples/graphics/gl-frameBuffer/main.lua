----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

function initFrameBuffer ()

	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "moai.png" )
	gfxQuad:setRect ( -128, -128, 128, 128 )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	local viewport = MOAIViewport.new ()
	viewport:setSize ( 256, 256 )
	viewport:setScale ( 256, 256 )

	local layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	
	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )

	prop:moveRot ( 360, 1.5 )

	local frameBuffer = MOAIFrameBufferTexture.new ()
	frameBuffer:setRenderTable ({ layer })
	frameBuffer:init ( 256, 256 )
	frameBuffer:setClearColor ( 1, 0, 0, 1 )
	MOAIRenderMgr.setBufferTable ({ frameBuffer })
	
	return frameBuffer
	
end

frameBuffer = initFrameBuffer ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( frameBuffer )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )
prop:moveRot ( -180, 1.5 )
