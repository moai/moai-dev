----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( 'hello, moai!' )

MOAISim.openWindow ( "test", 320, 480 )

function initFrameBuffer ()

	local gfxQuad = MOAIGfxQuad2D.new ()
	gfxQuad:setTexture ( "cathead.png" )
	gfxQuad:setRect ( -64, -64, 64, 64 )
	gfxQuad:setUVRect ( 0, 0, 1, 1 )

	local viewport = MOAIViewport.new ()
	viewport:setSize ( 128, 128 )
	viewport:setScale ( 128, 128 )

	local frameBuffer = MOAITexture.new ()
	frameBuffer:initFrameBuffer ( 128, 128 )

	local layer = MOAILayer2D.new ()
	layer:setViewport ( viewport )
	layer:setFrameBuffer ( frameBuffer )
	MOAISim.pushRenderPass ( layer )

	local prop = MOAIProp2D.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )

	prop:moveRot ( 360, 1.5 )

	return frameBuffer
	
end

frameBuffer = initFrameBuffer ()

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( frameBuffer )
gfxQuad:setRect ( -64, -64, 64, 64 )
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

prop:moveRot ( -360, 1.5 )
