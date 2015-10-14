----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.openWindow ( "test", 480, 640 )

MOAISim.clearLoopFlags()

MOAISim.setLoopFlags(MOAISim.SIM_LOOP_ALLOW_BOOST)
MOAISim.setLoopFlags(MOAISim.SIM_LOOP_LONG_DELAY)
MOAISim.setBoostThreshold(0)

MOAIDebugLines.setStyle(MOAIDebugLines.PROP_WORLD_BOUNDS)

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 640 )
viewport:setScale ( 480, -640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local goblinsData = MOAISpineSkeletonData.new ()
goblinsData:load("goblins/goblins-ffd.json", "goblins/goblins-ffd.atlas", 1)

local goblins = MOAISpineSkeleton.new ()
goblins:init(goblinsData)
goblins:setFlip(false, true)
goblins:setToSetupPose()
goblins:setSkin("goblin")
goblins:initAnimationState()
goblins:setAnimation (1, "walk", true)

-- By default MOAISpineSkeleton uses cached bounds for better performance. 
-- Bounds are recomputed: 
-- 1) when called setToSetupPose, setBonesToSetupPose, setSlotsToSetupPose or setSkin
-- 2) if setComputeBounds is true, bounds will be updated every frame
goblins:setComputeBounds (true)

goblins:start()
goblins:setLoc(0, 100, 0)

goblins:moveRot(0, 0, 360, 4)

layer:insertProp(goblins)
