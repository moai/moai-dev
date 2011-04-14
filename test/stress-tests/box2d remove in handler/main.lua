----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 16, 0 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

function onCollide ( event, a, b )
	body:destroyFixture ( a )
end

world = MOAIBox2DWorld.new ()
world:setGravity ( 0, -5 )
world:start ()

layer:setBox2DWorld ( world )

body = world:addBody ( MOAIBox2DBody.DYNAMIC, 0, 1 )
fixture = body:addRect ( -1, -1, 1, 1 )
fixture:setDensity ( 1 )
fixture:setFriction ( 0.3 )
fixture:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN )

body2 = world:addBody ( MOAIBox2DBody.STATIC )
fixture2 = body2:addRect ( -5, -5, 5, -3 )

MOAISim.openWindow ( "stress test", 960, 640 )
