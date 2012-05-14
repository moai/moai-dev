----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

viewport = MOAIViewport.new ()
viewport:setSize ( 320, 480 )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 320, 480 )

CONST = MOAIParticleScript.packConst

local PARTICLE_X1 = MOAIParticleScript.packReg ( 1 )
local PARTICLE_Y1 = MOAIParticleScript.packReg ( 2 )
local PARTICLE_R0 = MOAIParticleScript.packReg ( 3 )
local PARTICLE_R1 = MOAIParticleScript.packReg ( 4 )
local PARTICLE_S0 = MOAIParticleScript.packReg ( 5 )
local PARTICLE_S1 = MOAIParticleScript.packReg ( 6 )

----------------------------------------------------------------
local init = MOAIParticleScript.new ()
init:randVec	( PARTICLE_X1, PARTICLE_Y1, CONST ( 128 ), CONST ( 256 ))
init:rand		( PARTICLE_R0, CONST ( -180 ), CONST ( 180 ))
init:rand		( PARTICLE_R1, CONST ( -180 ), CONST ( 180 ))
init:set		( PARTICLE_S0, CONST ( 0 ))
init:set		( PARTICLE_S1, CONST ( 4 ))

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 6 )
system:reserveSprites ( 256 )
system:reserveStates ( 1 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

-- state 1
state1 = MOAIParticleState.new ()
state1:setTerm ( .75, 1.25 )
state1:setInitScript ( init )
if ParticlePresets then
	state1:setPlugin ( ParticlePresets.test )
else
	print ( 'Missing ParticlePresets table. Use AKUSetParticlePreset to register a C function as a particle handler.' )
end
system:setState ( 1, state1 )

system:surge ( 128 )
