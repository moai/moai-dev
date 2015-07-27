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

MOAISim.openWindow ( "moai", 640, 480 )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "moai.png" )
gfxQuad:setRect ( -128, -128, 128, 128 )
gfxQuad:setUVRect ( 0, 1, 1, 0 )

gfxQuad:setHitMask ( "moai.png" )
gfxQuad:setHitMaskScalar ( 1, 1, 1, 1 )
gfxQuad:setHitMaskThreshold ( 1, 1, 1, 1 ) -- just hit test the white pixels

prop = MOAIProp.new ()
prop:setDeck ( gfxQuad )
prop:setHitGranularity ( MOAIProp.HIT_TEST_FINE )
layer:insertProp ( prop )

CONST = MOAIParticleScript.packConst

----------------------------------------------------------------
local render = MOAIParticleScript.new ()

render:sprite			()
render:set				( MOAIParticleScript.SPRITE_RED, CONST ( 0.0 ))
render:rand				( MOAIParticleScript.SPRITE_GREEN, CONST ( 0.5 ), CONST ( 1 ))
render:rand				( MOAIParticleScript.SPRITE_BLUE, CONST ( 0.5 ), CONST ( 1 ))

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -2, -2, 2, 2 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 1024, 2 )
system:reserveSprites ( 1024 )
system:reserveStates ( 1 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

-- state 1
state1 = MOAIParticleState.new ()
state1:setTerm ( .75, 1.25 )
state1:setRenderScript ( render )
system:setState ( 1, state1 )

emitter = MOAIParticleTimedEmitter.new ()
emitter:setLoc ( 0, 0 )
emitter:setSystem ( system )
emitter:setFrequency ( 1 / 60 )
emitter:setEmission ( 12 )
emitter:setRect ( -128, -128, 128, 128 )
emitter:start ()

emitter:setMask ( prop )

function clickCallback ( down )
	
	if down then
		
		local x, y = MOAIInputMgr.device.pointer:getLoc ()
		x, y = layer:wndToWorld ( x, y )
		if prop:inside ( x, y ) then
			prop:addRot ( 0, 0, 5 )
		end
	end
end

MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
