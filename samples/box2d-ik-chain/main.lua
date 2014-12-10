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

-- set up the world and start its simulation
world = MOAIBox2DWorld.new ()
world:setGravity ( 0, 0 )
world:setUnitsToMeters ( .05 )
world:start ()
layer:setUnderlayTable ({ world })


local chain = {}

function dist ( x1, y1, x2, y2 )
	return math.sqrt ((( x2 - x1 ) ^ 2 ) + (( y2 - y1 ) ^ 2 ))
end

function addSprite ( x, y, r, density, type )

	local body = world:addBody ( type or MOAIBox2DBody.DYNAMIC )
	body:setTransform ( worldX, worldY )

	local r = r or math.random ( 10, 30 )

	local fixture = body:addCircle ( 0, 0, r )
	fixture:setDensity ( density or 1 )
	fixture:setFriction ( 0.3 )
	fixture:setFilter ( 0x01 )

	body:resetMassData ()
	--body:applyAngularImpulse ( 80 )

	local sprite = MOAIProp2D.new ()
	sprite:setBounds ( -r, -r, 0, r, r, 0 )
	sprite.body = body
	sprite:setParent ( body )
	layer:insertProp ( sprite )

	local top = #chain

	if top > 0 then

		local parent = chain [ top ]
		local parentX, parentY = parent:getPosition ()
		local d1 = dist ( parentX, parentY, x, y )

		world:addDistanceJoint ( parent, body, parentX, parentY, x, y )
		world:addRevoluteJoint ( parent, body, parentX, parentY )

		if top > 1 then

			local grandParent = chain [ top - 1 ]
			local grandParentX, grandParentY = grandParent:getPosition ()
			local d2 = dist ( grandParentX, grandParentY, x, y )

			local distanceJoint = world:addDistanceJoint ( grandParent, body, grandParentX, grandParentY, x, y )
			distanceJoint:setLength ( d1 + d2 )
			distanceJoint:setFrequency ( 2 )
			distanceJoint:setDampingRatio ( 10 )
		end
	end

	chain [ top + 1 ] = body
end

addSprite ( 0, 0, 40, 1000, MOAIBox2DBody.STATIC )

function pointerCallback ( x, y )
	worldX, worldY = layer:wndToWorld ( x, y )
	if mouseJoint then
		mouseBody:setTransform ( worldX, worldY )
		mouseJoint:setTarget ( worldX, worldY )
	end
end

function clickCallback ( down )

	if down then
		pick = layer:getPartition ():propForPoint ( worldX, worldY )
		if pick then
			mouseBody = world:addBody ( MOAIBox2DBody.STATIC )
			mouseBody:setTransform ( worldX, worldY )
			mouseJoint = world:addMouseJoint ( mouseBody, pick.body, worldX, worldY, 10000.0 * pick.body:getMass ());
			mouseJoint:setDampingRatio ( 2 );
		end
	else
		if pick then
			--also destroys joint
			mouseBody:destroy ()
			mouseBody = nil
			mouseJoint = nil
			pick = nil
		end
	end
end

function rightclickCallback ( down )
	if down then
	  addSprite ( worldX, worldY )
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
            if ( tapCount > 1 ) then
                rightclickCallback ( isDown )
            elseif eventType ~= MOAITouchSensor.TOUCH_MOVE then
                pointerCallback ( x, y )
                clickCallback ( isDown )
            elseif eventType == MOAITouchSensor.TOUCH_MOVE then
                pointerCallback ( x, y )
            end
        end
    )
end
