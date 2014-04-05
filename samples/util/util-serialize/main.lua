----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

-- make some test data
test = {
	foo = 0,
	bar = 1,
	baz = 92,
}

transform = MOAITransform.new ()

-- create the serializer and add the objects in the order we want them back
serializer = MOAISerializer.new ()
serializer:serialize ( test )
serializer:serialize ( transform )

-- the serializer will produce a chunk that, when executed, will return
-- the objects in the order they were paseed in
testStr = serializer:exportToString ()

-- we can write the chunk out to a file as is, or compile using string.dump before writing...
compiled = string.dump ( loadstring ( testStr, '' )) -- using empty string for name of chunk
file = io.open ( 'out.lua', 'wb' )
file:write ( compiled )
file:close ()

-- time to deserialize the objects
test, transform = dofile ( 'out.lua' )

-- we'll serialize all over again, but this time print the result to observe the chunk format
serializer = MOAISerializer.new ()
serializer:serialize ( test )
serializer:serialize ( transform )
testStr = serializer:exportToString ()

-- serialized objects, pretty printed
print ( testStr )