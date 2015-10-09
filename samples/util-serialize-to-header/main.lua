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

-- convert the serialized objects to Lua bytecode
compiled = string.dump ( loadstring ( testStr, '' )) -- using empty string for name of chunk

-- convert the Lua bytecode to a cpp header called 'serialized_lua' with 12 columns
header = MOAIDataBuffer.toCppHeader ( compiled, 'serialized_lua', 12 )

-- print the header
print ( header )

-- write the header to a file
file = io.open ( '../temp/serialized_lua.h', 'wb' )
file:write ( header )
file:close ()
