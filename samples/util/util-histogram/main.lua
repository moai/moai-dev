----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAILuaRuntime.setTrackingFlags ( MOAILuaRuntime.TRACK_OBJECTS )

objects = {}

table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())

table.insert ( objects, MOAILayer.new ())
table.insert ( objects, MOAILayer.new ())

table.insert ( objects, MOAITransform.new ())

print ( "REPORTING HISTOGRAM" )
MOAILuaRuntime.reportHistogram ( 'report.txt' )
print ()

print ( "GETTING, ITERATING HISTOGRAM" )
histogram = MOAILuaRuntime.getHistogram ()
for k, v in pairs ( histogram ) do
	print ( k, v )
end
print ()

print ( "FORCING GARBAGE COLLECTION" )
objects = nil
MOAISim.forceGC ()
print ()

print ( "REPORTING HISTOGRAM" )
MOAILuaRuntime.reportHistogram ()
print ()
