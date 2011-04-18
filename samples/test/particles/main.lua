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

-- the particle
local loc		= 0
local rot		= 2
local dxdy		= 3

-- extras
local r0		= 5
local r1		= 6
local r2		= 7
local r3		= 8
local r4		= 9
local r5		= 10

----------------------------------------------------------------
-- state 1
local init = MOAIParticleScript.new ()

	init:rand				( r0, 3, 5 )
	init:setDuration		( r0 )

	init:rand				( rot, 0, 360 )
	init:rand				( r0, 0.5, 1 )			
	init:scale				( dxdy, r0, 2 )
	init:load				( r0, 1, 1.5 )
	init:mul				( dxdy + 1, r0, 1 )

local update = MOAIParticleScript.new ()

	update:load				( r0, 0, 0 )
	update:accForces		( r0, loc )
	update:stepEuler		( loc, dxdy, r0 )
	
	update:load				( r0, 0.5, 0.5 )
	update:mul				( dxdy, r0, 2 )

----------------------------------------------------------------
-- state 2
local init2 = MOAIParticleScript.new ()

	init2:rand				( r0, 3, 5 )
	init2:setDuration		( r0 )

local update2 = MOAIParticleScript.new ()

	update2:load			( r0, 0, 0 )
	update2:accForces		( r0, loc )
	update2:stepEuler		( loc, dxdy, r0 )

----------------------------------------------------------------
-- render
local render = MOAIParticleScript.new ()

	render:loadTime			( r0 )
	render:load				( r1, 1, 1, 1, 1 )
	render:sub				( r4, r0 )

	render:sprite			()
	render:load				( r0, 1 )
	render:spriteGlow		( r0 )
	render:spriteLoc		( loc )
	render:spriteRot		( rot )
	render:spriteColor		( r1 )

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 5 )
system:reserveSprites ( 256 )
system:reserveStates ( 2 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

-- state 1
state = MOAIParticleState.new ()
state:init ( init, update, render )
system:setState ( 1, state )

force = MOAIParticleForce.new ()
force:initLinear ( 0, -2000 )
state:pushForce ( force )

force = MOAIParticleForce.new ()
force:initAttractor ( 128, -64000 )
state:pushForce ( force )

-- state 2
state = MOAIParticleState.new ()
state:init ( init2, update2, render )
system:setState ( 2, state )

force = MOAIParticleForce.new ()
force:initLinear ( 0, -80 )
state:pushForce ( force )

-- set the transitions
state1 = system:getState ( 1 )
state2 = system:getState ( 2 )
state1:setNext ( state2 )

emitter = MOAIParticleEmitter.new ()
emitter:setLoc ( 0, 0 )
emitter:setSystem ( system )
emitter:setMagnitude ( 96, 128 )
emitter:setFrequency ( 10, 20 )
emitter:setRadius ( 8, 8 )
emitter:surge ( 128 )
--emitter:start ()
