----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 800, 800 )

MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_CELLS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

viewport = MOAIViewport.new ()
viewport:setSize ( 800, 800 )
viewport:setScale ( 800, 800 )

partition = MOAIPartition.new ()
partition:reserveLevels ( 3 )
partition:setLevel ( 1, 256, 4, 4 )
partition:setLevel ( 2, 128, 8, 8 )
partition:setLevel ( 3, 96, 12, 12 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setPartition ( partition )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

function onKeyboardEvent ( key, down )
	if down == true then
		-- print ( key )
		if key == 119 then -- move up
			prop:moveLoc ( 0, 12 )
		elseif key == 115 then -- move down
			prop:moveLoc ( 0, -12 )
		elseif key == 97 then -- move left
			prop:moveLoc ( -12, 0 )
		elseif key == 100 then -- move right
			prop:moveLoc ( 12, 0 )
		elseif key == 113 then -- rotate left
			prop:moveRot ( -12 )
		elseif key == 101 then -- rotate right
			prop:moveRot ( 12 )
		elseif key == 114 then -- scale up
			prop:moveScl ( .2, .2 )
		elseif key == 102 then -- scale down
			prop:moveScl ( -.2, -.2 )
		end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

prop:setRot ( -360 )
