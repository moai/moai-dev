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

testStr = MOAISerializer.serializeToString ({ test, transform })

compiled = string.dump ( loadstring ( testStr, '' ))
file = io.open ( 'out.lua', 'wb' )
file:write ( compiled )
file:close ()

test, transform = unpack ( dofile ( 'out.lua' ))

testStr = MOAISerializer.serializeToString ({ test, transform })

print ( testStr )
