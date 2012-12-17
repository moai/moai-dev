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
world:setUnitsToMeters ( 2 )
world:start ()
layer:setBox2DWorld ( world )

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -5, -5, 5, -3 )
fixture2:setFilter ( 0x02 )
fixture2:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN + MOAIBox2DArbiter.END, 0x00 )

texture = MOAIGfxQuad2D.new ()
texture:setTexture ( 'moai.png' )
texture:setRect ( -0.5, -0.5, 0.5, 0.5 )

function addSprite()
	local body = world:addBody ( MOAIBox2DBody.DYNAMIC )

	local poly = {
		0, -1,
		1, 0,
		0, 1,
		-1, 0,
	}

	local fixture = body:addPolygon ( poly )
	fixture:setDensity ( 1 )
	fixture:setFriction ( 0.3 )
	fixture:setFilter ( 0x01 )
	fixture:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN + MOAIBox2DArbiter.END, 0x02 )

	body:resetMassData ()
	body:applyAngularImpulse ( 2 )

	local sprite = MOAIProp2D.new ()
	sprite:setDeck ( texture )
	sprite.body = body
	sprite:setParent ( body )
	layer:insertProp ( sprite )
end

function pointerCallback ( x, y )
	worldX, worldY = layer:wndToWorld ( x, y )

	if pick then
	  mouseJoint:setTarget(worldX, worldY)
	end
end

function clickCallback ( down )
	if down then
		pick = layer:getPartition():propForPoint ( worldX, worldY )
		if pick then
			mouseBody = world:addBody( MOAIBox2DBody.DYNAMIC )
			mouseBody:setTransform(worldX, worldY)

			mouseJoint = world:addMouseJoint(mouseBody, pick.body, worldX, worldY,  10000.0 * pick.body:getMass());
			mouseJoint:setDampingRatio(2);
		end
	else
		if pick then
			--also destroys joint
			mouseBody:destroy()
			mouseBody = nil
			pick = nil
		end
	end
end

function rightclickCallback ( down )
	if down then
	  addSprite()
	end
end

MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
MOAIInputMgr.device.mouseRight:setCallback ( rightclickCallback )
