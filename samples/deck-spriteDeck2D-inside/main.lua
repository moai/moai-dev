----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

gfxQuad = MOAISpriteDeck2D.new ()
gfxQuad:setTexture ( '../resources/moai.png' )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )
gfxQuad:setHitMaskImage ( '../resources/moai.png' )
gfxQuad:setHitMaskScalar ( 1, 1, 1, 1 )
gfxQuad:setHitMaskThreshold ( 1, 1, 1, 1 ) -- just hit test the white pixels

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setPartition ( layer )

function clickCallback ( down )
	
	if down then
		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = layer:wndToWorld ( x, y )
		if prop:pickByPoint ( x, y ) then
			prop:addRot ( 0, 0, 5 )
		end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
