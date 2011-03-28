local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

MOAISim.openWindow ( "cathead", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 16, 0 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

function onCollide ( event )

	if event == MOAIBox2DArbiter.BEGIN then
		print ( 'begin!' )
	end
	
	if event == MOAIBox2DArbiter.END then
		print ( 'end!' )
	end
	
	if event == MOAIBox2DArbiter.PRE_SOLVE then
		print ( 'pre!' )
	end
	
	if event == MOAIBox2DArbiter.POST_SOLVE then
		print ( 'post!' )
	end
end

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setGravity ( 0, -10 )
world:start ()
layer:setBox2DWorld ( world )

body = world:addBody ( MOAIBox2DBody.DYNAMIC )

fixture = body:addRect ( -1, -1, 1, 1 )
fixture:setDensity ( 1 )
fixture:setFriction ( 0.3 )
fixture:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN )

body:resetMassData ()
body:applyAngularImpulse ( 2 )

body2 = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = body2:addRect ( -5, -5, 5, -3 )
fixture2:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( 'cathead.png' )
texture:setRect ( -1, -1, 1, 1 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
sprite:setParent ( body )
layer:insertProp ( sprite )

--[[
texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "cathead.png" )
texture:setRect ( -1, -1, 1, 1 )

sprite = MOAIProp2D.new ()
sprite:setDeck ( texture )
sprite:moveRot ( 360, 1.5 )
--layer:insertProp ( sprite )
]]--

--[[
--polygon
poly = {
	0, 64,
	64, 0,
	0, -64,
	-64, 0,
}

function makeThing ()

	mass = 1
	moment = MOAICpShape.momentForPolygon ( mass, poly )

	body = MOAICpBody.new ( 1, moment )
	space:insertProp ( body )

	shape = body:addPolygon ( poly )
	shape:setElasticity ( 0.8 )
	shape:setFriction ( 0.8 )
	shape:setType ( 1 )
	shape.name = "thing"
	space:insertProp ( shape )

	sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	sprite:setParent ( body )
	layer:insertProp ( sprite )
end

function makeThings ( n )
	for i = 1, n do
		makeThing ()
	end
end

makeThings ( 15 )

-- set up the walls
body = space:getStaticBody ()

function addSegment ( x0, y0, x1, y1 )
	shape = body:addSegment ( x0, y0, x1, y1 )
	shape:setElasticity ( 1 )
	shape:setFriction ( 0.1 )
	shape:setType ( 2 )
	shape.name = "wall"
	space:insertProp ( shape )
end

addSegment ( -320, -240, 320, -240 )
addSegment ( -320, 240, 320, 240 )
addSegment ( -320, -240, -320, 240 )
addSegment ( 320, -240, 320, 240 )

-- test out the collision handlers
function handleCollisions ( event, a, b, arb )

	if ( event == MOAICpSpace.BEGIN ) then print ( "BEGIN" ) end
	if ( event == MOAICpSpace.PRE_SOLVE ) then print ( "PRE_SOLVE" ) end
	if ( event == MOAICpSpace.POST_SOLVE ) then print ( "POST_SOLVE" ) end
	if ( event == MOAICpSpace.SEPARATE ) then print ( "SEPARATE" ) end

	print ( a.name )
	print ( b.name )
	print ( arb:countContacts ())
	
	return true
end

space:setCollisionHandler ( 1, 2, handleCollisions )

-- set up the mouse body
mouseBody = MOAICpBody.new ( MOAICp.INFINITY, MOAICp.INFINITY )

mouseX = 0
mouseY = 0

MOAIInputMgr.device.pointer:setCallback (

	function ( x, y )
		mouseX, mouseY = layer:wndToWorld ( x, y )
		mouseBody:setPos ( mouseX, mouseY )
	end
)

MOAIInputMgr.device.mouseLeft:setCallback (

	function ( down )
	
		if down then
			
			pick = space:shapeForPoint ( mouseX, mouseY )
			
			if pick then
				
				body = pick:getBody ()
				
				mouseJoint = MOAICpConstraint.newPivotJoint (
					mouseBody,
					body,
					0,
					0,
					body:worldToLocal ( mouseX, mouseY )
				)
				mouseJoint:setMaxForce ( 50000 )
				mouseJoint:setBiasCoef ( 0.15 )
				space:insertProp ( mouseJoint )
			end
		else
			
			if mouseJoint then
				space:removeProp ( mouseJoint )
				mouseJoint = nil
			end
		end
	end
)
]]--
