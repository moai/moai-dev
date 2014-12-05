----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- Issue #745
-- ParticleForce attractor bug: only particles inside attraction radius should be affected by force
-- 

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

local radius = 128

local init = MOAIParticleScript.new ()

local render = MOAIParticleScript.new ()
render:sprite			()


texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead_small.png" )
texture:setRect ( -8, -8, 8, 8 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 0 )
system:reserveSprites ( 256 )
system:reserveStates ( 1 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

magnet = MOAIParticleForce.new ()
magnet:initAttractor ( radius, 256 )
magnet:setType ( MOAIParticleForce.FORCE )

-- state 1
state1 = MOAIParticleState.new ()
state1:setTerm ( 10000 )
state1:setMass ( 1, 4 )
state1:setInitScript ( init )
state1:setRenderScript ( render )
system:setState ( 1, state1 )

state1:pushForce ( magnet )

emitter = MOAIParticleTimedEmitter.new ()
emitter:setLoc ( 0, 0 )
emitter:setSystem ( system )
emitter:setEmission ( 0 )
emitter:setMagnitude ( 0 )
emitter:setRect ( -320, -240, 320, 240 )
emitter:surge ( 256 )

emitter:start ()


function onDraw ( index, xOff, yOff, xFlip, yFlip )

    MOAIGfxDevice.setPenColor ( 1, 0, 0, 1 )
    MOAIDraw.drawCircle ( 0, 0, radius, 32 )
end

scriptDeck = MOAIScriptDeck.new ()
scriptDeck:setRect ( -radius, -radius, radius, radius )
scriptDeck:setDrawCallback ( onDraw )

prop = MOAIProp2D.new ()
prop:setDeck ( scriptDeck )
layer:insertProp ( prop )