-- Sample of MOAIBox2DMotorJoint, based on the Box2D 2.3.0 Testbed/MotorJoint.h sample.
-- Click the mouse to move the prop/body, watch the Box2D motorJoint return the body to rest.
-- Author: www.fivevolthigh.com

local width = 640
local height = 480
MOAISim.openWindow ( "Box2D 2.3.0 MotorJoint", width ,height  )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale (width / (width/640), height / (height/480))

layer = MOAILayer2D.new ()
layer:setPartition( MOAIPartition.new() )
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setUnitsToMeters(0.05)
layer:setUnderlayTable ({ world })

worldBody = world:addBody ( MOAIBox2DBody.STATIC )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( 'moai.png' )
texture:setRect ( -25/2, -25/2, 25/2, 25/2 )

function addSprite()
	local body = world:addBody ( MOAIBox2DBody.DYNAMIC )
  
	local poly = {
		0, -25,
		25, 0,
		0, 25,
		-25, 0,
	}

	local fixture = body:addPolygon ( poly )
	fixture:setDensity ( 0.1 )
	fixture:setFriction ( 0.6 )
  body:resetMassData ()

	local sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	sprite.body = body
	sprite:setParent ( body )
	layer:insertProp ( sprite )	
  
	return sprite
end

-- Set up initial conditions for the motorJoint
spriteA = addSprite()
spriteA.body:setTransform(0, 80 )
motorJoint = world:addMotorJoint(worldBody, spriteA.body)
motorJoint:setMaxForce(spriteA.body:getMass() * 60 * 15)  -- Mass * Hz * Force
motorJoint:setMaxTorque(spriteA.body:getMass() * 60 * 15) -- Mass * Hz * Force

-- Let Box2D drive the motor joint to return the body to initial conditions over time
world:start ()

mouseX = 0
mouseY = 0
function pointerCallback(x, y)
  mouseX, mouseY = layer:wndToWorld(x, y)
end

function clickCallback(down)
  if down then
    -- Perturb the Box2D body and wake it up so the motorJoint can be applied
    spriteA.body:setTransform(mouseX, mouseY)
    spriteA.body:setAwake(true)
  end
end

if MOAIInputMgr.device.pointer then
	MOAIInputMgr.device.pointer:setCallback(pointerCallback)
	MOAIInputMgr.device.mouseLeft:setCallback(clickCallback)
end