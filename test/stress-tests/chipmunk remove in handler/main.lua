----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( 960, 640 )
viewport:setScale ( 960, 640 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

space = MOAICpSpace.new ()
space:setGravity ( 0, -250 )
space:setIterations ( 10 )
space:start ()

layer:setCpSpace ( space )

body = MOAICpBody.new ( 1, MOAICp.INFINITY )
body:setPos ( 0, 128 )
space:insertPrim ( body )
shape = body:addRect ( -64, -64, 64, 64 )
shape:setElasticity ( 0.3 )
shape:setFriction ( 0.8 )
shape:setType ( 1 )
space:insertPrim ( shape )

body2 = space:getStaticBody ()
shape2 = body2:addRect ( -360, -200, 360, -240 )
shape2:setElasticity ( 0 )
shape2:setFriction ( 0.1 )
shape2:setType ( 2 )
space:insertPrim ( shape2 )

function handleCollisions ( event, a, b, arb )
	space:removePrim ( shape )
	shape = nil

	return true
end

space:setCollisionHandler ( 1, 2, MOAICpSpace.BEGIN, handleCollisions )

MOAISim.openWindow ( "stress test", 960, 640 )
