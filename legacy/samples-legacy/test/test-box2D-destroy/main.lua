-- EXPECTED BEHAVIOR:
-- When the dynamic circle object touches the other objects, the other objects should be destroyed.
-- First it will hit the static object, then the rotating kinematic object.
-- If the fixtures and body are not set to nil immediately upon calling destroy, Moai crashes.

-- Could it be possible that if Box2D crashes when a body is destroyed, even if it's being set to nil, it means that some of the data is being retained?

MOAISim.openWindow ( "Box2D", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 64, 48 )

camera = MOAITransform.new ()

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
layer:setCamera ( camera )
MOAISim.pushRenderPass ( layer )

gravity = -32

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setGravity ( 0, gravity )
world:start ()

debuglayer = MOAILayer2D.new ()
debuglayer:setViewport ( viewport )
debuglayer:setCamera ( camera )
MOAISim.pushRenderPass ( debuglayer )
debuglayer:setBox2DWorld ( world )

body = world:addBody ( MOAIBox2DBody.DYNAMIC, 0, 4 )
body:setMassData ( 1, 0, 0, 0 )
body:setLinearDamping ( 1.5 )

fixture = body:addCircle ( 0, 0, 2 )
fixture:setRestitution ( 0 )
fixture:setFriction ( .1 )
fixture.killsYou = true

wallbodies = world:addBody ( MOAIBox2DBody.STATIC )
floorfixture = wallbodies:addRect ( -32, -14, 32, -15 )

spinbodies = world:addBody ( MOAIBox2DBody.KINEMATIC, 0, -20 )
spinfixture = spinbodies:addRect ( -2, 1, 2, -1 )
spinbodies:setAngularVelocity ( 45 )

wallbodies.someData = "blegh"
wallbodies.whatever = { "stupid", 200, function () print ( "hello" ) end }
spinbodies.someData = "poopy"
spinbodies.whatever = { "argh", 1, function () print ( "sup" ) end }

function collision_test_a ( event, a, b, arbiter )
	
	if b.killsYou then
		
		if floorfixture then
			floorfixture:destroy ()
			-- floorfixture = nil
		end
		if wallbodies then
			wallbodies:destroy ()
			-- wallbodies = nil
		end
	end
end

function collision_test_b ( event, a, b, arbiter )
	
	if b.killsYou then
		
		if spinfixture then
			spinfixture:destroy ()
			-- spinfixture = nil
		end
		if spinbodies then
			spinbodies:destroy ()
			-- spinbodies = nil
		end
	end
end

floorfixture:setCollisionHandler ( collision_test_a, MOAIBox2DArbiter.ALL )
spinfixture:setCollisionHandler ( collision_test_b, MOAIBox2DArbiter.ALL )
