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

particleName = 'deathBlossomCharge.pex'

local plugin =  MOAIParticlePexPlugin.load( particleName )
local maxParticles = plugin:getMaxParticles ()
local blendsrc, blenddst = plugin:getBlendMode ()
local minLifespan, maxLifespan = plugin:getLifespan ()
local duration = plugin:getDuration ()

system = MOAIParticleSystem.new ()
system._duration = duration
system._lifespan = maxLifespan
system:reserveParticles ( maxParticles , plugin:getSize() )
system:reserveSprites ( maxParticles )
system:reserveStates ( 1 )
system:setBlendMode ( blendsrc, blenddst )

local state = MOAIParticleState.new ()
state:setTerm ( minLifespan, maxLifespan )
state:setPlugin(  plugin  )
system:setState ( 1, state )

emitter = MOAIParticleTimedEmitter.new()
emitter:setSystem ( system )
emitter:setEmission ( plugin:getEmission () )
emitter:setFrequency ( plugin:getFrequency () )
emitter:setRect ( -1, -1, 1, 1 )

local deck = MOAIGfxQuad2D.new()
deck:setTexture( plugin:getTextureName() )
deck:setRect( -0.5, -0.5, 0.5, 0.5 ) -- HACK: Currently for scaling we need to set the deck's rect to 1x1
system:setDeck( deck )

system:start ()
emitter:start ()

layer:insertProp ( system )

