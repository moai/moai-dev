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

viewport = MOAIViewport.new ()
viewport:setSize ( 480, 640 )
viewport:setScale ( 480, -640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local goblinsData = MOAISpineSkeletonData.new ()
goblinsData:load("goblins/goblins.json", "goblins/goblins.atlas", 1)

local goblins = MOAISpineSkeleton.new ()
goblins:init(goblinsData)
goblins:setFlip(false, true)
goblins:setToSetupPose()
goblins:setSkin("goblin")
goblins:initAnimationState()
goblins:setAnimation (1, "walk", true)

goblins:start()
goblins:setLoc(0, 100, 0)

goblins:moveRot(0, 0, 360, 4)

layer:insertProp(goblins)

local skins = {"goblin", "goblingirl"}
local counter = 1
MOAIInputMgr.device.mouseLeft:setCallback(function(val)
    if val then
        goblins:setSkin(skins[1 + counter % 2])
        print(goblins:setAttachment("left hand item", math.random() < 0.5 and "dagger" or "spear"))
        counter = counter + 1
    end
end)