----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_ACTIVE_PROP_BOUNDS, 2, 0, 0, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS, 2, 1, 0, 0 )
MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_OVERLAP_PROP_BOUNDS, 2, 0, 1, 0 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

partition = MOAIPartition.new ()

onOverlap = function ( event, prop1, prop2 )
	
	local eventName
	
	eventName = ( event == MOAICollisionWorld.OVERLAP_BEGIN ) and 'BEGIN' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_END ) and 'END' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_UPDATE ) and 'UPDATE' or eventName
	
	print ( eventName, prop1, prop2 )
	
	if event == MOAICollisionWorld.OVERLAP_END then print () end
end

world = MOAICollisionWorld.new ()
world:setPartition ( partition )
world:setCallback ( onOverlap )
world:start ()

layer = MOAILayer2D.new ()
layer:setPartition ( partition )
layer:setViewport ( viewport )
layer:setOverlayTable ({ world })

MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

font = MOAIFont.new ()
font:loadFromTTF ( 'arial-rounded.TTF' )

ready = function ( prop, x, y, group )

	layer:insertProp ( prop )
	prop:setLoc ( x, y )
	
	local coll = MOAICollisionProp.new ()
	coll:setBounds ( prop:getBounds ())
	coll:setParent ( prop )
	--coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_ON_UPDATE + MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE )
	coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE )
	coll:setGroupMask ( group or MOAICollisionProp.GROUP_MASK_ALL )
	
	world:insertProp ( coll )
	
	return prop, coll
end

makePropWithColl = function ( x, y, group )

	local prop = MOAIGraphicsProp.new ()
	prop:setDeck ( gfxQuad )
	return ready ( prop, x, y, group )
end

makeTextWithColl = function ( text, size, x, y, group )

	local label = MOAITextLabel.new ()
	label:setString ( text )
	label:setFont ( font )
	label:setTextSize ( size )
	label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
	layer:insertProp ( label )

	return ready ( label, x, y, group )
end

--local prop1 = makePropWithColl ( -70, 30 )
--local prop2 = makePropWithColl ( 70, -30 )

local prop1 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, -70, 0, 1 )
local prop2 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, 70, 0, 1 )
--local prop3 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, 0, 0, 2 )

--prop1:moveRot ( 0, 0, 360, 5 )
--prop2:moveRot ( 0, 0, -360, 5 )

prop1:moveLoc ( 35, 0, 0, 5 )
prop2:moveLoc ( -35, 0, 0, 5 )

--[[
main = function ()
	
	while true do
		
		if prop1.coll:hasOverlaps () then
			print ( prop1, 'overlaps:' )
			print ( prop1.coll:getOverlaps ())
		end
		
		if prop2.coll:hasOverlaps () then
			print ( prop2, 'overlaps:' )
			print ( prop2.coll:getOverlaps ())
		end
		
		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
]]--
