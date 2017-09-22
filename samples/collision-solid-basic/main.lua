----------------------------------------------------------------
-- Copyright (c) 2010-2017 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

--[[

arcade style collision proof of concept. motivation is to provide an alternative
to box2d for arcade style games (top down scrollers and platformers). in particular,
platformers can benefit from a tighter integration of physics and terrain. will
also allow instancing of collision objects through tilemaps.

]]--

MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_PROP_BOUNDS, 2, 0, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_OVERLAP_PROP_BOUNDS, 2, 1, 1, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_ACTIVE_TOUCHED_PROP_BOUNDS, 2, 1, 0, 0 )

MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_NORMAL, 1, 0, 1, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_CORNER_EDGE_NORMAL, 1, 1, 0, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_CORNER_TANGENT, 1, 0.85, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_CORNER, 1, 1, 1, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_CROSSING, 1, 1, 0, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_LEAVING, 1, 1, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_CONTACT_POINT_PARALLEL, 1, 0, 1, 1 )

MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_MOVE_RETICLE, 1, 1, 1, 0 )

MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_OVERLAP_PROP_BOUNDS, 2, 0, 1, 0 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_OVERLAPS, 4, 1, 0, 1 )
MOAIDebugLinesMgr.setStyle ( MOAICollisionProp.DEBUG_DRAW_COLLISION_WORLD_BOUNDS, 1, 0.5, 0.5, 0.5 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

onOverlap = function ( event, prop1, prop2 )
	
	local eventName
	
	eventName = ( event == MOAICollisionWorld.OVERLAP_BEGIN ) and 'BEGIN' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_END ) and 'END' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_UPDATE ) and 'UPDATE' or eventName
	
	print ( eventName, prop1, prop2 )
	
	if event == MOAICollisionWorld.OVERLAP_END then print () end
end

camera = MOAICamera.new ()

world = MOAICollisionWorld.new ()
world:setCallback ( onOverlap )
world:start ()

debugLayer = MOAIPartitionViewLayer.new ()
debugLayer:setViewport ( viewport )
debugLayer:setLayerPartition ( world )
debugLayer:setCamera ( camera )
debugLayer:pushRenderPass ()

collDeck = MOAICollisionDeck.new ()
collDeck:reserveShapes ( 1 )
collDeck:setQuad ( 1,
	-64, -64,
	64, -64,
	64, 64,
	-64, 64
)
--[[
collDeck:setQuad ( 1,
	0, 64,
	-64, 0,
	0, -64,
	64, 0
)
]]--

makePropWithColl = function ()

	local coll = MOAICollisionProp.new ()
	coll:setParent ( prop )
	coll:setDeck ( collDeck )
	coll:setOverlapFlags ( 0 )
	--coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_ON_UPDATE + MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE )
	--coll:setOverlapFlags ( MOAICollisionProp.OVERLAP_EVENTS_LIFECYCLE + MOAICollisionProp.OVERLAP_CALCULATE_BOUNDS )
	--coll:setGroupMask ( group or MOAICollisionProp.CATEGORY_MASK_ALL )
	coll:setPartition ( world )

	return coll
end

local coll2 = makePropWithColl ()
coll2:setLoc ( 0, 0, 0 )
coll2:setRot ( 0, 0, 45 )

local coll3 = makePropWithColl ()
coll3:setLoc ( -128, 0, 0 )
coll3:setRot ( 0, 0, 45 )

local coll4 = makePropWithColl ()
coll4:setLoc ( 128, 0, 0 )
coll4:setRot ( 0, 0, 45 )

local coll1 = makePropWithColl ()
coll1:setLoc ( 0, 160, 0 )
--coll1:setLoc ( -64, 0, 0 )
--coll1:setLoc ( 32, 128, 0 )
coll1:setScl ( 0.5, 0.5, 1 )

--camera:setParent ( coll1 )
camera:setAttrLink ( MOAITransform.INHERIT_LOC, coll1, MOAITransform.TRANSFORM_TRAIT )

main = function ()

	local keyboard = MOAIInputMgr.device.keyboard

	local ACC = 6
	local DAMP = 0
	--local ACC = 0.15
	--local DAMP = 0.97
	local xd = 0
	local yd = 0
	while true do

		if keyboard:keyIsDown ( MOAIKeyCode.SPACE ) then
			xd = 2
			yd = 2
			DAMP = 1
		end

		if keyboard:keyIsDown ( MOAIKeyCode.RIGHT ) then
			xd = xd + ACC
		end

		if keyboard:keyIsDown ( MOAIKeyCode.LEFT ) then
			xd = xd - ACC
		end

		if keyboard:keyIsDown ( MOAIKeyCode.UP ) then
			yd = yd + ACC
		end

		if keyboard:keyIsDown ( MOAIKeyCode.DOWN ) then
			yd = yd - ACC
		end

		coll1:collisionMove ( xd, yd, 0, MOAICollisionProp.SURFACE_MOVE_SLIDE )
		--coll1:collisionMove ( 1, 0, 0 )

		xd = xd * DAMP
		yd = yd * DAMP

		xd = math.abs ( xd ) >= 0.01 and xd or 0
		yd = math.abs ( yd ) >= 0.01 and yd or 0

		local rd = 0

		if keyboard:keyIsDown ( 'p' ) then
			rd = rd - 1
		end

		if keyboard:keyIsDown ( 'o' ) then
			rd = rd + 1
		end

		coll2:moveRot ( 0, 0, rd )

		local s = 0.0

		if keyboard:keyIsDown ( 'a' ) then
			s = s - 0.01
		end

		if keyboard:keyIsDown ( 's' ) then
			s = s + 0.01
		end

		coll2:moveScl ( s, s, 0 )

		coroutine.yield ()
	end
end

thread = MOAICoroutine.new ()
thread:run ( main )
