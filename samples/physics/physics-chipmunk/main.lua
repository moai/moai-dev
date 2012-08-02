----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

MOAISim.openWindow ( "test", 640, 480 )

viewport = MOAIViewport.new ()
viewport:setSize ( 640, 480 )
viewport:setScale ( 640, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( "moai.png" )
texture:setRect ( -32, -32, 32, 32 )

-- set up the space and start its simulation
space = MOAICpSpace.new ()
space:setGravity ( 0, -2000 )
space:setIterations ( 5 )
space:start ()

-- attach the layer to the space for debug drawing
layer:setCpSpace ( space )

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
	space:insertPrim ( body )

	shape = body:addPolygon ( poly )
	shape:setElasticity ( 0.8 )
	shape:setFriction ( 0.8 )
	shape:setType ( 1 )
	shape.name = "thing"
	space:insertPrim ( shape )

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
	space:insertPrim ( shape )
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

--space:setCollisionHandler ( 1, 2, MOAICpSpace.BEGIN, handleCollisions )

if ( MOAIInputMgr.device.pointer     and
	 MOAIInputMgr.device.mouseLeft ) then

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
					space:insertPrim ( mouseJoint )
				end
			else
				
				if mouseJoint then
					space:removePrim ( mouseJoint )
					mouseJoint = nil
				end
			end
		end
	)
end