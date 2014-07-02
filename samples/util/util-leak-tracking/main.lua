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

print ( "REPORTING LEAKS" )
MOAILuaRuntime.reportLeaks ()
print ()
