require 'ParticleHelper'

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

MOAISim.openWindow ( "cathead", 640, 480 )

CONST = MOAIParticleScript.packConst


local reg={}	--shared register table


local render=makeParticleScript(function (  )
	
	ix=cos(time*10)*100
	iy=sin(time*10)*30

	sprite()
	sp.x=p.x+ix
	sp.y=p.y+iy
	sp.opacity=ease(1,0,EaseType.EASE_OUT)

	sprite()
	sp.x=p.x+iy
	sp.y=p.y+ix
	sp.opacity=ease(1,0,EaseType.EASE_OUT)

end,reg)

----------------------------------------------------------------
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -16, -16, 16, 16 )

system = MOAIParticleSystem.new ()
system:reserveParticles ( 256, 1 )
system:reserveSprites ( 256 )
system:reserveStates ( 1 )
system:setDeck ( texture )
system:start ()
layer:insertProp ( system )

state = MOAIParticleState.new ()
state:setTerm (3)
state:setRenderScript ( render )
system:setState ( 1, state )

emitter = MOAIParticleTimedEmitter.new ()
emitter:setLoc ( 0, 0 )
emitter:setSystem ( system )
emitter:setFrequency(0.2)
emitter:start ()


function pointerCallback ( x, y )
	
	mouseX, mouseY = layer:wndToWorld ( x, y )
	
	emitter:setLoc ( mouseX, mouseY )
	
end

MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
