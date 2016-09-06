----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad1 = MOAIGfxQuad2D.new ()
gfxQuad1:setTexture ( "../resources/moai.png" )
gfxQuad1:setRect ( -128, -128, 128, 128 )
gfxQuad1:setUVRect ( 0, 0, 1, 1 )

prop1 = MOAIProp2D.new ()
prop1:setDeck ( gfxQuad1 )
layer:insertProp ( prop1 )
prop1:moveRot ( 360, 5 )

gfxQuad2 = MOAIGfxQuad2D.new ()
gfxQuad2:setTexture ( "../resources/test.png" )
gfxQuad2:setRect ( -128, -128, 128, 128 )
gfxQuad2:setUVRect ( 0, 0, 1, 1 )

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad2 )
prop2:setColor ( 1, 1, 1, 0 )
layer:insertProp ( prop2 )
prop2:moveRot ( -360, 5 )

onRender = function ()

	MOAIDraw.setViewRect ( viewport )
	MOAIDraw.setScissorRect ( viewport )

	MOAIDraw.setClearColor ( 0, 0, 0, 1 )
	MOAIDraw.setClearDepth ( 0 )
	MOAIDraw.clear ()

	MOAIDraw.setViewProj ( viewport )

	--prop1:draw ()
	--prop2:draw ()

	MOAIDraw.setBlendMode ( MOAIGraphicsProp.BLEND_NORMAL )

	MOAIDraw.setPenColor ( 1, 0, 0, 1 )
	MOAIDraw.setMatrix ( MOAIDraw.WORLD_MATRIX, prop1 )

	MOAIDraw.fillRect ( -128, -128, 128, 128 )

	--gfxQuad1:draw ()

	--MOAIDraw.setPenColor ( 1, 1, 1, 0 )
	--MOAIDraw.setMatrix ( MOAIDraw.WORLD_MATRIX, prop2 )
	--gfxQuad2:draw ()

	--layer:draw ()
end

MOAIRenderMgr.setRenderCallback ({ onRender })

