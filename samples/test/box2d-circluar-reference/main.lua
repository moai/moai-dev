----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

print ( "hello, moai!" )

MOAISim.forceGarbageCollection ()
MOAISim.setHistogramEnabled ( true )

local b2dWorld = MOAIBox2DWorld.new ()
local layer = MOAILayer2D.new ()

local function makeThingBody ( theLayer )

	local layer = theLayer 
	
	local thing = {}
	function thing:foo ( )
		if layer then print "yay" end
	end
	
	return thing
end

local function makePhysicsBody ( theLayer )

	local layer = theLayer 
	local prop = MOAIProp2D.new ()
	prop.layer = layer
	layer:insertProp ( prop )
	
	prop.body = b2dWorld:addBody ( MOAIBox2DBody.DYNAMIC )	
	prop.fixture = prop.body:addRect ( -32, -32, 32, 32 )
	prop.fixture.isType = function ( self ) return 1 end
	
	local thing = makeThingBody ( layer )
	
	local function collision ( eventType, a, b, arbiter )

		if eventType == MOAIBox2DArbiter.PRE_SOLVE then
		
			prop:bar ()
			if ( a:isType () ) then
				prop:baz ()
				thing:foo ()
			end
		
		elseif eventType == MOAIBox2DArbiter.POST_SOLVE then
			
			prop:foo ()
		end
	end

	prop.fixture:setCollisionHandler ( collision )
	return prop
end

for i = 1, 3 do
	
	physBody = makePhysicsBody ( layer )
	physBody.body:destroy ()
	layer:removeProp ( physBody )
	physBody = nil
	
	print ( "---- Iteration " .. i .. " ----" )
	MOAISim.forceGarbageCollection ()	
	MOAISim.reportHistogram ()	
end

b2dWorld = nil
layer = nil

print ( "---- FINAL ----" )
MOAISim.forceGarbageCollection ()	
MOAISim.reportHistogram ()