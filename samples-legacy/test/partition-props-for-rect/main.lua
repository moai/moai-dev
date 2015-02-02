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
partition:reserveLevels ( 4 )
partition:setLevel ( 1, 256, 4, 4 )
partition:setLevel ( 2, 128, 8, 8 )
partition:setLevel ( 3, 96, 12, 12 )
partition:setLevel ( 4, 64, 16, 16 )

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

prop2 = MOAIProp2D.new ()
prop2:setDeck ( gfxQuad )
layer:insertProp ( prop2 )

function onKeyboardEvent ( key, down )
	if down == true then
		if key == MOAIKeyCode.W then -- move up
			prop:moveLoc ( 0, 12 )
			print ( prop:getLoc ())
		elseif key == MOAIKeyCode.S then -- move down
			prop:moveLoc ( 0, -12 )
			print ( prop:getLoc ())
		elseif key == MOAIKeyCode.A then -- move left
			prop:moveLoc ( -12, 0 )
			print ( prop:getLoc ())
		elseif key == MOAIKeyCode.D then -- move right
			prop:moveLoc ( 12, 0 )
			print ( prop:getLoc ())
		elseif key == MOAIKeyCode.Q then -- rotate left
			prop:moveRot ( 15 )
			print ( prop:getRot ())
		elseif key == MOAIKeyCode.E then -- rotate right
			prop:moveRot ( -15 )
			print ( prop:getRot ())
		elseif key == MOAIKeyCode.R then -- scale up
			prop:moveScl ( .2, .2 )
			print ( prop:getScl ())
		elseif key == MOAIKeyCode.F then -- scale down
			prop:moveScl ( -.2, -.2 )
			print ( prop:getScl ())
		end
	end
end
 
local x2 = 0
local y2 = 0
 
function onPointerEvent ( x, y )

	x2, y2 = layer:wndToWorld ( x, y )
	prop2:setLoc ( x2, y2 )
end

function onMouseLeftEvent ( down )

	if down == true then
		print ( partition:propListForRect ( x2 - 64, y2 - 64, x2 + 64, y2 + 64 ))
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( onMouseLeftEvent )
MOAIInputMgr.device.pointer:setCallback ( onPointerEvent )
MOAIInputMgr.device.keyboard:setKeyCallback ( onKeyboardEvent )

prop:setLoc ( 0, -200 )
prop:setScl ( 1.4, 1.4 )
prop:setRot ( -45 )
