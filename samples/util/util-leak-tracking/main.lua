----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAILuaRuntime.setTrackingFlags ( MOAILuaRuntime.TRACK_OBJECTS + MOAILuaRuntime.TRACK_OBJECTS_STACK_TRACE )

objects = {}

table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())

table.insert ( objects, MOAILayer.new ())
table.insert ( objects, MOAILayer.new ())

table.insert ( objects, MOAITransform.new ())

thread1 = coroutine.create ( function ()
	local obj = MOAIGraphicsProp.new ()
	print ( "hi from thread1" )
	coroutine.yield ()
end )

thread2 = MOAICoroutine.new ()
thread2:run ( function ()
	local obj = MOAIGraphicsProp.new ()
	print ( "hi from thread2" )
	coroutine.yield ( 1, 2, 3, 4, 5 )
end )

coroutine.resume ( thread1 )
thread2:floob ()

foo = objects [ 1 ]
bar = objects [ 2 ]
baz = objects [ 3 ]

print ( "REPORTING LEAKS" )
MOAILuaRuntime.reportLeaks ()
print ()
