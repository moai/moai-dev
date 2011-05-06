----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

----------------------------------------------------------------

local init = MOAIParticleScript.new ()
init:randVecConst		( 1, 2, 64, 128 )

local render = MOAIParticleScript.new ()
render:easeVarDelta		( MOAIParticleScript.PARTICLE_X, 0, 1, MOAIEaseType.EASE_IN, 0, 1 )
render:easeVarDelta		( MOAIParticleScript.PARTICLE_Y, 0, 2, MOAIEaseType.EASE_IN, 0, 1 )

render:sprite			()
render:setConst			( MOAIParticleScript.SPRITE_RED, 0 )
render:randConst		( MOAIParticleScript.SPRITE_GREEN, 0.5, 1 )
render:randConst		( MOAIParticleScript.SPRITE_BLUE, 0.5, 1 )

----------------------------------------------------------------

local init2 = MOAIParticleScript.new ()
init2:setConst			( 1, 0 )
init2:randConst			( 2, -360, 360 )

local render2 = MOAIParticleScript.new ()

render2:sprite			()
render2:easeVar			( MOAIParticleScript.SPRITE_ROT, 1, 2, MOAIEaseType.EASE_IN, 0, 1 )
render2:randConst		( MOAIParticleScript.SPRITE_RED, 0.5, 1 )
render2:randConst		( MOAIParticleScript.SPRITE_GREEN, 0.5, 1 )
render2:setConst		( MOAIParticleScript.SPRITE_BLUE, 0 )

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 2 )
system:reserveSprites ( 256 )
system:reserveStates ( 2 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

magnet = MOAIParticleForce.new ()
magnet:initBasin ( 256, 64 )
magnet:setType ( MOAIParticleForce.OFFSET )

-- state 1
state1 = MOAIParticleState.new ()
state1:setTerm ( .75, 1.25 )
state1:setInitScript ( init )
state1:setRenderScript ( render )
system:setState ( 1, state1 )

state1:pushForce ( magnet )

-- state 2
state2 = MOAIParticleState.new ()
state2:setTerm ( 0.5 )
state2:setInitScript ( init2 )
state2:setRenderScript ( render2 )
system:setState ( 2, state2 )

state2:pushForce ( magnet )

state1:setNext ( state2 )
state2:setNext ( state1 )

--[[
emitter = MOAIParticleEmitter.new ()
emitter:setLoc ( 0, 256 )
emitter:setSystem ( system )
emitter:setMagnitude ( 500, 1000 )
emitter:setFrequency ( 10, 20 )
emitter:setRadius ( 8, 8 )
emitter:surge ( 128 )
--emitter:start ()
]]--
system:surge ( 128 )
