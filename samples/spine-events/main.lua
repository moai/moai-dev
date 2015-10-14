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

local spineboyData = MOAISpineSkeletonData.new ()
spineboyData:load("spineboy/spineboy.json", "spineboy/spineboy.atlas", 1)

local spineboy = MOAISpineSkeleton.new ()
spineboy:init(spineboyData)
spineboy:setFlip(false, true)
spineboy:setToSetupPose()
spineboy:initAnimationState()
spineboy:setLoc(0, 100, 0)

spineboy:moveRot(0, 0, 360, 4)

layer:insertProp(spineboy)


spineboy:setListener(MOAISpineSkeleton.EVENT_ANIMATION_START, function(self, trackIndex)
    print("AnimationStart", self, trackIndex)
end)

spineboy:setListener(MOAISpineSkeleton.EVENT_ANIMATION_END, function(self, trackIndex)
    print("AnimationStop", self, trackIndex)
end)

spineboy:setListener(MOAISpineSkeleton.EVENT_ANIMATION_COMPLETE, function(self, trackIndex, loopCount)
    print("AnimationComplete", self, trackIndex, loopCount)
end)

spineboy:setListener(MOAISpineSkeleton.EVENT_ANIMATION_EVENT, function(self, trackIndex, eventName, intVal, floatVal, strVal)
    print("Event", self, trackIndex, "int " .. tostring(intVal), "float " .. tostring(floatVal), "str " .. strVal)
end)

spineboy:setAnimation (1, "walk", true)
spineboy:start()