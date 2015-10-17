----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 640 )

-- MOAIDebugLines.setStyle ( MOAIDebugLines.PROP_WORLD_BOUNDS )

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 640 )
viewport:setScale ( 480, -640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local goblinsData = MOAISpineSkeletonData.new ()
goblinsData:load ( "goblins/goblins-mesh.json", "goblins/goblins-mesh.atlas", 1 )

function makeGoblin ( x, y )
    local goblins = MOAISpineSkeleton.new ()
    goblins:init ( goblinsData )
    goblins:setFlip ( false, true )
    goblins:setToSetupPose ()
    goblins:setSkin ( "goblin" )
    goblins:setComputeBounds ( true )
    goblins:initAnimationState ()
    goblins:setAnimation (1, "walk", true)
    goblins:start ()
    goblins:setLoc ( x, y, 0 )
    layer:insertProp ( goblins )
end

timer = MOAITimer.new ()
timer:setSpan ( 2 )
timer:setListener ( MOAITimer.EVENT_TIMER_END_SPAN, function()
    print("draw calls:", MOAIRenderMgr.getPerformanceDrawCount ())
end)
timer:start ():setMode ( MOAITimer.LOOP )


makeGoblin ( -100, -40 )
makeGoblin ( -100, 300 )
makeGoblin ( 100, -40 )
makeGoblin ( 100, 300 )


-- stress test:

-- local count = 30
-- for i = 1, count do
--     makeGoblin ( math.random( -240, 240 ), math.random ( -320, 320 ))
-- end
