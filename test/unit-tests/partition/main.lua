----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

workingDir = MOAIFileSystem.getWorkingDirectory ()
MOAIFileSystem.setWorkingDirectory ( ".." )
require "testhelpers"
MOAIFileSystem.setWorkingDirectory ( workingDir )
MOAISim.openWindow ( "test", 800, 800 )

MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_CELLS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PARTITION_PADDED_CELLS, 1, 0.5, 0.5, 0.5 )
MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS, 2, 0.75, 0.75, 0.75 )

viewport = MOAIViewport.new ()
viewport:setSize ( 800, 800 )
viewport:setScale ( 800, 800 )

partition = MOAIPartition.new ()
partition:reserveLayers ( 3 )
partition:setLayer ( 1, 256, 4, 4 )
partition:setLayer ( 2, 128, 8, 8 )
partition:setLayer ( 3, 96, 12, 12 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setPartition ( partition )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )

function onKeyboardEvent ( key, down )
	if down == true then
		-- print ( key )
		elseif key == 119 and not running then -- move up
			prop:moveLoc ( 0, 12 )
		elseif key == 115 and not running then -- move down
			prop:moveLoc ( 0, -12 )
		elseif key == 97 and not running then -- move left
			prop:moveLoc ( -12, 0 )
		elseif key == 100 and not running then -- move right
			prop:moveLoc ( 12, 0 )
		elseif key == 113 and not running then -- rotate left
			prop:moveRot ( -12 )
		elseif key == 101 and not running then -- rotate right
			prop:moveRot ( 12 )
		elseif key == 114 and not running then -- scale up
			prop:moveScl ( .2 )
		elseif key == 102 and not running then -- scale down
			prop:moveScl ( -.2 )
		-- end
	end
end

MOAIInputMgr.device.keyboard:setCallback ( onKeyboardEvent )

-- prop:setLoc ( -64, 64 )
-- prop:moveLoc ( 128, -128, 3 )
-- prop:moveScl ( -0.5, -0.5, 3 )
