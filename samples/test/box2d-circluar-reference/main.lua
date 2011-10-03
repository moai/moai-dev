----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

local b2dWorld = MOAIBox2DWorld.new ()

local function makePhysicsBody ()

	local prop = MOAIProp2D.new ()
	prop.body = b2dWorld:addBody ( MOAIBox2DBody.DYNAMIC )
	prop.fixture = prop.body:addRect ( -32, -32, 32, 32 )
	prop.fixture.isType = function ( self )
		return 1
	end

	local function collision ( eventType, a, b, arbiter )

		if eventType == MOAIBox2DArbiter.PRE_SOLVE then
		
			prop:bar ()
			if ( a:isType () ) then
				prop:baz ()
			end
		
		elseif eventType == MOAIBox2DArbiter.POST_SOLVE then
			
			prop:foo ()
		end
	end

	prop.fixture:setCollisionHandler ( collision )
	return prop
end

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( false )
for i = 1, 3 do
	
	MOAISim.setHistogramEnabled ( false )
	MOAISim.setHistogramEnabled ( true )
	
	physBody = makePhysicsBody ()
	physBody = nil
	
	print ( "---- Iteration " .. i .. " ----" )
	MOAISim.forceGarbageCollection ()	
	MOAISim.reportHistogram ()	
end

b2dWorld = nil
print ( "---- FINAL ----" )
MOAISim.forceGarbageCollection ()	
MOAISim.reportHistogram ()