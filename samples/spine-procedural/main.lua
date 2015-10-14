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

local spineboyData = MOAISpineSkeletonData.new ()
spineboyData:load("spineboy/spineboy.json", "spineboy/spineboy.atlas", 1)

local spineboy = MOAISpineSkeleton.new ()
spineboy:init(spineboyData)
spineboy:setFlip(false, true)
spineboy:setToSetupPose()
spineboy:initAnimationState()
spineboy:setAnimation (1, "walk", true)

spineboy:start()
spineboy:setLoc(-100, 100, 0)

local spineboy2 = MOAISpineSkeleton.new ()
spineboy2:init(spineboyData)
spineboy2:setFlip(false, true)
spineboy2:setToSetupPose()
spineboy2:initAnimationState()
spineboy2:setAnimation (1, "walk", true)

spineboy2:start()
spineboy2:setLoc(100, 100, 0)

-- slow-motion
spineboy2:throttle(0.5)

layer:insertProp(spineboy)
layer:insertProp(spineboy2)

--======================================================================--
-- Controling bone transforms with Moai animation system
--======================================================================--
local right_upper_leg = spineboy:getBone("right upper leg")
local right_lower_leg = spineboy:getBone("right lower leg")
local right_foot = spineboy:getBone("right foot")

right_upper_leg:lockTransform(MOAISpineBone.LOCK_ROT)
right_lower_leg:lockTransform(MOAISpineBone.LOCK_ROT)
right_foot:lockTransform(MOAISpineBone.LOCK_ROT)

local c = MOAICoroutine.new()
c:run (function()
    MOAICoroutine.blockOnAction(right_lower_leg:moveRot(0, 0, 40, 1))
    while (true) do
        MOAICoroutine.blockOnAction(right_lower_leg:moveRot(0, 0, -80, 2))
        MOAICoroutine.blockOnAction(right_lower_leg:moveRot(0, 0, 80, 2))
    end
end)


--======================================================================--
-- Changing slot colors
--======================================================================--
local head_slot = spineboy:getSlot("head")
local right_lower_leg_slot = spineboy:getSlot("right lower leg")
head_slot:lockColor(true)
head_slot:seekColor(1, 0, 0, 1, 4)

-- 0.5 alpha; moai uses premultiplied alpha 
-- so we should multiply every color component by alpha value
right_lower_leg_slot:setColor(0.5, 0.5, 0.5, 0.5)


--======================================================================--
-- Attaching images to skeleton bones
--======================================================================--
local circleDeck = MOAIGfxQuad2D.new()
circleDeck:setTexture ( "circle.png" )
circleDeck:setRect ( -16, -16, 16, 16 )
circleDeck:setUVRect ( 0, 0, 1, 1 )

function attachCircle(parent)
    local circle = MOAIProp.new()
    circle:setDeck(circleDeck)
    circle:setAttrLink(MOAITransform.INHERIT_LOC, parent, MOAITransform.TRANSFORM_TRAIT)

    layer:insertProp(circle)
end

-- all bones of spineboy skeleton
boneNames = {"root", "hip", "pelvis", "torso", "neck", "head",
"left upper leg", "left lower leg", "left foot", 
"right upper leg", "right lower leg", "right foot", 
"left shoulder", "left arm", "left hand", 
"right shoulder", "right arm", "right hand",
}

for k, v in ipairs(boneNames) do
    local bone = spineboy2:getBone(v)
    attachCircle(bone)
end

-- local root = spineboy2:getBone("root")
-- root:lockTransform(MOAISpineBone.LOCK_SCL)
-- root:moveRot(0, 0, 360, 4)



