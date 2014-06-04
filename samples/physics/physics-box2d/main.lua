----------------------------------------------------------------
-- Copyright (c) 2010-2013 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

local width = MOAIEnvironment.horizontalResolution or 640
local height = MOAIEnvironment.verticalResolution or 480
MOAISim.openWindow ( "test", width ,height  )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale (width / (width/640), height / (height/480))

layer = MOAILayer2D.new ()
layer:setPartition( MOAIPartition.new() )
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
world:setUnitsToMeters ( .05 )
world:start ()
layer:setUnderlayTable ({ world })

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -(300/2), -200, 300/2, -300)
fixture2:setFilter ( 0x02 )
fixture2:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN + MOAIBox2DArbiter.END, 0x00 )

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
	fixture:setDensity ( 1 )
	fixture:setFriction ( 0.3 )
	fixture:setFilter ( 0x01 )
	fixture:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN + MOAIBox2DArbiter.END, 0x02 )

	body:resetMassData ()
	body:applyAngularImpulse ( 80 )

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
		pick = layer:getPartition():propForPoint ( worldX, worldY)
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


if MOAIInputMgr.device.pointer then
    MOAIInputMgr.device.pointer:setCallback ( pointerCallback )
    MOAIInputMgr.device.mouseLeft:setCallback ( clickCallback )
    MOAIInputMgr.device.mouseRight:setCallback ( rightclickCallback )
else
    MOAIInputMgr.device.touch:setCallback (
        function ( eventType, idx, x, y, tapCount )
            local isDown = eventType == MOAITouchSensor.TOUCH_DOWN
            if (tapCount > 1) then
                rightclickCallback(isDown)
            elseif eventType ~= MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
                clickCallback(isDown)
            elseif eventType == MOAITouchSensor.TOUCH_MOVE then
                pointerCallback(x,y)
            end
        end
    )
end
