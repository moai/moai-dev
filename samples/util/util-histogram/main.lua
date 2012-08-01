----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

MOAISim.setHistogramEnabled ( true )

objects = {}

table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())
table.insert ( objects, MOAIProp.new ())

table.insert ( objects, MOAILayer.new ())
table.insert ( objects, MOAILayer.new ())

table.insert ( objects, MOAITransform.new ())

print ( "REPORTING HISTOGRAM" )
MOAISim.reportHistogram ()
print ()

print ( "GETTING, ITERATING HISTOGRAM" )
histogram = MOAISim.getHistogram ()
for k, v in pairs ( histogram ) do
	print ( k, v )
end
print ()

print ( "FORCING GARBAGE COLLECTION" )
objects = nil
MOAISim.forceGarbageCollection ()
print ()

print ( "REPORTING HISTOGRAM" )
MOAISim.reportHistogram ()
print ()


