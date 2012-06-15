----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc. 
-- All Rights Reserved. 
-- http://getmoai.com
----------------------------------------------------------------

function test ()
	print ( 'This is a test of bundled lua.' )
end

-- dump the function to Lua bytecode
compiled = string.dump ( test )

-- convert the Lua bytecode to a cpp header called 'bundled_lua' with 12 columns
header = MOAIDataBuffer.toCppHeader ( compiled, 'bundled_lua', 12 )

-- print the header
print ( header )

-- write the header to a file
file = io.open ( 'bundled_lua.h', 'wb' )
file:write ( header )
file:close ()
