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

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

gfxQuad:setHitMask ( "moai.png" )
gfxQuad:setHitMaskScalar ( 1, 1, 1, 1 )
gfxQuad:setHitMaskThreshold ( 1, 1, 1, 1 )
gfxQuad:setHitGranularity ( MOAIGfxQuad2D.HIT_TEST_FINE )

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

function clickCallback ( down )
	
	if down then
		
		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = layer:wndToWorld ( x, y )
		if prop:inside ( x, y ) then
			prop:addRot ( 0, 0, 5 )
		end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
