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

texture = MOAIImageTexture.new ()
texture:init ( 16, 16 )

for i = 0,15 do
	texture:setRGBA ( i, i, 1, 0, 0, 1 )
end

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( texture )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

prop:moveRot ( 360, 1.5 )

function onMouseLeftEvent ( down )
	if down == true then
	
		for i = 0,15 do
			texture:setRGBA ( i, i, 1, 1, 1, 1 )
		end
	
		texture:invalidate ()
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )

function onMouseRightEvent ( down )
	if down == true then
	
		for i = 0,15 do
			texture:setRGBA ( i, i, 0, 0, 1, 1 )
		end
	
		texture:invalidate ( 4, 4, 12, 12 )
	end
end

MOAIInputMgr.device.mouseRight:setCallback ( onMouseRightEvent )