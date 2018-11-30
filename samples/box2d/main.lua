----------------------------------------------------------------
-- Copyright (c) 2010-2013 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

local function printf ( ... )
	return io.stdout:write ( string.format ( ... ))
end 

local width = 640
local height = 480
MOAISim.openWindow ( "test", width, height  )

viewport = MOAIViewport.new ()
viewport:setSize ( width, height )
viewport:setScale ( width / ( width / 640 ), height / ( height / 480 ))

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

debugLayer = MOAITableViewLayer.new ()
debugLayer:setViewport ( viewport )
debugLayer:setRenderTable ( world )
debugLayer:pushRenderPass ()

worldBody = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = worldBody:addRect ( -( 300 / 2 ), -200, 300 / 2, -300 )
fixture2:setFilter ( 0x02 )
fixture2:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN + MOAIBox2DArbiter.END, 0x00 )

layer = MOAIPartitionViewLayer.new ()
layer:setViewport ( viewport )
layer:pushRenderPass ()

deck = MOAISpriteDeck2D.new ()
deck:setTexture ( '../resources/moai.png' )
deck:setQuad ( 0, -25, 25, 0, 0, 25, -25, 0 )
deck:setUVRect ( 0, 0, 1, 1 )

function addSprite ( x, y )

	x = x or 0
	y = y or 0

	local body = world:addBody ( MOAIBox2DBody.DYNAMIC )
	body:setTransform ( x, y )

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

	local sprite = MOAIGraphicsProp.new ()
	sprite:setDeck ( deck )
	sprite.body = body
	sprite:setParent ( body )
	sprite:setPartition ( layer )
end

function pointerCallback ( x, y )

	worldX, worldY = layer:wndToWorld ( x, y )

	if pick then
		mouseJoint:setTarget ( worldX, worldY )
	end
end

function clickCallback ( down )
	if down then
		pick = layer:getLayerPartition ():hullForPoint ( worldX, worldY )
		if pick then

			mouseBody = world:addBody ( MOAIBox2DBody.DYNAMIC )
			mouseBody:setTransform ( worldX, worldY )

			mouseJoint = world:addMouseJoint ( mouseBody, pick.body, worldX, worldY,  10000.0 * pick.body:getMass ());
			mouseJoint:setDampingRatio ( 2 ) ;
		else
			addSprite ( worldX, worldY )
		end
	else
		if pick then
			--also destroys joint
			mouseBody:destroy ()
			mouseBody = nil
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
