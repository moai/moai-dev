----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.setHistogramEnabled ( true )

function makeCircularRef ()

	-- create some objects
	local world = MOAIBox2DWorld.new ()
	local body = world:addBody ( MOAIBox2DBody.DYNAMIC )
	local fixture = body:addRect ( -5, -5, 5, -3 )

	local function onCollide ( event )

		-- trap everything in a closure
		world.foo ()
		body.foo ()
		fixture.baz ()
	end
	
	-- create a circular reference
	fixture:setCollisionHandler ( onCollide, MOAIBox2DArbiter.BEGIN, 0x02 )
end

makeCircularRef ()

-- and try to collect
MOAISim.reportHistogram ()
MOAISim.forceGarbageCollection ()
MOAISim.reportHistogram ()
MOAISim.forceGarbageCollection ()

world = MOAIBox2DWorld.new ()
body = world:addBody ( MOAIBox2DBody.DYNAMIC )
fixture = body:addRect ( -5, -5, 5, -3 )
