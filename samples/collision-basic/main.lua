----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS, 2, 0, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS, 2, 1, 0, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS, 2, 0, 1, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

onOverlap = function ( event, prop1, prop2 )
	
	local eventName
	
	eventName = ( event == MOAICollisionWorld.OVERLAP_BEGIN ) and 'BEGIN' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_END ) and 'END' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_UPDATE ) and 'UPDATE' or eventName
	
	print ( eventName, prop1, prop2 )
	
	if event == MOAICollisionWorld.OVERLAP_END then print () end
end

world = MOAICollisionWorld.new ()
world:setCallback ( onOverlap )
world:start ()

layer = MOAILayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

debugLayer = MOAILayer.new ()
debugLayer:setViewport ( viewport )
debugLayer:usePartition ( world )
debugLayer:pushRenderPass ()

spriteDeck = MOAISpriteDeck2D.new ()
spriteDeck:setTexture ( '../resources/moai.png' )
spriteDeck:setRect ( -64, -64, 64, 64 )
spriteDeck:setUVRect ( 0, 0, 1, 1 )

--font = MOAIFont.new ()
--font:loadFromTTF ( '../resources/arial-rounded.TTF' )

collDeck = MOAICollisionDeck.new ()
collDeck:reserveShapes ( 1 )
collDeck:setQuad ( 1,
	0, 64,
	-64, 0,
	0, -64,
	64, 0
)

ready = function ( prop, x, y, group )

	local coll = MOAICollisionProp.new ()
	coll:setParent ( prop )
	coll:setDeck ( collDeck )
	--coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_ON_UPDATE + MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE )
	coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE )
	coll:setGroupMask ( group or MOAICollisionProp.GROUP_MASK_ALL )
	coll:setPartition ( world )
	
	return prop, coll
end

makePropWithColl = function ( x, y, group )

	local prop = MOAIGraphicsProp.new ()
	prop:setDeck ( spriteDeck )
	prop:setPartition ( layer )
	prop:setLoc ( x, y )
	return ready ( prop, x, y, group )
end

--[[
makeTextWithColl = function ( text, size, x, y, group )

	local label = MOAITextLabel.new ()
	label:setString ( text )
	label:setFont ( font )
	label:setTextSize ( size )
	label:setAlignment ( MOAITextBox.CENTER_JUSTIFY, MOAITextBox.CENTER_JUSTIFY )
	layer:insertProp ( label )

	return ready ( label, x, y, group )
end
]]--

local prop1, coll1 = makePropWithColl ( -70, 30 )
local prop2, coll2 = makePropWithColl ( 70, -30 )

--collDeck:setBox ( 1, -64, -64, 0, 64, 64, 0 )

--coll1:setBounds ( prop1:getBounds ())
--coll1:setDeck ( collDeck )
--coll2:setBounds ( prop1:getBounds ())

--local prop1 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, -70, 0, 1 )
--local prop2 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, 70, 0, 1 )
--local prop3 = makeTextWithColl ( 'TEXTY TEXT\nTEXT\nTEXTERSON', 22, 0, 0, 2 )

--prop1:moveRot ( 0, 0, 360, 5 )
--prop2:moveRot ( 0, 0, -360, 5 )

prop1:moveLoc ( 35, 0, 0, 5 )
prop1:moveRot ( 0, 0, 45, 5 )

prop2:moveLoc ( -35, 0, 0, 5 )
prop2:moveRot ( 0, 0, 45, 5 )

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
