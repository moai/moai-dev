----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAIDebugLines.setStyle ( MOAIDebugLines.COLLISION_ACTIVE_PROP_BOUNDS, 2, 1, 1, 1 )

MOAISim.openWindow ( "test", 320, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, -480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

onOverlap = function ( event, prop1, prop2 )
	
	local eventName
	
	eventName = ( event == MOAICollisionWorld.OVERLAP_BEGIN ) and 'BEGIN' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_END ) and 'END' or eventName
	eventName = ( event == MOAICollisionWorld.OVERLAP_UPDATE ) and 'UPDATE' or eventName
	
	print ( eventName, prop1, prop2 )
	
	if event == MOAICollisionWorld.OVERLAP_END then print () end
end

world = MOAICollisionWorld.new ()
world:start ()
world:setCallback ( onOverlap )

layer:setPartition ( world )
layer:setOverlayTable ({ world })

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 0, 0, 1, 1 )

makePropWithColl = function ( x, y )

	local prop = MOAIGraphicsProp.new ()
	prop:setDeck ( gfxQuad )
	layer:insertProp ( prop )
	prop:setLoc ( x, y )
	world:insertProp ( prop )

	local coll = MOAICollisionFacet.new ()
	coll:setOverlapFlags ( MOAICollisionFacet.OVERLAP_EVENTS_ON_UPDATE + MOAICollisionFacet.OVERLAP_EVENTS_LIFECYCLE )
	prop:setFacet ( MOAIProp.COLLISION_FACET, coll )
	
	return prop
end

local prop1 = makePropWithColl ( -70, 0 )
local prop2 = makePropWithColl ( 70, 0 )

prop1:moveRot ( 0, 0, 360, 5 )
prop2:moveRot ( 0, 0, -360, 5 )
